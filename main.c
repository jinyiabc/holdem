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
  if(l0){
   rr=l0-av[i]+1;j=atoi(l0+1);x=atof(l0+1);
   if(!strncmp(av[i],"an=",rr))an=x;
   if(!strncmp(av[i],"bl=",rr)){bl=x;blf=1;}
   if(!strncmp(av[i],"lim=",rr))lim=(j!=0);
   if(!strncmp(av[i],"liv=",rr))liv=j;
   if(!strncmp(av[i],"list=",rr))list=j;
   if(!strncmp(av[i],"pr=",rr))pr=j;
   if(!strncmp(av[i],"r=",rr))r=x;
   if(!strncmp(av[i],"nr=",rr))nr=j;
   if(!strncmp(av[i],"d=",rr))d=j;
   if(!strncmp(av[i],"d0=",rr)){d0=j;d0f=1;}
   if(!strncmp(av[i],"d1=",rr)){d1=j;d1f=1;}
   if(!strncmp(av[i],"pl=",rr))pl=j;
   if(!strncmp(av[i],"EP=",rr))EP1=EP2=EP3=x;
   if(!strncmp(av[i],"EP1=",rr))EP1=x;
   if(!strncmp(av[i],"EP2=",rr))EP2=x;
   if(!strncmp(av[i],"EP3=",rr))EP3=x;
   if(!strncmp(av[i],"EP4=",rr))EP4=x;
  }
}

fp=fopen("fa2n","r");
if(fp==0){printf("Couldn't open EV file\n");exit(0);}
for(i=0;i<169;i++)for(j=0;j<169;j++){
  fscanf(fp,"%*s %*s %lf",&wa[i][j]);
  // printf("%f\n",wa[i][j] );
}
fclose(fp);

{int i0,i1,j0,j1,s0,s1,s2,s3,n,t;
t=0;
for(i=0;i<169;i++)for(j=0;j<169;j++){
 i0=i/13;i1=i%13;j0=j/13;j1=j%13;
 n=0;
 for(s0=0;s0<4;s0++)for(s1=0;s1<4;s1++)for(s2=0;s2<4;s2++)for(s3=0;s3<4;s3++)
  n+=((i0<=i1||s0==s1)&&(i0>i1||s0!=s1)&&(j0<=j1||s2==s3)&&(j0>j1||s2!=s3)&&
       (i0!=j0||s0!=s2)&&(i0!=j1||s0!=s3)&&(i1!=j0||s1!=s2)&&(i1!=j1||s1!=s3));
 if(i0==i1)n/=2;
 if(j0==j1)n/=2;
 t+=n;
 prob[i][j]=n;
 printf(" %f\n",prob[i][j] );
}
if(t!=52*51*50*49/(2*2)){printf("Error: total count=%d\n",t);exit(0);}
for(i=0;i<169;i++)for(j=0;j<169;j++){
  // prob[i][j]*=169.0*169.0/t;
  prob[i][j] /=t;
  printf("prob[%d][%d]= %f\n",i,j,prob[i][j] );
}
}

if(list)for(i=0;i<169;i++)for(j=0;j<169;j++){
 x=wa[i][j]/prob[i][j];
 // printf("%s %s    P(win)=%8.6f    EV=%9.6f\n",hs[i],hs[j],(x+1)/2,x);
}


return 0;
}



// The betting structure used in this ev() is:
// Both ante an. P0 forced to bring it in for bl, live if liv=1.
// Then P1,P0,P1,... call,fold or raise alternately. They are allowed up to nr actions each (not
// counting the forced blind) which don't end the betting round. After that they must call or fold.
// If lim=1 then the raise is r, otherwise it is an (r)th of the pot after calling.
//
// The numbering of betting strategies used below is as follows:
// (1) Non-live case.
//     For P0 (2nr+1 options): R0F, R0C, R1F, R1C, ..., R(nr-1)F, R(nr-1)C, R(nr).
//     RnC means raise n times then call, etc.
//     Last one (#2nr) is special. After P0's (nr)th raise P1 must call or fold.
//     For P1 (2nr+2 options): R0F, R0C, ..., R(nr)F, R(nr)C
// (2) Live case.
//     For P0 (2nr+1 options): R0F, R0C, R1F, R1C, ..., R(nr-1)F, R(nr-1)C, R(nr).
//     For P1 (4nr+1 options): R0F, [R0C,] CR0F, CR0C,               0,  1,2
//                             R1F, R1C,   CR1F, CR1C,               3,4,5,6
//                             ...                                   ...
//                             R(nr)F, R(nr)C, [CR(nr)F, CR(nr)C]    4nr-1,4nr
//     (Strategies in [ ] don't exist)
// (This numbering is relied on outside descs(),ev() where it sets c0,c1, and where
// it finds the initial pure strategy.)
//
// The number of the situations (cards) for P0 in the live case is
// cC0, rC0, cC1, rC1, ...
// cCn = (n)th cards, P1 has called to begin with  (P1 has CRmF or CRmC)
// rCn = (n)th cards, P1 has raised or folded to begin with (P1 has RmF or RmC)
