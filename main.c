#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#define BIG (1e30)
#define HOLDEM

double ev();
void pivot(),procp(),descs(),descc(),descv(),lpad(),rpad(),spc(),prp();
int nr,pr,pl,lim,liv;
double r,an,bl;
char temp0[100],temp1[100];
#ifdef HOLDEM
double wa[169][169],prob[169][169];
char hs[169][4];
char*cd="xx23456789TJQKA";
#endif

int main(int ac,char **av){
  int c0,c1,d0,d1,d,m,n,blf,d0f,d1f;
  int t0,i,j,rr,seed,list;
  double EP1,EP2,EP3,ep1,ep2,ep3,Ep1,Ep2,Ep3,EP4,x,y;
  char *l0,as;
  FILE *fp;

  seed=t0=time(NULL);r=1;pl=0;nr=1;pr=6;an=.5;lim=1;liv=1;blf=d0f=d1f=0;d=169;list=0;
  EP1=1e-10;EP2=5e-10;EP3=0;EP4=0;


  for(i=1;i<ac;i++){
    l0=strchr(av[i],'=');
printf("%s\n",l0+1 );
  // if(l0){
   rr=l0-av[i]+1;j=atoi(l0+1);x=atof(l0+1);
  //  if(!strncmp(av[i],"an=",rr))an=x;
  //  if(!strncmp(av[i],"bl=",rr)){bl=x;blf=1;}
  //  if(!strncmp(av[i],"lim=",rr))lim=(j!=0);
  //  if(!strncmp(av[i],"liv=",rr))liv=j;
  //  if(!strncmp(av[i],"list=",rr))list=j;
  //  if(!strncmp(av[i],"pr=",rr))pr=j;
  //  if(!strncmp(av[i],"r=",rr))r=x;
  //  if(!strncmp(av[i],"nr=",rr))nr=j;
  //  if(!strncmp(av[i],"d=",rr))d=j;
  //  if(!strncmp(av[i],"d0=",rr)){d0=j;d0f=1;}
  //  if(!strncmp(av[i],"d1=",rr)){d1=j;d1f=1;}
  //  if(!strncmp(av[i],"pl=",rr))pl=j;
  //  if(!strncmp(av[i],"EP=",rr))EP1=EP2=EP3=x;
  //  if(!strncmp(av[i],"EP1=",rr))EP1=x;
  //  if(!strncmp(av[i],"EP2=",rr))EP2=x;
  //  if(!strncmp(av[i],"EP3=",rr))EP3=x;
  //  if(!strncmp(av[i],"EP4=",rr))EP4=x;
  // }
}
return 0;
}
