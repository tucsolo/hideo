#ifndef _LIBHTTPD_H_
#define _LIBHTTPD_H_

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/param.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int conndf_rv;

#define DATLEN 4096
#define MAXREQ	8192
#define METHOD_UNKNOWN 0
#define METHOD_GET 1
#define METHOD_HEAD 2
#define METHOD_POST 3


struct httpread {
	int dimArray;
	char **array;
};

/***************************************************************************************
	Structure which memorizes http messages, file descriptors, path files.
	It is created after accepting a connection and destroyed after closing the connection
****************************************************************************************/
struct conndata {
	char http_req[MAXREQ];
	int process_id;		//tid del thread
	int socketint;		//socket file descriptor
	char path[512];		//request path
	char method[5];		//request method
	char useragent[512];	//user agent
	char acceptfld[512];	//accept field
	char messages[3000];	//
	int msgtype;		//
	int get1head2;
	char *extension;
	char imgext[8];		//image extension
	float quality;		//image quality
	char ext[10];
	int quality_factor;
	char options[3000];
	int return_code;	//Return code
	char return_path[512];	//Return path
};

/*http messages*/
void http_200(struct conndata *);
void http_500(struct conndata *);
void http_404(struct conndata *);
void http_501(struct conndata *);
void http_connection_close(struct conndata *);

int find_quality(char *);
char *get_ext(char *);
char *get_mimetype(char *);

struct conndata *create_conndata(void);
void init_conndata(struct conndata *);

void print_message(struct conndata *);
int uacheck(char *, struct conndata *);
int accheck(char *, struct conndata *);
int connection_close(char *, struct conndata *);
int method_parse(char *, struct conndata *);
int path_parse(char *optstring, struct conndata *);
int client_request(struct conndata *);
int send_response(struct conndata *);
int serve_request(struct conndata *);

void *create_httpread();
void destroy_httpread(struct httpread *httpr);
char *read_string(int fd);
struct httpread *read_request(int fd);


#endif				/* _LIBHTTPD_H_ */
