#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <assert.h>

int init_socket(int *socket, int port);
int socket_recv(int fd, char *buf, int buf_len);
