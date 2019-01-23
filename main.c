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
float total=0;
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
 // total+=wa[i][j];   //total = -0.004034
 prob[i][j]=n;
}


if(t!=52*51*50*49/(2*2)){printf("Error: total count=%d\n",t);exit(0);}
for(i=0;i<169;i++)for(j=0;j<169;j++)prob[i][j]*=169.0*169.0/t;
// printf("total = %f\n",total );
if(list)for(i=0;i<169;i++)for(j=0;j<169;j++){
 x=wa[i][j]/prob[i][j];
 total+=wa[i][j];
 printf("%s %s    P(win)=%8.6f    EV=%9.6f prob=%9.6f\n",hs[i],hs[j],(x+1)/2,x,prob[i][j]);}
#endif
printf("test\n" );
descc(temp0,pl,0);rpad(temp0,10);printf("%s",temp0);printf("\n");
// descc(temp1,pl,0);lpad(temp1,10);printf("%s",temp1);printf("\n");
}

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
// Variable numbers of M(k)   are d0*c0+d1*c1+1...d0*c0+d1*(c1+1)

scale=d0*d1/(1+liv);
printf("Finding initial pure strategy\n");
for(i=0;i<d0;i++)ji[i]=0;//(c0>1&&pl==0&&(i&1)==0?1:0);
for(k=0;k<d1;k++)for(l=0;l<c1;l++){
 for(i=0,s=0;i<d0;i++)s+=ev(i,ji[i],k,l);pv[k][l]=s;
}

