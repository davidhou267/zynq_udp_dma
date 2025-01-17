//****************************************Copyright (c)***********************************//
//ԭ�Ӹ����߽�ѧƽ̨��www.yuanzige.com
//����֧�֣�www.openedv.com
//�Ա����̣�http://openedv.taobao.com
//��ע΢�Ź���ƽ̨΢�źţ�"����ԭ��"����ѻ�ȡZYNQ & FPGA & STM32 & LINUX���ϡ�
//��Ȩ���У�����ؾ���
//Copyright(C) ����ԭ�� 2018-2028
//All rights reserved
//----------------------------------------------------------------------------------------
// File name:           axi_gpio_cfg.c
// Last modified Date:  2020/04/15 10:59:46
// Last Version:        V1.0
// Descriptions:        AXI GPIO����
//----------------------------------------------------------------------------------------
// Created by:          ����ԭ��
// Created date:        2023/06/30 15:59:52
// Version:             V1.0
// Descriptions:        The original version
//
//----------------------------------------------------------------------------------------
//****************************************************************************************//

#include"axi_gpio_cfg.h"

#define AXI_GPIO_0_ID XPAR_AXI_GPIO_0_DEVICE_ID	//AXI GPIO 0���� ID

#define AXI_GPIO_0_CHANEL1   1

XGpio   axi_gpio_inst0;      //AXI GPIO 0 ����ʵ��

//AXI GPIO��ʼ��
void axi_gpio_init(void)
{
    //AXI GPIO 0����
    XGpio_Initialize(&axi_gpio_inst0, AXI_GPIO_0_ID);
    //����AXI GPIO 0 ͨ��1Ϊ����
    XGpio_SetDataDirection(&axi_gpio_inst0, AXI_GPIO_0_CHANEL1,1);
}

//ͨ��AXI GPIO��ȡFIFO���ݸ���
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

