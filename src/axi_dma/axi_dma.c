//****************************************Copyright (c)***********************************//
//原子哥在线教学平台：www.yuanzige.com
//技术支持：www.openedv.com
//淘宝店铺：http://openedv.taobao.com
//关注微信公众平台微信号："正点原子"，免费获取ZYNQ & FPGA & STM32 & LINUX资料。
//版权所有，盗版必究。
//Copyright(C) 正点原子 2018-2028
//All rights reserved
//----------------------------------------------------------------------------------------
// File name:           axi_dma.c
// Last modified Date:  2023/08/9 10:59:46
// Last Version:        V1.0
// Descriptions:        Axi dma驱动程序在中断模式下接收数据包
//----------------------------------------------------------------------------------------
// Created by:          正点原子
// Created date:        2019/06/30 15:59:52
// Version:             V1.0
// Descriptions:        The original version
//
//----------------------------------------------------------------------------------------
//****************************************************************************************//

/***************************** Include Files *********************************/
#include "axi_dma.h"

/************************** Constant Definitions *****************************/
#define DMA_DEV_ID             XPAR_AXIDMA_0_DEVICE_ID
#define RX_INTR_ID             XPAR_FABRIC_AXIDMA_0_VEC_ID
#define INTC_DEVICE_ID         XPAR_SCUGIC_SINGLE_DEVICE_ID
#define DDR_BASE_ADDR          XPAR_PS7_DDR_0_S_AXI_BASEADDR   //0x00100000
#define MEM_BASE_ADDR          (DDR_BASE_ADDR + 0x1000000)     //0x01100000
#define RX_BUFFER_BASE         (MEM_BASE_ADDR + 0x00300000)    //0x01400000
#define RESET_TIMEOUT_COUNTER  10000    //复位时间

/************************** Variable Definitions *****************************/

static XAxiDma axidma;     //XAxiDma实例
volatile int rx_done=0;    //接收完成标志
volatile int error;        //传输出错标志
u32 *rx_buffer_ptr;

/************************** Function Definitions *****************************/

int axi_dma_cfg(void)
{

    int status;

    XAxiDma_Config *config;

    rx_buffer_ptr = (u32 *) RX_BUFFER_BASE;

    xil_printf("\r\n--- Entering axi_dma_cfg --- \r\n");

    config = XAxiDma_LookupConfig(DMA_DEV_ID);
    if (!config) {
        xil_printf("No config found for %d\r\n", DMA_DEV_ID);
        return XST_FAILURE;
    }

    //初始化DMA引擎
    status = XAxiDma_CfgInitialize(&axidma, config);
    if (status != XST_SUCCESS) {
        xil_printf("Initialization failed %d\r\n", status);
        return XST_FAILURE;
    }

    if (XAxiDma_HasSg(&axidma)) {
        xil_printf("Device configured as SG mode \r\n");
        return XST_FAILURE;
    }

    xil_printf("AXI DMA CFG Success\r\n");
    return XST_SUCCESS;

}

//启用AXI DMA
int axi_dma_start(u32 pkt_len)
{
    int status;
    u32 a;
    //初始化标志信号
    error   = 0;
    a = (u32)(pkt_len * sizeof(u32)); //计算pek_len字节数
    xil_printf("%d",a);

    status = XAxiDma_SimpleTransfer(&axidma, (u32) rx_buffer_ptr,
    		(u32) (pkt_len*sizeof(u32)), XAXIDMA_DEVICE_TO_DMA);
    if (status != XST_SUCCESS) {
    	xil_printf("AXI DMA Start FAILURE\r\n");
    	return XST_FAILURE;
    }
   // while(!rx_done && !error);//等待DMA将数据从FIFO搬运到DDR
    Xil_DCacheFlushRange((UINTPTR) rx_buffer_ptr, pkt_len); //刷新Data Cache
    return XST_SUCCESS;
}

//DMA RX中断处理函数
void rx_intr_handler(void *callback)
{
    u32 irq_status;
    int timeout;
    XAxiDma *axidma_inst = (XAxiDma *) callback;

    irq_status = XAxiDma_IntrGetIrq(axidma_inst, XAXIDMA_DEVICE_TO_DMA);
    XAxiDma_IntrAckIrq(axidma_inst, irq_status, XAXIDMA_DEVICE_TO_DMA);

    //Rx出错               0x00004000 /**< Error interrupt */
    if ((irq_status & XAXIDMA_IRQ_ERROR_MASK)) {
        error = 1;
        xil_printf("XAxiDma error");
        XAxiDma_Reset(axidma_inst);
        timeout = RESET_TIMEOUT_COUNTER;
        while (timeout) {
            if (XAxiDma_ResetIsDone(axidma_inst))
                break;
            timeout -= 1;
        }
        return;
    }
    //Rx完成     0x00001000 /**< Completion intr */
    if ((irq_status & XAXIDMA_IRQ_IOC_MASK))
        rx_done = 1;

    irq_status = XAxiDma_IntrGetIrq(axidma_inst, XAXIDMA_DEVICE_TO_DMA);
}

//建立DMA中断系统
//  @param   int_ins_ptr是指向XScuGic实例的指针
//  @param   AxiDmaPtr是指向DMA引擎实例的指针
//  @param   rx_intr_id是RX通道中断ID
//  @return：成功返回XST_SUCCESS，否则返回XST_FAILURE
int dma_setup_intr_system(XScuGic * int_ins_ptr)
{
    int status;
    //设置优先级和触发类型
    XScuGic_SetPriorityTriggerType(int_ins_ptr, RX_INTR_ID, 0xA0, 0x3);

    //为中断设置中断处理函数
    status = XScuGic_Connect(int_ins_ptr, RX_INTR_ID,
            (Xil_InterruptHandler) rx_intr_handler, &axidma);
    if (status != XST_SUCCESS) {
        return status;
    }
    XScuGic_Enable(int_ins_ptr, RX_INTR_ID);

    //使能DMA中断
    XAxiDma_IntrEnable(&axidma,XAXIDMA_IRQ_ALL_MASK,XAXIDMA_DEVICE_TO_DMA);

    return XST_SUCCESS;
}






