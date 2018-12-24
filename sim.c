/* Optimal heads-up preflop holdem.
   A.P.Selby
   February 1999 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#define BIG (1e30)
#define HOLDEM
#define fnep(xxi,xxj) ((xxi)*c0+(xxj)+1)
#define fney(xxk,xxl) (d0*c0+(xxk)*c1+(xxl)+1)
#define fnem(xxi) (d0*c0+d1*c1+(xxi)+1)
#define sgn(x) ((x)>0?1:((x)<0?-1:0))

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
char *l0;
FILE *fp;

seed=t0=time(NULL);r=1;pl=0;nr=1;pr=6;an=.5;lim=1;liv=1;blf=d0f=d1f=0;d=169;list=0;
EP1=1e-10;EP2=5e-10;EP3=0;EP4=0;
for(i=1;i<ac;i++){l0=strchr(av[i],'=');if(l0){
 rr=l0-av[i]+1;j=atoi(l0+1);x=atof(l0+1);
 if(!strncmp(av[i],"seed=",rr))seed=j;
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
}}
if(blf==0)bl=lim?r/2:r/2;//2*r*an;
if(d0f==0)d0=(1+liv)*d;
if(d1f==0)d1=d;
fp=fopen("log","a");
if(fp==0){printf("Couldn't open log file\n");exit(0);}
for(i=0;i<2;i++)fprintf(i==0?fp:stdout,"%s seed=%d an=%g bl=%g lim=%d r=%g liv=%d\\\n\
 d0=%d d1=%d nr=%d pl=%d pr=%d EP1=%g EP2=%g EP3=%g EP4=%g\n",
 av[0],seed,an,bl,lim,r,liv,d0,d1,nr,pl,pr,EP1,EP2,EP3,EP4);
fclose(fp);
srand48(seed);
#ifdef HOLDEM
printf("Holdem ");
#else
printf("Numbers vs numbers ");
#endif
if(lim)printf("limit: betsize=%g ",r); else printf("potlimit: betsize=%g*pot ",r);
printf("ante=%g blind=%g (%s)\n",an,bl,liv?"LIVE":"NON-LIVE");
#ifdef HOLDEM
for(i=0;i<13;i++)for(j=0;j<13;j++)
 sprintf(hs[i*13+j],"%c%c%c",i>=j?cd[i+2]:cd[j+2],
  i>=j?cd[j+2]:cd[i+2],"os"[i>j]);
fp=popen("gunzip -c fa2n.gz","r");
if(fp==0){printf("Couldn't open EV file\n");exit(0);}
for(i=0;i<169;i++)for(j=0;j<169;j++)fscanf(fp,"%*s %*s %lf",&wa[i][j]);
pclose(fp);
for(i=0;i<169;i++)for(j=0;j<169;j++){
 x=fabs(wa[i][j]+wa[j][i]);
 if(x>1e-5)printf("Error of %g at %d,%d\n",x,i,j);
}
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
}
if(t!=52*51*50*49/(2*2)){printf("Error: total count=%d\n",t);exit(0);}
for(i=0;i<169;i++)for(j=0;j<169;j++)prob[i][j]*=169.0*169.0/t;
}
if(list)for(i=0;i<169;i++)for(j=0;j<169;j++){
 x=wa[i][j]/prob[i][j];printf("%s %s    P(win)=%8.6f    EV=%9.6f\n",hs[i],hs[j],(x+1)/2,x);}
#endif

c0=2*nr+1;c1=2*nr+2+(2*nr-1)*liv;
if(pl==1){i=c0;c0=c1;c1=i;i=d0;d0=d1;d1=i;}
n=d0*(c0-1)+d1; //#variables-#constraints=#"non-basic" variables=dim(complex) ==> var=0
m=    d1*c1+d0; //#constraints=#"basic" variables

{
int i0,i1,j0,j1,k,k1,l,ind,it,t,dg,cn,tph;
int mi[m+1],ni[n+1],rc[m+n+1],lk[d1],ji[d0];
double g,s,s1,min,max,v,z,scale;
double (*a)[n+1],p[d0][c0],q[d1][c1],pv[d1][c1],xx[m+n+1];

a=(double (*)[n+1])malloc((m+1)*(n+1)*sizeof(double));
if(a==0){printf("Couldn't malloc a\n");exit(0);}

/*
for(i=0;i<d0;i++)for(k=0;k<d1;k++){
 descc(temp0,pl,i);descc(temp1,1-pl,k);printf("%s vs %s\n",temp0,temp1);
 printf("    ");
 for(l=0;l<c1;l++){descs(temp0,1-pl,l);lpad(temp0,pr+8);printf("%s",temp0);}printf("\n");
 for(j=0;j<c0;j++){descs(temp0,pl,j);lpad(temp0,6);printf("%s",temp0);
  for(l=0;l<c1;l++)printf("%*.*g  ",pr+6,pr,ev(i,j,k,l));
  printf("\n");}
 printf("\n");}
*/

