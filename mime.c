#include "mime.h"



mr_mime* creer_mr_mime(char* name ,char* ext){
  mr_mime* m=(mr_mime*)malloc(sizeof (mr_mime));
  m->nom=strdup(name);
  m->extension=strdup(ext);
  return m;
}


int read_line (int fd, char *ret) {
  char val;
  int i = 0;

  if (read (fd, &val, 1) == 0) {
    ret[0] = '\0';
    return 0;
  }

  while ( val!= '\n' && val != '\0' )  {
    ret [i] = val;
    i++;
    read (fd, &val, 1);
  }

  ret [i] = '\0';
  return 1;
}

int creerTab(mr_mime*** tab,int* taille){

  int i;
  mr_mime** mr_mim_tab=(mr_mime**) malloc(1500*sizeof(mr_mime*));

  for(i=0;i<1500;i++)
    mr_mim_tab[i]=NULL;

  char * regexString ="(\\S+)\\s*(.+)*";
  regex_t regexCompiled;
  if (regcomp(&regexCompiled, regexString, REG_EXTENDED))
  {
    printf("Could not compile regular expression.\n");
    return -1;
  }

  int fd=open("/etc/mime.types",O_RDONLY);
  if(fd<0){
    perror("open()");
    return -2;
  }

  char buffer[128];
  int count=0;
  while(read_line(fd,buffer)){
    
    if(buffer[0] == '#' || buffer[0]=='\n' || buffer[0]=='\0')
      continue;


    size_t maxGroups = 0;
    regmatch_t *groupArray = NULL;

    maxGroups = regexCompiled.re_nsub +1;
    groupArray = (regmatch_t*) malloc (sizeof (regmatch_t) * maxGroups);

    int match=regexec(&regexCompiled, buffer, maxGroups, groupArray, 0);

    if (match == 0)
    {
      if(maxGroups ==1){
        continue;
      }

      unsigned int start_0=groupArray[1].rm_so;
      unsigned int end_0  =groupArray[1].rm_eo;
      unsigned int size_0 =end_0-start_0;
      unsigned int start_1=groupArray[2].rm_so;
      unsigned int end_1  =groupArray[2].rm_eo;
      unsigned int size_1 =end_1-start_1;

      char* group0=(char*)malloc((size_0 +1)*sizeof(char));
      char* group1=(char*)malloc((size_1 +1)*sizeof(char));

      strncpy(group0,&buffer[start_0],size_0);
      group0[size_0]='\0';
      strncpy(group1,&buffer[start_1],size_1);
      group1[size_1]='\0';
      
      const char s[2] = " ";
      if(group1[0]!='\0'){
        char* tmp=strtok(group1,s);
        while(tmp !=NULL){

          mr_mim_tab[count++]=creer_mr_mime(group0,tmp);

          tmp=strtok(NULL,s);
        }
      }

    }else if ( match== REG_NOMATCH){
      printf("pas de match\n");
       return -3;
    }else{
      printf("pas de regexec\n");
       return -4;
    }

  }

  mr_mime** mr_mim_ret=(mr_mime**) malloc(count*sizeof(mr_mime*));
  for ( i = 0; i < count; ++i)
  {
    mr_mim_ret[i]=mr_mim_tab[i];
  }
  free(mr_mim_tab);
  *tab=mr_mim_ret;
  *taille=count;
  return 0;


}
