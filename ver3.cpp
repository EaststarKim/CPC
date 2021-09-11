#include <bits/stdc++.h>
using namespace std;
char a[3000],b[3000];
int D[3000][3000],s[300][300];
int n,m,ch,c;
FILE *A=fopen("codeA.txt","r"),*B=fopen("codeB.txt","r");
enum TknKind{
    Lparen='A',Rparen,Plus,Minus,Multi,Divi,
    Assign,Comma,DblQ,
    Equal,NotEq,Less,LessEq,Great,GreatEq,
    If,Else,Scan,Print,Ident,IntNum,
    String,Letter,Digit,EofTkn,Others,END_list
};
struct Token{
    TknKind kind;
    string text;
    int intVal;
    Token(){kind=Others;text="";intVal=0;}
    Token(TknKind k,const string& s,int d=0){
        kind=k;text=s;intVal=d;
    }
};
TknKind ctyp[256];
Token token;
struct KeyWord{
    const char *keyName;
    TknKind keyKind;
};
KeyWord KeyWdTbl[]={
    {"if",If},{"else",Else},
    {"scanf",Scan},{"printf",Print},
    {"(",Lparen},{")",Rparen},
    {"+",Plus},{"-",Minus},
    {"*",Multi},{"/",Divi},
    {"=",Assign},{",",Comma},
    {"==",Equal},{"!=",NotEq},
    {"<",Less},{"<=",LessEq},
    {">",Great},{">=",GreatEq},
    {"",END_list}
};
void initChTyp(){
    int i;
    for(i=0;i<256;++i)ctyp[i]=Others;
    for(i='0';i<='9';++i)ctyp[i]=Digit;
    for(i='A';i<='Z';++i)ctyp[i]=Letter;
    for(i='a';i<='z';++i)ctyp[i]=Letter;
    ctyp['(']=Lparen,ctyp[')']=Rparen;
    ctyp['<']=Less,ctyp['>']=Great;
    ctyp['+']=Plus,ctyp['-']=Minus;
    ctyp['*']=Multi,ctyp['/']=Divi;
    ctyp['_']=Letter,ctyp['=']=Assign;
    ctyp[',']=Comma,ctyp['"']=DblQ;
}
int nextCh(FILE *fin){
    if(c==EOF)return c;
    if((c=fgetc(fin))==EOF)fclose(fin);
    return c;
}
bool is_ope2(int c1,int c2){
    char s[]="   ";
    if(c1=='|0'||c2=='|0')return false;
    s[1]=c1;s[2]=c2;
    return strstr(" <= >= == != ",s)!=NULL;
}
TknKind get_kind(const string& s){
    for(int i=0;KeyWdTbl[i].keyKind!=END_list;++i)if(s==KeyWdTbl[i].keyName)return KeyWdTbl[i].keyKind;
    if(ctyp[s[0]]==Letter)return Ident;
    if(ctyp[s[0]]==Digit)return IntNum;
    return Others;
}
Token nextTkn(FILE *fin){
    TknKind kd;
    int ch0,num=0;
    string txt="";
    while(isspace(ch))ch=nextCh(fin);
    if(ch==EOF)return Token(EofTkn,txt);
    switch(ctyp[ch]){
    case Letter:
        for(;ctyp[ch]==Letter||ctyp[ch]==Digit;ch=nextCh(fin))txt+=ch;
        break;
    case Digit:
        for(num=0;ctyp[ch]==Digit;ch=nextCh(fin))num=num*10+ch-'0';
        return Token(IntNum,txt,num);
    case DblQ:
        for(ch=nextCh(fin);ch!=EOF&&ch!='\n'&&ch!='"';ch=nextCh(fin))txt+=ch;
        ch=nextCh(fin);
        return Token(String,txt);
    default:
        txt+=ch;ch0=ch;ch=nextCh(fin);
        if(is_ope2(ch0,ch))txt+=ch,ch=nextCh(fin);
    }
    kd=get_kind(txt);
    return Token(kd,txt);
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
    initChTyp();
    for(ch=' ',token=nextTkn(A);token.kind!=EofTkn;token=nextTkn(A))a[++n]=(char)token.kind;
    for(ch=' ',c=0,token=nextTkn(B);token.kind!=EofTkn;token=nextTkn(B))b[++m]=(char)token.kind;
    puts(a+1);
    puts(b+1);
    DP();
    printf("Edit Dis : %d\n",D[n][m]);
    printf("%lf",(double)(max(n,m)-D[n][m])*100/max(n,m));
    return 0;
}