// Vars:        p(i,j)   (i<d0,j<c0)
//              y(k,l)   (k<d1,l<c1)
//              m(k)     (k<d1)
// Equations:   m(k)+y(k,l)=sum_{i,j}p(i,j)ev(i,j,k,l)
//              sum_j p(i,j)=1
// Constraints: y(k,l)>=0, p(i,j)>=0, m(k)=anything
// Objective: sum_k m(k)
// Initialise so that p(i,ji[i]), y(k,not lk[k]), m(i) are basic
//                    p(i,not ji[i]), y(k,lk[k]) are non-basic
//
// Variable numbers of P(i,j) are 1...d0*c0
// Variable numbers of Y(k,l) are d0*c0+1...d0*c0+d1*c1
// Variable numbers of M(k)   are d0*c0+d1*c1+1...d0*c1+d1*(c1+1)

scale=d0*d1/(1+liv);
printf("Finding initial pure strategy\n");
for(i=0;i<d0;i++)ji[i]=0;//(c0>1&&pl==0&&(i&1)==0?1:0);
for(k=0;k<d1;k++)for(l=0;l<c1;l++){
 for(i=0,s=0;i<d0;i++)s+=ev(i,ji[i],k,l);pv[k][l]=s;
}

tph=(liv==1&&pl==0);
for(cn=0;cn<1+tph;cn++){
 i0=i=0;
 do{
  for(j=0,max=-BIG;j<c0;j++){
   for(k=0,s=0;k<d1;k++){
    for(l=0,min=BIG;l<c1;l++){
     if(cn==tph||((l+1)&3)<2){v=pv[k][l]+ev(i,j,k,l)-ev(i,ji[i],k,l);if(v<min)min=v;}
    }
    s+=min;
   }
   if(s>max){max=s;j0=j;}
  }
  if(j0!=ji[i]){
   for(k=0;k<d1;k++)for(l=0;l<c1;l++)pv[k][l]+=ev(i,j0,k,l)-ev(i,ji[i],k,l);
   ji[i]=j0;i0=i;
  }
  i++;if(i==d0)i=0;
 }while(i!=i0);
}

/*
printf("Initial pure strategy for player %d:\n",pl);
for(j=0;j<c0;j++){
 descs(temp0,pl,j);printf("    --- %s ---\n",temp0);
 for(i=0;i<d0;i++)if(ji[i]==j){
  descc(temp0,pl,i);rpad(temp0,10);printf("%s",temp0);
 }
 printf("\n\n");
}
for(k=0,s=0;k<d1;k++){for(l=0,min=BIG;l<c1;l++)if(pv[k][l]<min)min=pv[k][l];s+=min;}
printf("Value=%g\n",s/scale);
*/

// If start with all ji[i]=1, EP4=0, then tends to be stuck (almost) immediately (esp pl=1,nr>1)
// for(i=0;i<d0;i++)ji[i]=1;
printf("Setting up tableau\n");
for(k=0;k<d1;k++){
 for(l=0,min=BIG;l<c1;l++){
  for(i=0,s=0;i<d0;i++)s+=ev(i,ji[i],k,l);
  if(s<min){min=s;lk[k]=l;}
 }
}

