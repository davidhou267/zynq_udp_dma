#include <stdio.h>
#include <string.h>
#include "lwip/err.h"
#include "lwip/udp.h"
#include "xil_printf.h"
#include "lwip/inet.h"

static struct udp_pcb *pcb;

#define SER_PORT 8000

//打印应用程序
void print_app_header()
{
    xil_printf("\r\n-----Network port UDP transmission camera video display on upper computer ------\r\n");
}

//UDP发送功能函数
void udp_tx_data(u8 *buffer_ptr,unsigned int len){
    static struct pbuf *ptr;

    ptr = pbuf_alloc(PBUF_TRANSPORT, len, PBUF_POOL); /* 申请内存 */

    if (ptr)
    {
        pbuf_take(ptr, buffer_ptr,1024); /* 将buffer_ptr中的数据打包进pbuf结构中 */
        udp_send(pcb, ptr);              /* udp发送数据 */
        pbuf_free(ptr);                  /* 释放内存 */
    }
}

void start_application()
{
    err_t err;

    /* 设置客户端的IP地址 */

    ip_addr_t DestIPaddr;

    IP4_ADDR( &DestIPaddr,192,168,1,102);

    //创建新的UDP PCB
    pcb = udp_new();
    if (!pcb) {
        xil_printf("Error creating PCB. Out of Memory\r\n");
        return;
    }

    //绑定端口
    err = udp_bind(pcb, IP_ADDR_ANY, SER_PORT);
    if (err != ERR_OK) {
        xil_printf("Unable to bind to port %d; err %d\r\n",
                SER_PORT, err);
        udp_remove(pcb);
        return;
    }
    /* 设置客户端的端口 */
    udp_connect(pcb, &DestIPaddr, 8000);

    xil_printf("UDP server started @ port %d\n\r", SER_PORT);
}
