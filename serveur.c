#include "serveur.h"

int dispo(int* tab,int taille){
	int i;
	for (i = 0; i < taille; ++i){
		if(tab[i]) return i;
	}
	return -1;
}

mr_mime** mime_tab;
int taille_mime_tab;


int main(int argc, char const *argv[]){

	int port;
	int nb_client;
	int nb_q5;
	int csock;
	int i;
	int sock,recv_ret;
	int client=0;
	fd_set rdfs;
	socklen_t length;
	pthread_t* tab_thread;
	char buf[TAILLE_BUFFER+TAILLE_BUFFER];
	reponse_thread info;

	int* tab_acces;

	if(argc !=4){
		perror("mauvaise utilisation : port nb_client nb_q5");
		exit (1);
	}

	printf("INITIALIZATIOOOOOON \n");
	printf("Death to ze romans\n");
	if(creerTab(&mime_tab,&taille_mime_tab) <0){
		perror("tab mime");
		exit(-500);
	}
	printf("mr_mime INITIALIZED \n");


	port=atoi(argv[1]);
	nb_client=atoi(argv[2]);
	nb_q5=atoi(argv[3]);

	tab_thread=(pthread_t*)malloc(sizeof(pthread_t)*nb_client);

	tab_acces =(int*)setmmap_thread(nb_client);

	int sock_client[nb_client];
	
	for(i=0;i<nb_client;i++){
		tab_acces[i]=1;
	}

	for(i=0;i<nb_client;i++){
		sock_client[i] = port+i;
	}

	sock=init_sock(nb_client,port);

	while(1){

		FD_ZERO(&rdfs);
		FD_SET(STDIN_FILENO, &rdfs);
		FD_SET(sock, &rdfs);


		for(i=0;i<nb_client;i++){
			FD_SET(sock_client[i],&rdfs);
		}

		

		if(select(sock+1,&rdfs,NULL,NULL,NULL) ==-1){
			perror("select()");
			exit(errno);
		}


		if(FD_ISSET(sock, &rdfs)){
			struct sockaddr_in csin;
			memset(&csin,0,sizeof csin);
			length=sizeof csin;
			csock=accept(sock,(struct sockaddr*)&csin,&length);

			if(csock <0){
				perror("accept()");
				close(sock);
				exit(errno);
			}


			recv_ret=recv(csock,buf,sizeof buf,0);
			if(recv_ret <0){
				perror("recv()");
				close(sock);
				exit(errno);
			}

			printf("message : {\n\t" );
			for (i = 0; i < recv_ret; ++i){
				putchar(buf[i]);
				if(buf[i]=='\n')putchar('\t');
			}
			printf("} %d fin\n",recv_ret);

			info.sock 		= csock;
			info.buffer 	= buf;
			info.mux 		= &mu_tex;
			info.tab  		= tab_acces;
			info.csin		= csin;
			pthread_mutex_lock(&mu_tex);
			client=dispo(tab_acces,nb_client);
			pthread_mutex_unlock(&mu_tex);


			if(client==-1)continue;

			info.num		=client;

			pthread_mutex_lock(&mu_tex);
			tab_acces[client]=0;
			pthread_mutex_unlock(&mu_tex);


			if(pthread_create(&(tab_thread[client]),NULL,routine,&info)){
				perror("pthread_create()");
				close(sock);
				exit(errno);
			}
			
		}

	}
	free(tab_thread);
	close(csock);
	close(sock);
	return 0;
}

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


