#include <bits/stdc++.h>
using namespace std;
char a[3000],b[3000],c[3000];
int D[3000][3000],s[300][300],n,m;
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
void DP(){
    int i,j;
    for(i=0;i<=256;++i)for(j=0;j<=256;++j)s[i][j]=1;
    for(i=0;i<=256;++i)s[i][i]=0;
    s[32][256]=s[256][32]=0;
    for(i=0;i<=n;++i){
        for(j=0;j<=m;++j){
            if(i&&j)D[i][j]=min(min(D[i-1][j-1]+s[a[i]][b[j]],D[i-1][j]+s[a[i]][256]),D[i][j-1]+s[256][b[j]]);
            else D[i][j]=max(i,j);
        }
    }
}
int main(){
    FILE *finA=fopen("codeA.txt","r"),*finB=fopen("codeB.txt","r");
    //freopen("output.txt","w",stdout);
    input(a,n,finA);
    input(b,m,finB);
    //puts(a+1);puts(b+1);
    DP();
    printf("Edit Dis : %d\n",D[n][m]);
    printf("%lf",(double)(max(n,m)-D[n][m])*100/max(n,m));
    return 0;
}
