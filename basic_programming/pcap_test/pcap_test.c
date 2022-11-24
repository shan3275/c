#include <pcap.h>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

#define THREAD_NUM 2
typedef struct
{
    pcap_t *fp;
    struct bpf_program filter;
    char packet_filter[128];
    pthread_t tid;
} libpcap_param_t;

void libpcap_packet_handler(u_char *arg, const struct pcap_pkthdr *pkthdr, const u_char *packet)
{
    int *id = (int *)arg;
    printf("ptd:%lu, id: %d, pkt_len:%d, caplen:%d\n", pthread_self(), ++(*id), pkthdr->len, pkthdr->caplen);
    // printf("Packet length: %d\n", pkthdr->len);
    // printf("Number of bytes: %d\n", pkthdr->caplen);
    // printf("Recieved time: %s", ctime((const time_t *)&pkthdr->ts.tv_sec));
    /*
        int i;
        for(i=0; i<pkthdr->len; ++i)
        {
            printf(" %02x", packet[i]);
            if( (i + 1) % 16 == 0 )
            {
                printf("\n");
            }
        }

        printf("\n\n");
    */
}

void *pcap_rx_loop(void *_param)
{
    libpcap_param_t *rparam = (libpcap_param_t *)_param;


    if (pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL) != 0)
    {
        perror("pthread_setcancelstate err:");
        return NULL;
    }
    if (pthread_setcanceltype(PTHREAD_CANCEL_DEFERRED, NULL) != 0)
    {
        perror("pthread_setcanceltype err:");
        return NULL;
    }

    /* wait loop forever */
    int id = 0;
    pcap_loop(rparam->fp, 0, (pcap_handler)libpcap_packet_handler, (u_char *)&id);
    printf("pcap loop after %lu\n", pthread_self());
    return NULL;
}

int main(int argc, char *argv[])
{
    int i;
    libpcap_param_t param[THREAD_NUM];
    int rv;
    char *devStr;
    char errBuf[PCAP_ERRBUF_SIZE];
    if (argc == 1)
    {
        printf("must have a parameter, such as: eth0\n");
        exit(1);
    }
    devStr = strdup(argv[1]);
    ///* get a device */
    // devStr = pcap_lookupdev(errBuf);
    // if(devStr)
    //{
    //     printf("success: device: %s\n", devStr);
    // }
    // else
    //{
    //     printf("error: %s\n", errBuf);
    //     exit(1);
    // }

    /* open a device, wait until a packet arrives */
    pcap_t *device = NULL;

    for (i = 0; i < THREAD_NUM; i++)
    {
        device = pcap_open_live(devStr, 65535, 1 | 8 | 16, 0, errBuf);
        if (!device)
        {
            printf("error: pcap_open_live(): %s\n", errBuf);
            exit(1);
        }
        param[i].fp = device;

        memset(param[i].packet_filter, 0, 128);
        // snprintf(packet_filter, 128, "%s", "dst port 80");
        pcap_compile(param[i].fp, &param[i].filter, param[i].packet_filter, 1, 0);
        pcap_setfilter(param[i].fp, &param[i].filter);
        rv = pthread_create(&param[i].tid, NULL, pcap_rx_loop, (void *)&param[i]);
        if (rv)
        {
            printf("Failed Create Thread for interface-%s\n", devStr);
        }
        else
        {
            pthread_detach(param[i].tid);
            printf("Success Create Thread-%ld for interface-%s\n", param[i].tid, devStr);
        }
    }

    while (1)
    {
        sleep(100);
    }

    return 0;
}
