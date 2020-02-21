#ifndef __SOCK_STREM_H__
#define __SOCK_STREM_H__

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>

int create_ipv6_socket();

int tcp_server_socket_ipv6(int port);
int udp_server_socket_ipv6(int port);
int tcp_accept_ipv6(int fd);

int connect_ipv6_socket(unsigned char *ip, int port);

#endif