ind=1;
for(i=0;i<d0;i++)mi[ind++]=fnep(i,ji[i]);
for(k=0;k<d1;k++)for(l=0;l<c1;l++)if(l!=lk[k])mi[ind++]=fney(k,l);
for(k=0;k<d1;k++)mi[ind++]=fnem(k);
ind=1;
for(i=0;i<d0;i++)for(j=0;j<c0;j++)if(j!=ji[i])ni[ind++]=fnep(i,j);
for(k=0;k<d1;k++)ni[ind++]=fney(k,lk[k]);
for(i=1;i<=m;i++)rc[mi[i]]=i;
for(i=1;i<=n;i++)rc[ni[i]]=i;

for(i=0;i<d0;i++){
 t=rc[fnep(i,ji[i])];
 for(i1=0;i1<d0;i1++)for(j=0;j<c0;j++)if(j!=ji[i1])
  a[rc[fnep(i,ji[i])]][rc[fnep(i1,j)]]=(i==i1);
 for(k=0;k<d1;k++)a[t][rc[fney(k,lk[k])]]=0;
 a[t][0]=1;
}
for(k=0;k<d1;k++)for(l=0;l<c1;l++)if(l!=lk[k]){
 t=rc[fney(k,l)];
 for(i=0;i<d0;i++)for(j=0;j<c0;j++)if(j!=ji[i])
  a[t][rc[fnep(i,j)]]=-(ev(i,j,k,l)-ev(i,ji[i],k,l)-ev(i,j,k,lk[k])+ev(i,ji[i],k,lk[k]));
 for(k1=0;k1<d1;k1++)a[t][rc[fney(k1,lk[k1])]]=-(k==k1);
 for(i=0,s=0;i<d0;i++)s+=ev(i,ji[i],k,l)-ev(i,ji[i],k,lk[k]);a[t][0]=s;
}
for(k=0;k<d1;k++){
 t=rc[fnem(k)];
 for(i=0;i<d0;i++)for(j=0;j<c0;j++)if(j!=ji[i])
  a[t][rc[fnep(i,j)]]=-(ev(i,j,k,lk[k])-ev(i,ji[i],k,lk[k]));
 for(k1=0;k1<d1;k1++)a[t][rc[fney(k1,lk[k1])]]=(k==k1);
 for(i=0,s=0;i<d0;i++)s+=ev(i,ji[i],k,lk[k]);a[t][0]=s;
}
for(j=0;j<=n;j++){
 for(k=0,s=0;k<d1;k++)s+=a[rc[fnem(k)]][j];
 a[0][j]=s;
}
for(i=1;i<=m;i++)a[i][0]+=EP4*drand48();
for(j=1;j<=n;j++)a[0][j]+=EP4*drand48();

it=0;
//procp(m,n,c0,c1,d0,d1,a,mi,ni);
ep1=ep2=ep3=-BIG;
Ep1=Ep2=Ep3=BIG;