tph=(liv==1&&pl==0);
/*printf("tph=%d\n",tph );

for(cn=0;cn<1+tph;cn++){
for (int l = 0; l < 10; l++) {
  descs(temp0,1-pl,l);lpad(temp0,pr+8);printf("%s",temp0);printf("\n");
  printf("cn==tph||((%d+1)&3)<2 %d\n",l,cn==tph||((l+1)&3)<2 );   //If tph =1 R0F R1F R1C R2F R2C ......
}
}
*/
for(cn=0;cn<1+tph;cn++){
 i0=i=0;
 do{
  for(j=0,max=-BIG;j<c0;j++){
   for(k=0,s=0;k<d1;k++){
    for(l=0,min=BIG;l<c1;l++){
     if(cn==tph||((l+1)&3)<2)
     {v=pv[k][l]+ev(i,j,k,l)-ev(i,ji[i],k,l);
       if(v<min)min=v;}
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
/*
--- R0C ---
r22o      c32o      r32o      c42o      r42o      c52o      r52o      c62o      r62o      c72o      r72o      c82o      r82o      c92o      r92o      cT2o      rT2o      rJ2o      rQ2o      rK2o      rA2o      c32s      r32s      r33o      c43o      r43o      c53o      r53o      c63o      r63o      c73o      r73o      c83o      r83o      c93o      r93o      cT3o      rT3o      rJ3o      rQ3o      rK3o      rA3o      c42s      r42s      c43s      r43s      c54o      r54o      c64o      r64o      c74o      r74o      c84o      r84o      c94o      r94o      rT4o      rJ4o      rQ4o      rK4o      c52s      r52s      c53s      r53s      c54s      r54s      c65o      r65o      c75o      r75o      c85o      r85o      c95o      r95o      rT5o      rJ5o      rQ5o      rK5o      c62s      r62s      c63s      r63s      c64s      r64s      c65s      r65s      c76o      r76o      c86o      r86o      r96o      rT6o      rJ6o      rQ6o      rK6o      c72s      r72s      c73s      r73s      c74s      r74s      c75s      r75s      c76s      r76s      r87o      r97o      rT7o      rJ7o      rQ7o      rK7o      c82s      r82s      c83s      r83s      c84s      r84s      c85s      r85s      r86s      r87s      r98o      rT8o      rJ8o      rQ8o      rK8o      c92s      r92s      c93s      r93s      c94s      r94s      r95s      r96s      r97s      r98s      rT9o      rJ9o      rQ9o      rT2s      rT3s      rT4s      rT5s      rT6s      rT7s      rT8s      rT9s      rJTo      rQTo      rJ2s      rJ3s      rJ4s      rJ5s      rJ6s      rJ7s      rJ8s      rJ9s      rJTs      rQ2s      rQ3s      rQ4s      rQ5s      rQ6s      rQ7s      rQ8s      rQ9s      rK2s      rK3s      rK4s      rK5s      rK6s      rK7s

--- R1 ---
c22o      cJ2o      cQ2o      cK2o      cA2o      c33o      cJ3o      cQ3o      cK3o      cA3o      c44o      r44o      cT4o      cJ4o      cQ4o      cK4o      cA4o      rA4o      c55o      r55o      cT5o      cJ5o      cQ5o      cK5o      cA5o      rA5o      c66o      r66o      c96o      cT6o      cJ6o      cQ6o      cK6o      cA6o      rA6o      c77o      r77o      c87o      c97o      cT7o      cJ7o      cQ7o      cK7o      cA7o      rA7o      c86s      c87s      c88o      r88o      c98o      cT8o      cJ8o      cQ8o      cK8o      cA8o      rA8o      c95s      c96s      c97s      c98s      c99o      r99o      cT9o      cJ9o      cQ9o      cK9o      rK9o      cA9o      rA9o      cT2s      cT3s      cT4s      cT5s      cT6s      cT7s      cT8s      cT9s      cTTo      rTTo      cJTo      cQTo      cKTo      rKTo      cATo      rATo      cJ2s      cJ3s      cJ4s      cJ5s      cJ6s      cJ7s      cJ8s      cJ9s      cJTs      cJJo      rJJo      cQJo      rQJo      cKJo      rKJo      cAJo      rAJo      cQ2s      cQ3s      cQ4s      cQ5s      cQ6s      cQ7s      cQ8s      cQ9s      cQTs      rQTs      cQJs      rQJs      cQQo      rQQo      cKQo      rKQo      cAQo      rAQo      cK2s      cK3s      cK4s      cK5s      cK6s      cK7s      cK8s      rK8s      cK9s      rK9s      cKTs      rKTs      cKJs      rKJs      cKQs      rKQs      cKKo      rKKo      cAKo      rAKo      cA2s      rA2s      cA3s      rA3s      cA4s      rA4s      cA5s      rA5s      cA6s      rA6s      cA7s      rA7s      cA8s      rA8s      cA9s      rA9s      cATs      rATs      cAJs      rAJs      cAQs      rAQs      cAKs      rAKs      cAAo      rAAo

Value=0.0266245
*/
// #define fnep(xxi,xxj) ((xxi)*c0+(xxj)+1)
// #define fney(xxk,xxl) (d0*c0+(xxk)*c1+(xxl)+1)
// #define fnem(xxi) (d0*c0+d1*c1+(xxi)+1)
printf("Setting up tableau\n");
for(k=0;k<d1;k++){
 for(l=0,min=BIG;l<c1;l++){
  for(i=0,s=0;i<d0;i++)s+=ev(i,ji[i],k,l);
  if(s<min){min=s;lk[k]=l;}
 }
}

// printf("c0=%d fnep=%d\n",c0,fnep(1,10) );   //14
// printf("c0=%d d0=%d fney=%d\n",c0,d0,fney(1,10) ); //1030

ind=1;
for(i=0;i<d0;i++){mi[ind++]=fnep(i,ji[i]);}   // d0
for(k=0;k<d1;k++)for(l=0;l<c1;l++)if(l!=lk[k])mi[ind++]=fney(k,l);  // d1(c1-1)
for(k=0;k<d1;k++)mi[ind++]=fnem(k); // d1
ind=1;
for(i=0;i<d0;i++)for(j=0;j<c0;j++)if(j!=ji[i])ni[ind++]=fnep(i,j);  //d0*(c0-1)
for(k=0;k<d1;k++)ni[ind++]=fney(k,lk[k]); // d1
for(i=1;i<=m;i++)rc[mi[i]]=i;  // 1.2,..,(d1*c1+d0+1)
for(i=1;i<=n;i++)rc[ni[i]]=i;  // (d1*c1+d0+1)+1,...,d0*c0+d1*c1+d1+1

// Vars:        p(i,j)   (i<d0,j<c0)
//              y(k,l)   (k<d1,l<c1)
//              m(k)     (k<d1)
// Equations:   m(k)+y(k,l)=sum_{i,j}p(i,j)ev(i,j,k,l)
//              sum_j p(i,j)=1
// Constraints: y(k,l)>=0, p(i,j)>=0, m(k)=anything
// Objective: sum_k m(k)
// Initialise so that p(i,ji[i]), y(k,not lk[k]), m(i) are basic = d1*c1+d0
//                    p(i,not ji[i]), y(k,lk[k]) are non-basic = d0*(c0-1)+d1
// int b[10];
// size_t n = sizeof(b) / sizeof(int);
// printf("%zu\n", n );   // length of array

for(i=0;i<d0;i++){
  for (j = 0; j < c0; j++) {
    /* code */

 // t=rc[fnep(i,ji[i])];   // basic variables of d0, define 1 for non-basic variables
 descc(temp0,pl,i);rpad(temp0,10);
 descs(temp1,pl,j);rpad(temp1,10);
 printf("i=%d j=%d %s %s\n",i,j,temp0,temp1);


}
 // for(i1=0;i1<d0;i1++)for(j=0;j<c0;j++)if(j!=ji[i1])   // a[basic][non-basic]
 //  {
 //    a[rc[fnep(i,ji[i])]][rc[fnep(i1,j)]]=(i==i1);   // for given (i,j), (i,not j) => a[basic][non-basic] =1 d0*(c0-1)
 //  //   if(i==i1)
 //  // printf("a[%d][%d]= %f i=%d j=%d\n", rc[fnep(i,ji[i])],rc[fnep(i1,j)],a[rc[fnep(i,ji[i])]][rc[fnep(i1,j)]],i,j);
 //  }
 // for(k=0;k<d1;k++)a[t][rc[fney(k,lk[k])]]=0;
}

for(k=0;k<d1;k++){
  for (l = 0; l < c1; l++) {

 descc(temp0,1-pl,k);rpad(temp0,10);
 descs(temp1,1-pl,l);rpad(temp1,10);
 printf("k=%d l=%d %s %s\n",k,l,temp0,temp1);


}

}

// a=(double (*)[n+1])malloc((m+1)*(n+1)*sizeof(double));
for(k=0;k<d1;k++)for(l=0;l<c1;l++)if(l!=lk[k]){
 t=rc[fney(k,l)];    // basic variables of k, not lk(k)
 for(i=0;i<d0;i++)for(j=0;j<c0;j++)if(j!=ji[i])                                           // non-basic variables
  a[t][rc[fnep(i,j)]]=-(ev(i,j,k,l)-ev(i,ji[i],k,l)-ev(i,j,k,lk[k])+ev(i,ji[i],k,lk[k])); // d0*(c0-1)
 for(k1=0;k1<d1;k1++)a[t][rc[fney(k1,lk[k1])]]=-(k==k1);                                  // d1
 for(i=0,s=0;i<d0;i++)s+=ev(i,ji[i],k,l)-ev(i,ji[i],k,lk[k]);a[t][0]=s;                   //
}

for(k=0;k<d1;k++){
 t=rc[fnem(k)];     // basic variables of m(k)
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



}

return 0;
}
void rpad(char *buf,int n){int i;for(i=strlen(buf);i<n;i++)buf[i]=' ';buf[i]=0;}
void lpad(char *buf,int n){int i,s;
s=strlen(buf);if(s<n){
 for(i=n-1;i>=n-s;i--)buf[i]=buf[i-(n-s)];
 for(i=0;i<n-s;i++)buf[i]=' ';
 buf[n]=0;
}}
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

// c22o vs 22o
//                R0F          CR0F          CR0C           R1F           R1C
//    R0F           0     -0.105498     -0.105498             0             0
//    R0C           0             0             0             0             0
//     R1           0      0.105498             0             0             0
