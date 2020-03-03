#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <endian.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

#include "main.h"
#include "net.h"
#include "err.h"

int main(int argc, char **argv) {

	char *req = "GET / HTTP/1.1\r\n" 
	"Accept: text/html\r\n"
	"Host: home.agh.edu.pl\r\n"
	"\r\n";
	
	struct addrinfo *addrlist = resolve("home.agh.edu.pl");
	int sfd = connect2(addrlist);
	
	int res = send(sfd, req, strlen(req), 0);
	check(res, "send");

	FILE *file = fdopen(sfd, "r");
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
