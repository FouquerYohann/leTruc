#include "thread.h"

void* routine(void* arg){
	reponse_thread* info=(reponse_thread*)arg;
	int csock=info->sock;
	char message[TAILLE_BUFFER];
	int send_ret;

	strcpy(message,parse_message(info->buffer, info->csin)); 
	send_ret=write(csock,message,strlen(message));
	if(send_ret <0){
		perror("send()");
		close(csock);
		exit(errno);
	}
	printf("MESSAGE : %s\n",message );
	printf("VALEUR SEND : %d\n PID : %d\n", send_ret,pthread_self());


	pthread_mutex_lock(info->mux);
	info->tab[info->num]=0;
	pthread_mutex_unlock(info->mux);
	return NULL;
}



void* setmmap_thread(int taille){
	int fd;
    void *data;

    if ((fd = shm_open(NAME_MMAP,O_CREAT | O_RDWR,0600)) == -1) {
        perror("shm_open()");
        exit(1);
    }
    
	if (ftruncate(fd, sizeof(int)*taille) == -1) {
			perror("ftruncate");
			exit(1);}


    data = mmap((caddr_t)0, (sizeof (int))*taille, PROT_READ| PROT_WRITE, MAP_SHARED, fd, 0);
                                                           
    if (data == (caddr_t)(-1)) {
        perror("mmap()");
        exit(1);
    }
    return data;
}



