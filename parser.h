#ifndef PARSER_H
#define PARSER_H

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
#include <pthread.h>
#include <regex.h>
#include <sys/types.h>
#include <sys/stat.h>


#include "mime.h"
#include "log.h"

extern mr_mime** mime_tab;
extern int taille_mime_tab;


char* parse_message(char* buffer,struct sockaddr_in csin);

#ifndef STR_REGEX
#define STR_REGEX "(GET \\/([^\\.]+(\\.(.+))?)? HTTP\\/1\\.1)\r\nHost: 127\\.0\\.0\\.1:.*"
#endif
/*STR_REGEX*/

#endif
/*PARSER_H*/