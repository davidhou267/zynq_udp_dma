//****************************************Copyright (c)***********************************//
//原子哥在线教学平台：www.yuanzige.com
//技术支持：www.openedv.com
//淘宝店铺：http://openedv.taobao.com
//关注微信公众平台微信号："正点原子"，免费获取ZYNQ & FPGA & STM32 & LINUX资料。
//版权所有，盗版必究。
//Copyright(C) 正点原子 2020-2030
//All rights reserved
//----------------------------------------------------------------------------------------
// File name:           main.c
// Last modified Date:  2023/08/05 15:59:46
// Last Version:        V1.0
// Descriptions:        PS端网口传输OV5640摄像头视频在上位机显示
//----------------------------------------------------------------------------------------
// Created by:          正点原子
// Created date:        2023/08/02 15:59:52
// Version:             V1.0
// Descriptions:        The original version
//server_netif
//----------------------------------------------------------------------------------------
//****************************************************************************************//
/***************************** Include Files *********************************/
 #include <stdio.h>
 #include <stdlib.h>
 #include <string.h>

 #include "xil_types.h"
 #include "xparameters.h"
 //#include "emio_sccb_cfg/emio_sccb_cfg.h"
 #include "axi_gpio_cfg/axi_gpio_cfg.h"
 #include "axi_dma/axi_dma.h"
 //#include "ov5640/ov5640_init.h"
 #include "sys_intr/sys_intr.h"

 #include "udp_perf_server.h"
 #include "netif/xadapter.h"
 #include "platform.h"
 #include "platform_config.h"
 #include "xil_printf.h"
 #include "lwip/tcp.h"

 #include "sleep.h"
 #include "lwip/priv/tcp_priv.h"
 #include "lwip/init.h"
 #include "lwip/inet.h"
 #include "xil_cache.h"

 extern volatile int TcpFastTmrFlag;
 extern volatile int TcpSlowTmrFlag;

 #define DEFAULT_IP_ADDRESS  "192.168.1.10"
 #define DEFAULT_IP_MASK     "255.255.255.0"
 #define DEFAULT_GW_ADDRESS  "192.168.1.1"

 void start_application(void);
 void print_app_header(void);
 int lwip_udp_init();
 #define MAX_PKT_LEN     2048      //发送包长度

 //函数声明
 struct netif *netif;
 extern volatile int rx_done;
 extern u8 *rx_buffer_ptr;
 u32 fifo_count = 0;
 u8 dma_start_flag = 0;            //0：启动DMA ，1：关闭DMA

 struct netif server_netif;
 static XScuGic Intc;              //GIC


 int main(void)
 {

     axi_gpio_init();               // 初始AXI-GPIO接口
     axi_dma_cfg();                 // 配置AXI DMA
     Init_Intr_System(&Intc);       // 初始DMA中断系统
     Setup_Intr_Exception(&Intc);   // 启用来自硬件的中断
     dma_setup_intr_system(&Intc);  // 建立DMA中断系统

     lwip_udp_init();               // UDP通信配置

     //接收和处理数据包
     while (1) {

         xemacif_input(netif);


         //FIFO中的读数据计数个数达到发送包长度后，开始启动DMA从FIFO中读取1024个数据存储进DDR中
         if(dma_start_flag == 0){
        	 axi_gpio_out1();
             axi_dma_start(MAX_PKT_LEN);
             axi_gpio_out0();
             dma_start_flag = 1;
         }
         //DMA搬运1024个数据完成后，网口就可以从DDR中取数据进行发送了
         if(rx_done){
             udp_tx_data(rx_buffer_ptr,8192);
             rx_done = 0;
             dma_start_flag = 0;
         }
     }
     return 0;
 }

 static void print_ip(char *msg, ip_addr_t *ip)
 {
     print(msg);
     xil_printf("%d.%d.%d.%d\r\n", ip4_addr1(ip), ip4_addr2(ip),
             ip4_addr3(ip), ip4_addr4(ip));
 }

 static void print_ip_settings(ip_addr_t *ip, ip_addr_t *mask, ip_addr_t *gw)
 {
     print_ip("Board IP:       ", ip);
     print_ip("Netmask :       ", mask);
     print_ip("Gateway :       ", gw);
 }

 //设置静态IP地址
 static void assign_default_ip(ip_addr_t *ip, ip_addr_t *mask, ip_addr_t *gw)
 {
     int err;

     xil_printf("Configuring default IP %s \r\n", DEFAULT_IP_ADDRESS);

     err = inet_aton(DEFAULT_IP_ADDRESS, ip);
     if (!err)
         xil_printf("Invalid default IP address: %d\r\n", err);

     err = inet_aton(DEFAULT_IP_MASK, mask);
     if (!err)
         xil_printf("Invalid default IP MASK: %d\r\n", err);

     err = inet_aton(DEFAULT_GW_ADDRESS, gw);
     if (!err)
         xil_printf("Invalid default gateway address: %d\r\n", err);
 }

 int lwip_udp_init()
 {
     /*设置领航者开发板的MAC地址 */
     unsigned char mac_ethernet_address[] = {
         0x00, 0x0a, 0x35, 0x00, 0x01, 0x02 };

     netif = &server_netif;

     xil_printf("\r\n\r\n");
     xil_printf("-----lwIP RAW Mode UDP Server Application-----\r\n");

     /* 初始化lwIP*/
     lwip_init();

     /* 将网络接口添加到netif_list，并将其设置为默认网络接口 （用于输出未找到特定路由的所有数据包） */
     if (!xemac_add(netif, NULL, NULL, NULL, mac_ethernet_address,
                 PLATFORM_EMAC_BASEADDR)) {
         xil_printf("Error adding N/W interface\r\n");
         return -1;
     }
     netif_set_default(netif);

     /* 指定网络是否已启动*/
     netif_set_up(netif);

     //设置静态IP地址
     assign_default_ip(&(netif->ip_addr), &(netif->netmask), &(netif->gw));

     //打印IP设置
     print_ip_settings(&(netif->ip_addr), &(netif->netmask), &(netif->gw));

     xil_printf("\r\n");

     /* 打印应用程序标题 */
     print_app_header();

     /* 启动应用程序*/
     start_application();
     xil_printf("\r\n");
     return 0;
 }


