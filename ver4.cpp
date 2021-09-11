#include <bits/stdc++.h>
using namespace std;
char a[15][3000];
double Q[3000][3000],s[300][300];
int n[15],ch,c;
FILE *A=fopen("codeA.txt","r"),*B=fopen("codeB.txt","r"),*C=fopen("codeC.txt","r");
FILE *D=fopen("codeD.txt","r"),*E=fopen("codeE.txt","r");
enum TknKind{
	Lparen='A',Rparen,Plus,Minus,Multi,Divi,
	Assign,Comma,DblQ,
	Equal,NotEq,Less,LessEq,Great,GreatEq,
	If,Else,Scan,Print,Ident,IntNum,
	String,Letter,Digit,EofTkn,Others,paren,END_list
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
	{"{",paren},{"}",paren},
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
	ctyp['{']=ctyp['}']=paren;
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
	for(int i=0;KeyWdTbl[i].keyKind!=END_list;++i)
        if(s==KeyWdTbl[i].keyName)return KeyWdTbl[i].keyKind;
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
void parse(int p,FILE *A){
    int &l=n[p];
    char t;
    for(ch=' ',c=0,token=nextTkn(A);token.kind!=EofTkn;token=nextTkn(A)){
        a[p][++l]=(char)token.kind;
        if(a[p][l-1]=='F'&&a[p][l]=='F'){
            for(;ch!='\n'&&ch!=EOF;ch=fgetc(A));
            ch=fgetc(A);
            l-=2;
        }
        if(a[p][l-1]=='F'&&a[p][l]=='E'){
            for(t=ch,ch=fgetc(A);t!='*'||ch!='/';t=ch,ch=fgetc(A));
            ch=fgetc(A);
            l-=2;
        }
	}
}
void DP(int p,int q){
	int i,j;
	for(i=0;i<=256;++i)for(j=0;j<=256;++j)s[i][j]=1;
	for(i=0;i<=256;++i)s[i][i]=0;
	s[32][256]=s[256][32]=0;
	s[91][256]=s[256][91]=0.01;
	for(i=0;i<=n[p];++i){
    	for(j=0;j<=n[q];++j){
        	if(i&&j)Q[i][j]=min(min(Q[i-1][j-1]+s[a[p][i]][a[q][j]],Q[i-1][j]+s[a[p][i]][256]),Q[i][j-1]+s[256][a[q][j]]);
        	else Q[i][j]=max(i,j);
    	}
	}
}
int main(){
    freopen("output.txt","w",stdout);
	initChTyp();
	int p;
	double avg=0;
	scanf("%d",&p);
	int i,j;
	parse(0,A);parse(1,B);parse(2,C);parse(3,D);parse(4,E);
	for(i=0;i<p;i++){
        printf("%c\n",i+'A');
        puts("");
        for(j=0;j<p;j++){
            if(i==j)continue;
            DP(i,j);
            printf("%c and %c\n",i+'A',j+'A');
            printf("Edit Dis : %lf\n",Q[n[i]][n[j]]);
            printf("%.1f%%\n",((double)max(n[i],n[j])-Q[n[i]][n[j]])*100/(double)max(n[i],n[j]));
            avg+=(((double)max(n[i],n[j])-Q[n[i]][n[j]])*100/(double)max(n[i],n[j]));
            memset(Q,0,sizeof Q);
            puts("");
        }
	}
	avg=avg/(i*(i-1));
	printf("average : %.1f%%\n",avg);
	return 0;
}
