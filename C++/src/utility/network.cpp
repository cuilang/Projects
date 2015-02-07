#include <arpa/inet.h>

#include "network.h"

unsigned short ChecksumTcpUdp(unsigned int saddr, unsigned int daddr,
        unsigned short len, unsigned char proto, const unsigned char *data)
{
    const sniff_tcp *tcpHeader = (const sniff_tcp *)data;
    unsigned char protoStr[2] = { 0x00, proto };
    long sum = (saddr & 0xffff) + (saddr >> 16) + (daddr & 0xffff)
        + (daddr >> 16) + len + *(unsigned short *)protoStr;
 
    int count = ntohs(len);
 
    while (count > 1) {
        sum += *(unsigned short *)data;
        data += 2;
        count -= 2;
    }
    if (count > 0) {
        sum += *(unsigned char *)data;
    }
 
    sum -= tcpHeader->th_sum;
 
    while (sum >> 16) {
        sum = (sum & 0xffff) + (sum >> 16);
    }
 
    return ~sum;
}
 
 
unsigned short Csum(unsigned char *addr, int count)
{
    long sum = 0;
    while (count > 1) {
        sum += *(unsigned short *)addr;
        addr += 2;
        count -= 2;
    }
    if (count > 0) {
        sum += *(unsigned char *)addr;
    }
    while (sum >> 16) {
        sum = (sum & 0xffff) + (sum >> 16);
    }
    return ~sum;
}

