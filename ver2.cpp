#include <bits/stdc++.h>
using namespace std;
char a[3000],b[3000],c[3000],d[3000],d2[3000],hd[100][100];
int D[3000][3000],s[300][300],n,m,l1,l2;
void input(char *a,int &n,FILE *fin){
    int i,len;
    n=0;
    for(;;){
        memset(c,0,sizeof c);
        fgets(c,sizeof c,fin);
        len=strlen(c);
        if(!len)break;
        for(i=0;c[i]==32;++i);
        for(;i<len-1;++i)a[++n]=c[i];
    }
}
int same(char *a,int s,int e,char *p){
    int i;
    for(i=0;i<=e-s;i++)if(a[i+s]!=p[i])return 0;
    return 1;
}
void parse(char *a,char *d,int n,int &l){
    int i=1,j=1,hdk=0;
    memset(hd,0,sizeof hd);
    for(;j<=n;){
        if(same(a,j,j+7,"#include")){
            j+=8;
            int re=0;
            while(a[j]!='<')j++;
            j++;
            while(a[j]!='>'){
                hd[hdk][re++]=a[j];
                j++;
            }
            hdk++;j++;
        }
        else if(same(a,j,j+5,"scanf(")){
            d[i++]='I';d[i++]='N';d[i++]='P';d[i++]='U';d[i++]='T';
            j+=5;
        }
        else if(same(a,j,j+4,"for")){
            d[i++]='L';d[i++]='O';d[i++]='O';d[i++]='P';
            j+=3;
        }
        else if(same(a,j,j+5,"while(")){
            d[i++]='L';d[i++]='O';d[i++]='O';d[i++]='P';
            j+=3;
        }
        else if(same(a,j,j+7,"printf(")){
            d[i++]='O';d[i++]='U';d[i++]='T';d[i++]='P';d[i++]='U';d[i++]='T';
            j+=6;
        }
        else if(same(a,j,j+4,"char ")){
            d[i++]='C';d[i++]='H';d[i++]='A';d[i++]='R';
            j+=4;
        }
        else if(same(a,j,j+3,"int ")){
            d[i]='I';d[i+1]='N';d[i+2]='T';
            i+=3;j+=3;
        }
        else if(same(a,j,j+2,"if(")){
            d[i]='I';d[i+1]='F';
            i+=2;j+=2;
        }
        else if(same(a,j,j,"?")){
            d[i]='I';d[i+1]='F';
            i+=2;j++;
        }
        else{
            d[i]=a[j];
            i++,j++;
        }
    }
    l=i-1;
}
void DP(){
    int i,j;
    for(i=0;i<=256;++i)for(j=0;j<=256;++j)s[i][j]=1;
    for(i=0;i<=256;++i)s[i][i]=0;
    s[32][256]=s[256][32]=0;
    for(i=0;i<=l1;++i){
        for(j=0;j<=l2;++j){
            if(i&&j)D[i][j]=min(min(D[i-1][j-1]+s[d[i]][d2[j]],D[i-1][j]+s[d[i]][256]),D[i][j-1]+s[256][d2[j]]);
            else D[i][j]=max(i,j);
        }
    }
}
int main(){
    FILE *finA=fopen("codeA.txt","r"),*finB=fopen("codeB.txt","r");
    input(a,n,finA);
    input(b,m,finB);
    parse(a,d,n,l1);
    parse(b,d2,n,l2);
    DP();
    printf("Edit Dis : %d\n",D[l1][l2]);
    printf("%lf\n",(double)(max(l1,l2)-D[l1][l2])*100/max(l1,l2));
    return 0;
}
