# zynq_udp_dma
AXI_DMA_UDP以太网传输
参考博客：https://blog.csdn.net/weixin_43982855/article/details/136379075?utm_medium=distribute.pc_relevant.none-task-blog-2~default~baidujs_baidulandingword~default-0-136379075-blog-136379937.235^v43^control&spm=1001.2101.3001.4242.1&utm_relevant_index=1
bug调试：https://blog.csdn.net/weixin_43982855/article/details/136379937

数据传输流程：
1.PS端下发控制指令，开启DMA传输的同时下发一个上升沿信号（后文将说为什么要加一个上升沿）
2.PL端接收到指令开始产生2048个32bit的数据，通过AXI DMA将数据传输到PS端的DDR3中；
3.PS端将PS端的DDR的数据通过UDP传输给PC，用网口传输助手查看传输的数据。

搭建方式：
 vivado2020.2：
   1.source *:/*******/axi_dma_udp.tcl 
   2.添加 data_gen ip

vitis:
  在BSP中选中lwip211
  添加src中的文件
