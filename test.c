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

int init_sock(int nb_client,int port){
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

	if(bind(sock,(struct sockaddr *) &sin, sizeof sin) < 0){
		perror("bind()");
		exit(errno);
	}

	if(listen(sock,nb_client) <0 ){
		perror("error()");
		exit(errno);
	}
	return sock;
}

#ifndef TAILLE_BUFFER
#define TAILLE_BUFFER 255
#endif

int main(void){
	int port=4500;
	int nb_client=1;
	int sock=init_sock(nb_client,port);
	int csock;
	int recv_ret,send_ret;
	int i;
	socklen_t length;
	char message[TAILLE_BUFFER];
	char buffer[TAILLE_BUFFER];

	struct sockaddr_in csin;
	memset(&csin,0,sizeof csin);
	length=sizeof csin;
	csock=accept(sock,(struct sockaddr*)&csin,&length);

	if(csock<0){
		perror("accept()");
		exit(errno);
	}

	/*RECEPTION*/
	recv_ret=recv(csock,buffer,TAILLE_BUFFER*sizeof(char),0);
	
	if(recv_ret<0){
		perror("recv()");
		exit(errno);
	}

	/*AFFICHAGE DU RETOUR DE RECV*/
	printf("message : {\n\t" );
	for (i = 0; i < recv_ret; ++i){
		putchar(buffer[i]);
		if(buffer[i]=='\n')putchar('\t');
	}
	printf("\n} fin\n");

	/*ENVOI*/
	strcpy(message,"<html><header><title>JE SUIS UN TITRE</title></header><body>je suis un message</body></html>"); 
	send_ret=write(csock,message,strlen(message));
	if( send_ret < 0){
		perror("send()");
		exit(errno);
	}
	printf("VALEUR SEND : %d\n", send_ret);

	return 0;
}