//****************************************Copyright (c)***********************************//
//ԭ�Ӹ����߽�ѧƽ̨��www.yuanzige.com
//����֧�֣�www.openedv.com
//�Ա����̣�http://openedv.taobao.com
//��ע΢�Ź���ƽ̨΢�źţ�"����ԭ��"����ѻ�ȡZYNQ & FPGA & STM32 & LINUX���ϡ�
//��Ȩ���У�����ؾ���
//Copyright(C) ����ԭ�� 2018-2028
//All rights reserved
//----------------------------------------------------------------------------------------
// File name:           axi_dma.h
// Last modified Date:  2019/06/30 15:59:46
// Last Version:        V1.0
// Descriptions:        AXI DMA
//----------------------------------------------------------------------------------------
// Created by:          ����ԭ��
// Created date:        2019/06/30 15:59:52
// Version:             V1.0
// Descriptions:        The original version
//
//----------------------------------------------------------------------------------------
//****************************************************************************************//

#include "xaxidma.h"
#include "xparameters.h"
#include "xil_exception.h"
#include "xscugic.h"

#ifndef AXI_DMA_CFG_
#define AXI_DMA_CFG_

int check_data(int length, u8 start_value);
int axi_dma_cfg(void);
void rx_intr_handler(void *callback);
int axi_dma_start(u32 pkt_len);
int dma_setup_intr_system(XScuGic * int_ins_ptr);

#endif /* sccb_EMIO_CFG_ */
