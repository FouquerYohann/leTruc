#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/select.h>
#include <unistd.h> /* close */
#include <netdb.h> /* gethostbyname */


#define _XOPEN_SOURCE 700

int main(int argc, char const *argv[])
{
	int port;
	int nb_client;
	int nb_q5;
	char buf[64];
	fd_set rdfs;

	if(argc !=4){
		perror("mauvaise utilisation : port nb_client nb_q5");
		exit (1);
	}

	port=atoi(argv[1]);
	nb_client=atoi(argv[2]);
	nb_q5=atoi(argv[3]);


	int sock= socket(AF_INET, SOCK_STREAM,0);
	struct sockaddr_in sin;
	memset(&sin,0,sizeof sin);

	if(sock < 0){
		perror("socket()");
		exit(errno);
	}

	sin.sin_addr.s_addr = htonl(INADDR_ANY);
	sin.sin_port =htons(port);
	sin.sin_family = AF_INET;

	if(bind(sock,(struct sockadrr *) &sin, sizeof sin) < 0){
		perror("bind()");
		exit(errno);
	}

	if(listen(sock,nb_client) <0 ){
		perror("error()");
		exit(errno);
	}

	while(1){
		if( recv(sock,buf, sizeof(buf),0) <0){
			perror("recv()");
			exit(errno);
		}
		printf("on a bien recu un truc \n");
	}



}
