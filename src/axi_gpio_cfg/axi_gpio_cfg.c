//****************************************Copyright (c)***********************************//
//原子哥在线教学平台：www.yuanzige.com
//技术支持：www.openedv.com
//淘宝店铺：http://openedv.taobao.com
//关注微信公众平台微信号："正点原子"，免费获取ZYNQ & FPGA & STM32 & LINUX资料。
//版权所有，盗版必究。
//Copyright(C) 正点原子 2018-2028
//All rights reserved
//----------------------------------------------------------------------------------------
// File name:           axi_gpio_cfg.c
// Last modified Date:  2020/04/15 10:59:46
// Last Version:        V1.0
// Descriptions:        AXI GPIO配置
//----------------------------------------------------------------------------------------
// Created by:          正点原子
// Created date:        2023/06/30 15:59:52
// Version:             V1.0
// Descriptions:        The original version
//
//----------------------------------------------------------------------------------------
//****************************************************************************************//

#include"axi_gpio_cfg.h"

#define AXI_GPIO_0_ID XPAR_AXI_GPIO_0_DEVICE_ID	//AXI GPIO 0器件 ID

#define AXI_GPIO_0_CHANEL1   1

XGpio   axi_gpio_inst0;      //AXI GPIO 0 驱动实例

//AXI GPIO初始化
void axi_gpio_init(void)
{
    //AXI GPIO 0驱动
    XGpio_Initialize(&axi_gpio_inst0, AXI_GPIO_0_ID);
    //配置AXI GPIO 0 通道1为输入
    XGpio_SetDataDirection(&axi_gpio_inst0, AXI_GPIO_0_CHANEL1,1);
}

//通过AXI GPIO获取FIFO数据个数
//u32 get_fifo_count(void)
//{
//    u32 fifo_count = 0;
//    fifo_count = XGpio_DiscreteRead(&axi_gpio_inst0, AXI_GPIO_0_CHANEL1);
//    return fifo_count;
//}

void axi_gpio_out1(void){
	XGpio_DiscreteWrite(&axi_gpio_inst0, 1, 0x01);
}

void axi_gpio_out0(void){
	XGpio_DiscreteWrite(&axi_gpio_inst0, 1, 0x00);
}

