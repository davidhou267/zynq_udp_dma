//****************************************Copyright (c)***********************************//
//ԭ�Ӹ����߽�ѧƽ̨��www.yuanzige.com
//����֧�֣�www.openedv.com
//�Ա����̣�http://openedv.taobao.com
//��ע΢�Ź���ƽ̨΢�źţ�"����ԭ��"����ѻ�ȡZYNQ & FPGA & STM32 & LINUX���ϡ�
//��Ȩ���У�����ؾ���
//Copyright(C) ����ԭ�� 2018-2028
//All rights reserved
//----------------------------------------------------------------------------------------
// File name:           emio_sccb_cfg.h
// Last modified Date:  2019/06/30 15:59:46
// Last Version:        V1.0
// Descriptions:        SCCB����
//----------------------------------------------------------------------------------------
// Created by:          ����ԭ��
// Created date:        2019/06/30 15:59:52
// Version:             V1.0
// Descriptions:        The original version
//
//----------------------------------------------------------------------------------------
//****************************************************************************************//

#include "xil_types.h"
#include "xgpio.h"

#ifndef AXI_GPIO_CFG_
#define AXI_GPIO_CFG_


void axi_gpio_init(void);
//u32 get_fifo_count(void);
void axi_gpio_out0(void);
void axi_gpio_out1(void);

#endif /* sccb_EMIO_CFG_ */
