#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <endian.h>

#include "net.h"
#include "err.h"

void itoip(in_addr_t ip, unsigned char *octets) {
	octets[3] = (ip >> 0) & 0xFF;
	octets[2] = (ip >> 8) & 0xFF;
	octets[1] = (ip >> 16) & 0xFF;
	octets[0] = (ip >> 24) & 0xFF;
}

struct addrinfo *resolve(char *node) {
	struct addrinfo hints = {
		.ai_family = AF_INET,	
		.ai_socktype = SOCK_STREAM,
		.ai_protocol = getprotobyname("TCP")->p_proto,
		.ai_flags = 0,
		.ai_addrlen = 0,
		.ai_addr = NULL,
		.ai_canonname = NULL,
		.ai_next = NULL
	};
	struct addrinfo *res;
	int retval = getaddrinfo(node, NULL, &hints, &res);
	check(retval, "getaddrinfo");
	return res;
}

int connect2(struct addrinfo *addrinfo) {
	int sfd = -1;
	struct addrinfo *aip;
	for (aip = addrinfo; aip != NULL; aip = aip->ai_next) {
		sfd = socket(AF_INET, SOCK_STREAM, 0);
		check(sfd, "socket");
		struct sockaddr_in *addr = (struct sockaddr_in *) aip -> ai_addr;
		addr -> sin_port = htobe16(80);
		int res = connect(sfd, (struct sockaddr *) addr, sizeof(*addr));
		if (res == 0) {
			break;
		}
		perror("connect");
		close(sfd);
	}
	return sfd;
}
