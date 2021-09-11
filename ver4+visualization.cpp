#include <bits/stdc++.h>
#define WIDTH 3000
#define HEIGHT 450
#define HEADER 54
using namespace std;
unsigned char R1[HEIGHT][WIDTH],G1[HEIGHT][WIDTH],B1[HEIGHT][WIDTH],R2[HEIGHT][WIDTH],G2[HEIGHT][WIDTH],B2[HEIGHT][WIDTH],hd[HEADER];
char a[15][3500],t[15][3500];
int Q[3500][3500],s[300][300],P[3500][3500],n[15],ch,c;
int numb[13]={-1,21,42,64,85,109,130,153,174,197,219,252,264};
FILE *A=fopen("codeA.txt","r"),*B=fopen("codeB.txt","r"),*C=fopen("codeC.txt","r");
FILE *D=fopen("codeD.txt","r"),*E=fopen("codeE.txt","r");
FILE *fp1=fopen("A_B.bmp","wb"),*fp2=fopen("A_C.bmp","wb");
FILE *fp3=fopen("A_D.bmp","wb"),*fp4=fopen("A_E.bmp","wb");
FILE *fp5=fopen("B_C.bmp","wb"),*fp6=fopen("B_D.bmp","wb");
FILE *fp7=fopen("B_E.bmp","wb"),*fp8=fopen("C_D.bmp","wb");
FILE *fp9=fopen("C_E.bmp","wb"),*fp10=fopen("D_E.bmp","wb");
FILE *fp20=fopen("back.bmp","rb"),*num=fopen("number.bmp","rb");
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
	Token(TknKind k,const string& s,int d=0){kind=k;text=s;intVal=d;}
};
TknKind ctyp[256];
Token token;
struct KeyWord{
	const char *keyName;
	TknKind keyKind;
};
KeyWord KeyWdTbl[]={
	{"(",Lparen},{")",Rparen},{"if",If},{"else",Else},
	{"scanf",Scan},{"printf",Print},
	{"+",Plus},{"-",Minus},{"*",Multi},{"/",Divi},
	{"=",Assign},{",",Comma},{"==",Equal},{"!=",NotEq},
	{"<",Less},{"<=",LessEq},{">",Great},{">=",GreatEq},
	{"",END_list}
};
void initChTyp(){
	int i;
	for(i=0;i<256;++i)ctyp[i]=Others;
	for(i='0';i<='9';++i)ctyp[i]=Digit;
	for(i='A';i<='Z';++i)ctyp[i]=ctyp[i+32]=Letter;
	ctyp['(']=Lparen,ctyp[')']=Rparen;ctyp['<']=Less,ctyp['>']=Great;
	ctyp['+']=Plus,ctyp['-']=Minus,ctyp['*']=Multi,ctyp['/']=Divi;
	ctyp['_']=Letter,ctyp['=']=Assign;ctyp[',']=Comma,ctyp['"']=DblQ;
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
	int n1,n2,n3;
	for(i=0;i<=256;++i)for(j=0;j<=256;++j)s[i][j]=1;
	for(i=0;i<=256;++i)s[i][i]=0;
	s[32][256]=s[256][32]=0;
	for(i=0;i<=n[p];++i){
    	for(j=0;j<=n[q];++j){
        	if(i&&j){
        	    n1=Q[i-1][j-1]+s[a[p][i]][a[q][j]];
                n2=Q[i-1][j]+s[a[p][i]][256];
                n3=Q[i][j-1]+s[256][a[q][j]];
                Q[i][j]=min(min(n1,n2),n3);
                if(Q[i][j]==n1)P[i][j]=1;
                else if(Q[i][j]==n2)P[i][j]=2;
                else P[i][j]=3;
        	}
        	else{
                Q[i][j]=max(i,j);
                if(Q[i][j]==i)P[i][j]=2;
                else P[i][j]=3;
        	}
    	}
	}
}
void print(int i,int j,FILE *fp){putc(B1[i][j],fp);putc(G1[i][j],fp);putc(R1[i][j],fp);}
int main(){
	if(fp20==NULL){
	    puts("file not found");
        return 0;
	}
	initChTyp();
	int p;
	scanf("%d",&p);
	int i,j,i2,j2;
	parse(0,A);parse(1,B);parse(2,C);parse(3,D);parse(4,E);
	for(i=0;i<HEADER;i++){
        hd[i]=getc(fp20);
        getc(num);
        putc(hd[i],fp1),putc(hd[i],fp2);
        putc(hd[i],fp3),putc(hd[i],fp4);
        putc(hd[i],fp5),putc(hd[i],fp6);
        putc(hd[i],fp7),putc(hd[i],fp8);
        putc(hd[i],fp9),putc(hd[i],fp10);
	}
	for(i=0;i<HEIGHT;i++){
        for(j=0;j<WIDTH;j++){
            B2[i][j]=getc(num);
            G2[i][j]=getc(num);
            R2[i][j]=getc(num);
        }
    }
    for(i=0;i<p;i++){
        for(j=0;j<i;j++){
            DP(j,i);
            int y,x,l=0;
            y=n[j],x=n[i];
            while(P[y][x]){
                if(P[y][x]==3){
                    t[j][l]=' ',t[i][l]=a[i][x];
                    x--;
                }
                else if(P[y][x]==2){
                    t[i][l]=' ',t[j][l]=a[j][y];
                    y--;
                }
                else{
                    t[j][l]=a[j][y],t[i][l]=a[i][x];
                    y--,x--;
                }
                l++;
            }
            int rate=1000.0*((double)max(n[i],n[j])-Q[n[j]][n[i]])/(double)max(n[i],n[j]);
            int k=WIDTH/l;
            for(i2=0;i2<HEIGHT;i2++){
                for(j2=0;j2<WIDTH;j2++){
                    if(i2<60)B1[i2][j2]=G1[i2][j2]=R1[i2][j2]=255;
                    else if(j2>=k*l)B1[i2][j2]=G1[i2][j2]=R1[i2][j2]=255;
                    else if(t[j][l-j2/k-1]==t[i][l-j2/k-1])B1[i2][j2]=G1[i2][j2]=120,R1[i2][j2]=255;
                    else R1[i2][j2]=B1[i2][j2]=149,G1[i2][j2]=255;
                }
            }
            int yo=450;
            for(j2=numb[rate/100]+1;j2<numb[(rate/100)+1];j2++){
                for(i2=0;i2<29;i2++){
                    B1[i2][j2+yo-numb[rate/100]]=B2[i2+421][j2];
                    G1[i2][j2+yo-numb[rate/100]]=G2[i2+421][j2];
                    R1[i2][j2+yo-numb[rate/100]]=R2[i2+421][j2];
                }
            }
            yo+=numb[(rate/100)+1]-numb[rate/100];
            rate%=100;
            for(j2=numb[rate/10]+1;j2<numb[(rate/10)+1];j2++){
                for(i2=0;i2<29;i2++){
                    B1[i2][j2+yo-numb[rate/10]]=B2[i2+421][j2];
                    G1[i2][j2+yo-numb[rate/10]]=G2[i2+421][j2];
                    R1[i2][j2+yo-numb[rate/10]]=R2[i2+421][j2];
                }
            }
            yo+=numb[(rate/10)+1]-numb[rate/10];
            rate%=10;
            for(j2=numb[11]+1;j2<numb[12];j2++){
                for(i2=0;i2<29;i2++){
                    B1[i2][j2+yo-numb[11]]=B2[i2+421][j2];
                    G1[i2][j2+yo-numb[11]]=G2[i2+421][j2];
                    R1[i2][j2+yo-numb[11]]=R2[i2+421][j2];
                }
            }
            yo+=numb[12]-numb[11];
            for(j2=numb[rate]+1;j2<numb[rate+1];j2++){
                for(i2=0;i2<29;i2++){
                    B1[i2][j2+yo-numb[rate]]=B2[i2+421][j2];
                    G1[i2][j2+yo-numb[rate]]=G2[i2+421][j2];
                    R1[i2][j2+yo-numb[rate]]=R2[i2+421][j2];
                }
            }
            yo+=numb[rate+1]-numb[rate];
            for(j2=numb[10]+1;j2<numb[11];j2++){
                for(i2=0;i2<29;i2++){
                    B1[i2][j2+yo-numb[10]]=B2[i2+421][j2];
                    G1[i2][j2+yo-numb[10]]=G2[i2+421][j2];
                    R1[i2][j2+yo-numb[10]]=R2[i2+421][j2];
                }
            }
            yo+=numb[11]-numb[10];
            for(i2=0;i2<HEIGHT;i2++){
                for(j2=0;j2<WIDTH;j2++){
                    if(j==0&&i==1)print(i2,j2,fp1);
                    else if(j==0&&i==2)print(i2,j2,fp2);
                    else if(j==0&&i==3)print(i2,j2,fp3);
                    else if(j==0&&i==4)print(i2,j2,fp4);
                    else if(j==1&&i==2)print(i2,j2,fp5);
                    else if(j==1&&i==3)print(i2,j2,fp6);
                    else if(j==1&&i==4)print(i2,j2,fp7);
                    else if(j==2&&i==3)print(i2,j2,fp8);
                    else if(j==2&&i==4)print(i2,j2,fp9);
                    else if(j==3&&i==4)print(i2,j2,fp10);
                }
            }
        }
    }
}
