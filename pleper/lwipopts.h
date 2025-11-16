#ifndef __LWIPOPTS_H__
#define __LWIPOPTS_H__

// Configuración mínima necesaria para Pico W

#define NO_SYS                      1
#define LWIP_SOCKET                 0
#define LWIP_NETCONN                0
#define MEM_ALIGNMENT               4
#define MEM_SIZE                    4000
#define MEMP_NUM_PBUF               10
#define MEMP_NUM_TCP_PCB            5
#define MEMP_NUM_TCP_SEG            16
#define MEMP_NUM_SYS_TIMEOUT        10

#define TCP_MSS                     1460
#define TCP_SND_BUF                 (4 * TCP_MSS)
#define TCP_WND                     (2 * TCP_MSS)

#define LWIP_TCP_KEEPALIVE          1
#define LWIP_DHCP                   1
#define LWIP_ICMP                   1
#define LWIP_IPV4                   1
#define LWIP_IPV6                   0

#define LWIP_DEBUG                  0

#endif /* __LWIPOPTS_H__ */
