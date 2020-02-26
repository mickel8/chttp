#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <endian.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

#include "main.h"

#define check(RES, MSG) if (RES < 0) perror(MSG) 

int main(int argc, char **argv) {

	int fd = socket(AF_INET, SOCK_STREAM, 0);
	check(fd, "socket");

	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htobe16(80);
	int res = inet_aton("149.156.98.66", &addr.sin_addr);
	check(res, "inet_aton");

	res = connect(fd, (struct sockaddr *) &addr, sizeof(addr));
	check(res, "connect");

	char *req = "GET / HTTP/1.1\r\n" 
	"Accept: text/html\r\n"
	"Host: home.agh.edu.pl\r\n"
	"\r\n";
	
	res = send(fd, req, strlen(req), 0);
	check(res, "send");

	FILE *file = fdopen(fd, "r");
	char *line = NULL;
	
	const char *cnt_len_hdr = "Content-Length";
	size_t n = strlen(cnt_len_hdr);
	size_t n2 = 0;
	
	FILE *res_file = fopen("response", "w");
	while(getline(&line, &n2, file) != -1) {
		if (strncasecmp(line, cnt_len_hdr, n) == 0) {
			printf("Found content-length: %s\n", line);
		}
		fputs(line, res_file);
	}
	fclose(file);
	fclose(res_file);
	return 0;
}
