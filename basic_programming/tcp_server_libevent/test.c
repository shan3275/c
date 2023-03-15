#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>
#include <unistd.h>
#include <event.h>

// debug 宏，打印可视化年月日时分秒毫秒，函数，文件，行号
#define debug(fmt, ...) \
    do { \
        struct timeval tv; \
        struct tm *tm; \
        gettimeofday(&tv, NULL); \
        tm = localtime(&tv.tv_sec); \
        printf("%04d-%02d-%02d %02d:%02d:%02d.%03d %s:%s:%d " fmt, \
                tm->tm_year + 1900, tm->tm_mon + 1, tm->tm_mday, \
                tm->tm_hour, tm->tm_min, tm->tm_sec, (int)tv.tv_usec / 1000, \
                __FUNCTION__, __FILE__, __LINE__, ##__VA_ARGS__); \
    } while (0)

void on_accept(int, short, void *);
void on_read(int, short, void *);

void print_events(struct event_base* base) {
    int n_events = event_base_get_num_events(base, EVENT_BASE_COUNT_ADDED);
    printf("Total events: %d\n", n_events);
}

int main(int argc, char **argv)
{
    struct event_base *base;
    struct event *listener_event;
    struct sockaddr_in sin;
    int *listener_fd;
    int reuseaddr_on = 1;
    int i, num_ports = 2;
    int ports[] = {5555, 6666};
    const char *version = event_get_version();
    debug("Libevent version: %s\n", version);
    base = event_base_new();

    if (!base) {
        debug("Error creating event_base.\n");
        return 1;
    }

    listener_fd = calloc(num_ports, sizeof(int));
    memset(&sin, 0, sizeof(sin));
    sin.sin_family = AF_INET;
    sin.sin_addr.s_addr = htonl(INADDR_ANY);

    for (i = 0; i < num_ports; i++) {
        sin.sin_port = htons(ports[i]);

        listener_fd[i] = socket(AF_INET, SOCK_STREAM, 0);

        if (listener_fd[i] < 0) {
            debug("Error creating listening socket.\n");
            return 1;
        }

        setsockopt(listener_fd[i], SOL_SOCKET, SO_REUSEADDR, &reuseaddr_on, sizeof(reuseaddr_on));

        if (bind(listener_fd[i], (struct sockaddr *)&sin, sizeof(sin)) < 0) {
            debug("Error binding to port %d.\n", ports[i]);
            return 1;
        }

        if (listen(listener_fd[i], 16) < 0) {
            debug("Error listening on port %d.\n", ports[i]);
            return 1;
        }

        listener_event = event_new(base, listener_fd[i], EV_READ|EV_PERSIST, on_accept, (void *)base);
        event_add(listener_event, NULL);
    }
    debug("server started\n");
    print_events(base);
    event_base_dispatch(base);

    return 0;
}

void print_address(struct sockaddr_in address) {
    char ip[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &(address.sin_addr), ip, INET_ADDRSTRLEN);
    int port = ntohs(address.sin_port);
    debug("IP address: %s\n", ip);
    debug("Port: %d\n", port);
}

void on_accept(int fd, short ev, void *arg)
{
    struct event_base *base = (struct event_base *)arg;
    struct event *ev_accept;
    struct sockaddr_in sin;
    socklen_t slen = sizeof(sin);
    int client_fd;
    debug("on_accept\n");
    client_fd = accept(fd, (struct sockaddr *)&sin, &slen);
    //debug("client_fd = %d\n", client_fd);

    if (client_fd < 0) {
        debug("Error accepting connection.\n");
        return;
    }
    print_address(sin);
    debug("event_new\n");
    ev_accept = event_new(base, client_fd, EV_READ|EV_PERSIST, on_read, event_self_cbarg());
    event_add(ev_accept, NULL);
    print_events(base);
    debug("event: %p\n", ev_accept);
}


void on_read(int fd, short ev, void *arg)
{
    char buf[1024];
    int nread;
    //debug("event: %p\n", arg);
    //debug("on_read\n");
    nread = recv(fd, buf, sizeof(buf), 0);
    //debug("nread = %d\n", nread);
    if (nread < 0) {
        debug("Error reading from socket.\n");
        return;
    }

    if (nread == 0) {
        event_del((struct event *)arg);
        close(fd);
        debug("Connection closed.\n");
        return;
    }

    //debug("Received %d bytes\n", nread);
}
