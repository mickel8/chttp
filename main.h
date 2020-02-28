#define HTTP_P 80

struct msg *deserialize(char *);
char *serialize(struct msg);
int send_request(struct msg);
struct msg receive_response(void);

enum Method {
	GET,
	HEAD,
	POST,
	PUT,
	DELETE,
	TRACE,
	CONNECT
};

struct req_line {
	enum Method method;
	char *request_target;
	float http_version;
};

struct stat_line {
	float http_version;
	int status_code;
	char *reason_phrase; // char * ?
};

struct start_line {
	struct req_line requestLine;
	struct stat_line statusLine;
};

struct msg {
	struct start_line start_line;
	char **header_fields;
	char *message_body;
};