while(1){
for(i=1;i<=m;i++)rc[mi[i]]=-i;
for(i=1;i<=n;i++)rc[ni[i]]=i;
g=-BIG;
for(j=1;j<=n;j++){
 if(ni[j]>d0*c0+d1*c1&&a[0][j]>0)for(i=0;i<=m;i++)a[i][j]=-a[i][j];
 if(a[0][j]>-EP1){if(-a[0][j]>ep1)ep1=-a[0][j];continue;}
 if(-a[0][j]<Ep1)Ep1=-a[0][j];
 s=BIG;s1=0;
 for(i=1;i<=m;i++){
  if(mi[i]>d0*c0+d1*c1)continue;
  if(a[i][j]<=EP2){if(a[i][j]>ep2)ep2=a[i][j];continue;}
  if(a[i][j]<Ep2)Ep2=a[i][j];
  x=fabs(a[i][0]);
  if(x<=EP3){
   if(x>ep3)ep3=x;
   if(s>0){s=0;i0=i;}else{
    for(j0=1;j0<=m+n;j0++){
     t=rc[j0];if(t<0){x=(i0==-t);y=(i==-t);}else{x=a[i0][t];y=a[i][t];}
     //     z=(x==0?(y==0?0:-1):(y==0?1:x/a[i0][j]/(y/a[i][j])*sgn(y)-sgn(y)));
     z=x/a[i0][j]-y/a[i][j];
     //if(fabs(z)>EP3)break;
     if(z!=0)break;
    }
    if(j0>m+n){printf("Stuck?\n");exit(0);}
    if(z>0)i0=i;
   }
  }else{
   if(x<Ep3)Ep3=x;
   if(s>0){x=a[i][0]/a[i][j];if(x<s){s=x;i0=i;}}
  }
 }
 if(s==BIG){printf("Unbounded\n");exit(0);}
 if(s>0)v=-s*a[0][j]; else v=1/a[0][j];
 if(v>g){g=v;i1=i0;j1=j;}
}
if(g==-BIG)break;
for(i=1,dg=0;i<=m;i++)dg+=(fabs(a[i][0])<=EP3);
descv(temp0,c0,c1,d0,d1,mi[i1]);rpad(temp0,12);
descv(temp1,c0,c1,d0,d1,ni[j1]);rpad(temp1,12);
printf("%4d) Obj=%15.9g  %s->%s A(i,j)=%15.9g Degen=%5d Val=",
 it,a[0][0]/scale,temp0,temp1,a[i1][j1],dg);
if(g>=0)printf(" %15.9g\n",g/scale); else printf("[%15.9g]\n",-scale/g);
pivot(m,n,i1,j1,mi,ni,a);
it++;
//procp(m,n,c0,c1,d0,d1,a,mi,ni);
}

printf("\nDONE.  Time=%ds\n",time(NULL)-t0);
for(i=0;i<=m+n;i++)xx[i]=0;
for(i=1;i<=m;i++)xx[mi[i]]=a[i][0];
for(i=0;i<d0;i++)for(j=0;j<c0;j++)p[i][j]=xx[fnep(i,j)];
for(i=0;i<=m+n;i++)xx[i]=0;
for(j=1;j<=n;j++)xx[ni[j]]=a[0][j];
for(k=0;k<d1;k++)for(l=0;l<c1;l++)q[k][l]=xx[fney(k,l)];
printf("Final objective function=%.9g (value to player %d)\n",a[0][0]/scale,pl);
for(i=0,s=0;i<d0;i++){for(j=0,x=0;j<c0;j++)x+=p[i][j];s+=(x-1)*(x-1);}s=sqrt(s/d0);
printf("RMS primal prob. error=%.9g\n",s);
for(k=0,s=0;k<d1;k++){for(l=0,x=0;l<c1;l++)x+=q[k][l];s+=(x-1)*(x-1);}s=sqrt(s/d1);
printf("RMS dual prob. error=%.9g\n",s);
for(i=0;i<d0;i++){
 for(j=0,s=0;j<c0;j++){if(p[i][j]<0)p[i][j]=0;s+=p[i][j];}
 for(j=0;j<c0;j++)p[i][j]/=s;
}
for(k=0;k<d1;k++){
 for(l=0,s=0;l<c1;l++){if(q[k][l]<0)q[k][l]=0;s+=q[k][l];}
 for(l=0;l<c1;l++)q[k][l]/=s;
}
for(k=0,s=0;k<d1;k++){g=BIG;
 for(l=0;l<c1;l++){
  for(i=0,y=0;i<d0;i++)for(j=0;j<c0;j++)y+=p[i][j]*ev(i,j,k,l);
  if(y<g)g=y;
 }s+=g;
}
printf("\nActual primal value=%.9g\n",s/scale);
for(i=0,s=0;i<d0;i++){g=-BIG;
 for(j=0;j<c0;j++){
  for(k=0,y=0;k<d1;k++)for(l=0;l<c1;l++)y+=q[k][l]*ev(i,j,k,l);
  if(y>g)g=y;
 }s+=g;
}
printf("Actual dual value=%.9g\n\n",s/scale);
printf("EP1=%11.5g   EP2=%11.5g   EP3=%11.5g\n",EP1,EP2,EP3);
printf("ep1=%11.5g   ep2=%11.5g   ep3=%11.5g\n",ep1,ep2,ep3);
printf("Ep1=%11.5g   Ep2=%11.5g   Ep3=%11.5g\n",Ep1,Ep2,Ep3);
printf("d0=%d d1=%d nr=%d r=%g EP4=%g\n",d0,d1,nr,r,EP4);
printf("\n");prp(pl,c0,d0,pr,p);
printf("\n");prp(1-pl,c1,d1,pr,q);
}
return 0;
}

