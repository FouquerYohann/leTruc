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
	int sock_client[5];
	fd_set rdfs;
	int i=0;

	if(argc !=4){
		perror("mauvaise utilisation : port nb_client nb_q5");
		exit (1);
	}


	port=atoi(argv[1]);
	nb_client=atoi(argv[2]);
	nb_q5=atoi(argv[3]);

	for(i=0;i<5;i++){
      	sock_client[i]=port+i;
      }

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

	  int i = 0;
      FD_ZERO(&rdfs);
      FD_SET(STDIN_FILENO, &rdfs);
      FD_SET(sock, &rdfs);

      
      
      for(i=0;i<5;i++){
      	FD_SET(sock_client[i],&rdfs);
      }
     
      

      if(select(sock+1,&rdfs,NULL,NULL,NULL) ==-1){
      	perror("select()");
      	exit(errno);
      }
      

      if(FD_ISSET(sock, &rdfs)){
      	struct sockaddr_in csin;
      	memset(&csin,0,sizeof csin);
      	socklen_t length=sizeof csin;
      	
      	int csock=accept(sock,(struct sockaddr*)&csin,&length);

      	if(csock <0){
      		perror("accept()");
      		exit(errno);
      	}

      	if(recv(csock,buf,sizeof buf,0) <0){
      		perror("recv()");
      		exit(errno);
      	}
      	char* message="coucou tu veux voir ma bite"; 
      	if(send(csock,message,strlen(message),0) <0){
      		perror("send()");
      		exit(errno);
      	}
      }

	}



}
