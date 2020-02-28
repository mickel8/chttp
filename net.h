void itoip(in_addr_t, unsigned char *);

struct addrinfo *resolve(char *node);

int connect2(struct addrinfo *addrinfo);