void prp(int px,int cx,int dx,int pr,double (*p)[cx]){
int i,j,pure[dx];
double ep;
 
ep=pow(10,-pr);
for(i=0;i<dx;i++)for(j=0,pure[i]=0;j<cx;j++)if(fabs(p[i][j]-1)<ep)pure[i]=1;
printf("Strategy for player %d (%s blind)\nPure bit:\n",px,px?"small":"big");
for(j=0;j<cx;j++){
 descs(temp0,px,j);printf("    --- %s ---\n",temp0);
 for(i=0;i<dx;i++)if(pure[i]&&p[i][j]>.5){
  descc(temp0,px,i);rpad(temp0,10);printf("%s",temp0);
 }
 printf("\n\n");
}
printf("Probabilistic bit:\n");
printf("     ");
for(j=0;j<cx;j++){descs(temp0,px,j);lpad(temp0,pr+8);printf("%s",temp0);}printf("\n");
for(i=0;i<dx;i++)if(!pure[i]){descc(temp0,px,i);lpad(temp0,5);printf("%s  ",temp0);
 for(j=0;j<cx;j++)printf("%*.*g  ",pr+6,pr,p[i][j]);printf("\n");}
}

void pivot(int m,int n,int i1,int j1,int *mi,int *ni,double (*a)[n+1]){
int i,j,t;
double r;
t=mi[i1];mi[i1]=ni[j1];ni[j1]=t;
r=1/a[i1][j1];
for(i=0;i<=m;i++)if(i!=i1)for(j=0;j<=n;j++)if(j!=j1)a[i][j]-=a[i1][j]*a[i][j1]*r;
for(i=0;i<=m;i++)if(i!=i1)a[i][j1]=-a[i][j1]*r;
for(j=0;j<=n;j++)if(j!=j1)a[i1][j]=a[i1][j]*r;
a[i1][j1]=r;
}
void procp(int m,int n,int c0,int c1,int d0,int d1,double (*a)[n+1],int *mi,int *ni){
int i,j,ii;
for(i=1;i<=12;i++)printf(" ");
for(j=1;j<=n;j++){descv(temp0,c0,c1,d0,d1,ni[j]);lpad(temp0,pr+7);printf("%s",temp0);}
for(i=1;i<=pr;i++)printf(" ");printf(" Constr\n");
for(i=1;i<=m+1;i++){
 if(i<=m){ii=i;descv(temp0,c0,c1,d0,d1,mi[i]);lpad(temp0,12);printf("%s ",temp0);}
  else{ii=0;printf("         Obj ");}
 for(j=1;j<=n+1;j++)printf("%*.*g ",pr+6,pr,a[ii][(j<=n?j:0)]);printf("\n");
}
printf("\n");
}
void descv(char *buf,int c0,int c1,int d0,int d1,int st){
int act,sit;
if(st<=d0*c0){
 act=(st-1)%c0;sit=(st-1)/c0;
 descs(buf,pl,act);
 sprintf(buf+strlen(buf),"(");descc(buf+strlen(buf),pl,sit);sprintf(buf+strlen(buf),")");
 return;
}
if(st<=d0*c0+d1*c1){
 act=(st-d0*c0-1)%c1;sit=(st-d0*c0-1)/c1;
 sprintf(buf,"S:");descs(buf+2,1-pl,act);
 sprintf(buf+strlen(buf),"(");descc(buf+strlen(buf),1-pl,sit);sprintf(buf+strlen(buf),")");
 return;
}
sit=st-d0*c0-d1*c1-1;
sprintf(buf,"M%d(",pl);descc(buf+strlen(buf),1-pl,sit);sprintf(buf+strlen(buf),")");
}
void rpad(char *buf,int n){int i;for(i=strlen(buf);i<n;i++)buf[i]=' ';buf[i]=0;}
void lpad(char *buf,int n){int i,s;
s=strlen(buf);if(s<n){
 for(i=n-1;i>=n-s;i--)buf[i]=buf[i-(n-s)];
 for(i=0;i<n-s;i++)buf[i]=' ';
 buf[n]=0;
}}
void spc(int n){for(;n>0;n--)printf(" ");}

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

