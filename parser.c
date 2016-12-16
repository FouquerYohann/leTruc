#include "parser.h"

char* parse_message(char* buffer, struct sockaddr_in csin){
	int i;
	int start; 
	int end; 
	size_t size; 
	char* ret;
	struct stat fic_stat;



	char* message=(char*)malloc(512*sizeof(char));
	char* cp="<html>HTTP/1.1 ";
	strcpy(message,cp); 


	regex_t reg;
	int err= regcomp(&reg, STR_REGEX, REG_EXTENDED);
	if(err!=0){
		perror("regcomp()");
		exit(errno);
	}



	size_t nmatch = 0;
	regmatch_t *pmatch = NULL;

	nmatch = reg.re_nsub +1;
	pmatch = (regmatch_t*) malloc (sizeof (regmatch_t) * nmatch);


	
	int match;
	match=regexec(&reg,buffer,nmatch,pmatch,0);
	if(match == REG_NOMATCH){
		perror("no match from regex");
		exit(errno);
	}

	regfree(&reg);

	if(pmatch[2].rm_so !=-1){
		char *fname = NULL;
		start = pmatch[2].rm_so;
		end = pmatch[2].rm_eo;
		size = end - start;

		fname = (char*)malloc (sizeof(char) * (size + 1));
		if (fname ==NULL){
			perror("malloc()");
			exit(errno);
		}

		strncpy (fname, &buffer[start], size);
		fname[size] = '\0';


		if(stat(fname,&fic_stat) == -1){
			ret=strdup("404 Not Found");
			strcat(message,ret);
		}else {
			if(fic_stat.st_mode & S_IRGRP ){
				ret=strdup("200 Ok");
				strcat(message,ret);

				char* content_type=NULL;

				if(pmatch[4].rm_so >0){
					start = pmatch[4].rm_so;
					end = pmatch[4].rm_eo;
					size = end - start;

					char* fext=(char*)malloc (sizeof(char) * (size + 1));
					strncpy (fext, &buffer[start], size);
					fext[size]='\0';
					printf(" extension %s\n", fext);

					for(i=0;i<taille_mime_tab;i++){
						if(strcmp(fext,mime_tab[i]->extension) == 0){
							content_type=strdup(mime_tab[i]->nom);
							break;
						}
					}
					free(fext);
				}

				strcat(message,"<br/> Contente Type : ");
				strcat(message, (content_type!=NULL)?content_type:" inconnu");
				strcat(message,"<br/>");
				int size_read=fic_stat.st_size;
				int fde=open(fname,O_RDONLY);
				if(fde<0){
					perror("open()");
					exit(errno);
				}
				char* read_f=(char*)malloc(size_read*sizeof(char));
				if(read(fde,read_f,size_read)<0){
					perror("read()");
					exit(errno);
				}
				strcat(message,read_f);
			}else{
				ret=strdup("403 Forbidden");
				strcat(message,ret);
			}

		}

		free(fname);
	}
	if(pmatch[1].rm_so <0){
		printf("\n\nerror pmatch[1]\n\n\n");
		perror("pmatch");
		exit(errno);
	}
	start = pmatch[1].rm_so;
	end = pmatch[1].rm_eo;
	size = end - start;

	char* clientIP= strdup(inet_ntoa(csin.sin_addr));
	int thID=pthread_self();
	char* threadID=(char*)malloc (32*sizeof(char));;
	sprintf(threadID,"%d",thID);
	char* first = (char*)malloc (sizeof(char) * (size + 1));
	strncpy(first,&buffer[start], size);
	first[size]='\0';
	char* size_fic = (char*)malloc (32*sizeof(char));
	sprintf(size_fic,"%zu octets",fic_stat.st_size);

	ret=(ret==NULL)?"no file required":ret;

	logInfo* log=creer_log(clientIP,threadID,first,ret,size_fic);
	write_log(log);

	regfree(&reg);
	strcat(message,"</html>");
	free(pmatch);

	
	return message;
}

