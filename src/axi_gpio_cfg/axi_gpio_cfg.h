//****************************************Copyright (c)***********************************//
//原子哥在线教学平台：www.yuanzige.com
//技术支持：www.openedv.com
//淘宝店铺：http://openedv.taobao.com
//关注微信公众平台微信号："正点原子"，免费获取ZYNQ & FPGA & STM32 & LINUX资料。
//版权所有，盗版必究。
//Copyright(C) 正点原子 2018-2028
//All rights reserved
//----------------------------------------------------------------------------------------
// File name:           emio_sccb_cfg.h
// Last modified Date:  2019/06/30 15:59:46
// Last Version:        V1.0
// Descriptions:        SCCB驱动
//----------------------------------------------------------------------------------------
// Created by:          正点原子
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