void descs(char *buf,int px,int st){
if(px==0&&st==2*nr){sprintf(buf,"R%d",nr);return;}
if(px==0||liv==0){sprintf(buf,"R%d%c",st/2,(st&1)?'C':'F');return;}
st+=(st>=1);
sprintf(buf,"%sR%d%c",((st&3)>=2)?"C":"",st/4,(st&1)?'C':'F');
}
#ifdef HOLDEM
void descc(char *buf,int px,int i){
if(px==1||liv==0)sprintf(buf,"%s",hs[i]); else sprintf(buf,"%c%s",(i&1)?'r':'c',hs[i/2]);
}
#else
void descc(char *buf,int px,int i){
if(px==1||liv==0)sprintf(buf,"%d",i); else sprintf(buf,"%c%d",(i&1)?'r':'c',i/2);
}
#endif
double ev(int i0,int s0,int i1,int s1){
int r0,r1,a1,o0,o1,c,t,rr,cf;
double ev,make;
if(pl==1){t=i0;i0=i1;i1=t; t=s0;s0=s1;s1=t;}
if(liv==0) {r0=s0>>1;o0=s0&1;r1=s1>>1;o1=s1&1;c=0;} else
 {r0=s0>>1;o0=s0&1;s1+=(s1>=1);r1=s1>>2;o1=s1&1;c=(s1&2)>>1; if((i0&1)==c)return 0; else i0=i0>>1;}
a1=r1+c;
if(r0>=a1&&o1==0) {rr=2*r1+c;cf=1;}                  // P1 F after making r1 raises
if(r0>=a1&&o1==1) {rr=2*r1+c+1;cf=0;}                // P1 C after making r1 raises
if(r0< a1&&o0==0) {rr=2*r0+1-c+(r0==0&&c==1);cf=-1;} // P0 F after making r0 (voluntary) raises
if(r0< a1&&o0==1) {rr=2*r0+2-c;cf=0;}                // P0 C after making r0 (voluntary) raises
if(rr==0)make=an; else {if(lim)make=an+bl+r*(rr-1); else make=(an+bl)*pow(1+2*r,rr-1);}
#ifdef HOLDEM
if(cf==0)ev=make*wa[i0][i1]; else ev=make*prob[i0][i1]*cf;
#else
if(cf==0)ev=make*(i0>i1?1:(i0<i1?-1:0)); else ev=make*cf;
#endif
if(pl==1)ev=-ev;
return ev;
}

// To define the game, you need to supply the three functions descs(),descc(),ev().
// P0 has d0 situations, and in each one he has c0 different betting strategies.
// P1 has d1 situations, and in each one he has c1 different betting strategies.
// Decide on a numbering scheme for situtations and betting strategies.
//
// descs(char *buf,int pl,int st) should return into buf a short string describing strategy
// number st of player pl (using the pl passed to it).
// descc(char *bug,int pl,int i) should return into buf a short string describing situation number i
//
// Let Px denote Player pl, Py denote Player 1-pl (using the global pl).
//
// double ev(int i0,int s0,int i1,int s1) should return:
// [Value (to Px) given that Px is in situation i0 and does strategy s0,
//   and Py is in situation i1 and does s1]*Pr(i0,i1)*scale
// 
// This is set up so that if p(i0,s0) is the probability of Px using strategy s0 given he
// is in situation i0, and q(i1,s1) is the probability of Py using strategy s1 given he
// is in situation i1, then the expected value (to Px) of the whole game is
// Sum over all (i0,s0,i1,s1) of p(i0,s0)ev(i0,s0,i1,s1)q(i1,s1)/scale
//
// scale is a arbitrary way of getting the numbers ev(.,.,.,.) not too far from 1. This
// helps stability of the simplex algorithm.
