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

	size_t nmatch = 0;
	regmatch_t *pmatch = NULL;

	nmatch = reg.re_nsub +1;
	pmatch = (regmatch_t*) malloc (sizeof (regmatch_t) * nmatch);


	if(err == 0){
		int match;
		match=regexec(&reg,buffer,nmatch,pmatch,0);

		regfree(&reg);


		if(match==0){
			strcat(message," \n regex valide");


			char *fname = NULL;
			start = pmatch[2].rm_so;
			end = pmatch[2].rm_eo;
			size = end - start;

			fname = (char*)malloc (sizeof(char) * (size + 1));
			if (fname){

				strncpy (fname, &buffer[start], size);
				fname[size] = '\0';
				printf ("<<%s>>\n", fname);

				char* fname2=(char*)malloc(10+sizeof fname);
				fname2[0]='.';
				fname2[1]='\0';

				strcat(fname2,fname);

				fname2[1+sizeof fname2]='\0';

				printf("\n\tpathfichier %s\n\n", fname2);

				

				if(stat(fname2,&fic_stat) == -1){
					ret=strdup("404 Not Found");
					strcat(message,ret);
				}else {
					if(fic_stat.st_mode & S_IRGRP ){
						ret=strdup("200 Ok");
						strcat(message,ret);

						char* content_type;

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
						strcat(message, content_type);
						strcat(message,"<br/>");
						int size_read=fic_stat.st_size;
						int fde=open(fname2,O_RDONLY);
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

				free(fname2);
			}

			free(fname);

		}
		else if (match == REG_NOMATCH){
			strcat(message," \n regex invalide WOIAROZEAEIOZAT");
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

		logInfo* log=creer_log(clientIP,threadID,first,ret,size_fic);
		write_log(log);

		regfree(&reg);
		strcat(message,"</html>");
		free(pmatch);
		
	}
	return message;
}

