#include <stdio.h>
#include <string.h>
#include "lwip/err.h"
#include "lwip/udp.h"
#include "xil_printf.h"
#include "lwip/inet.h"

static struct udp_pcb *pcb;

#define SER_PORT 8000

//��ӡӦ�ó���
void print_app_header()
{
    xil_printf("\r\n-----Network port UDP transmission camera video display on upper computer ------\r\n");
}

//UDP���͹��ܺ���
void udp_tx_data(u8 *buffer_ptr,unsigned int len){
    static struct pbuf *ptr;

    ptr = pbuf_alloc(PBUF_TRANSPORT, len, PBUF_POOL); /* �����ڴ� */

    if (ptr)
    {
        pbuf_take(ptr, buffer_ptr,1024); /* ��buffer_ptr�е����ݴ����pbuf�ṹ�� */
        udp_send(pcb, ptr);              /* udp�������� */
        pbuf_free(ptr);                  /* �ͷ��ڴ� */
    }
}

void start_application()
{
    err_t err;

    /* ���ÿͻ��˵�IP��ַ */

    ip_addr_t DestIPaddr;

    IP4_ADDR( &DestIPaddr,192,168,1,102);

    //�����µ�UDP PCB
    pcb = udp_new();
    if (!pcb) {
        xil_printf("Error creating PCB. Out of Memory\r\n");
        return;
    }

    //�󶨶˿�
    err = udp_bind(pcb, IP_ADDR_ANY, SER_PORT);
    if (err != ERR_OK) {
        xil_printf("Unable to bind to port %d; err %d\r\n",
                SER_PORT, err);
        udp_remove(pcb);
        return;
    }
    /* ���ÿͻ��˵Ķ˿� */
    udp_connect(pcb, &DestIPaddr, 8000);

    xil_printf("UDP server started @ port %d\n\r", SER_PORT);
}
