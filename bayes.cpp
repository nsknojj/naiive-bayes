#include<cstdio>
#include<cstring>
#include<iostream>
#include<cmath>
#include<algorithm>
#include<cstdlib>
using namespace std;

#define sqr(x) ((x)*(x))

const int SIZE=784,
    LIM=1,
    MAXN=40010;

const double eps=0.01;

FILE *Ftrain=fopen("train.csv","r");
FILE *Ftest=fopen("test.csv","r");
FILE *out=fopen("result.txt","w");

struct number
{
    bool data[SIZE];
    void read(FILE *in){
        int a;
        for(int i=0;i<SIZE;i++){
            fscanf(in,"%d",&a);
            data[i]=a>LIM;
        }
    }
}   a[MAXN],b;

int sum[10],num[MAXN];
double x[10][SIZE],sig[10][SIZE],p[10][SIZE][2];
int n;

double gauss(double x,double sig,double u)      //计算高斯函数的值 
{
    if(sig<eps) return 0;
    return -log(sqrt(2*M_PI))-log(sig)-sqr(x-u)/(2*sqr(sig));
}

void train()                                    //用训练集进行训练 
{
    while(fscanf(Ftrain,"%d",&num[n])>0){
        sum[num[n]]++;        
        a[n++].read(Ftrain);
    }
    for(int i=0;i<10;i++)
        for(int j=0;j<n;j++)
            if(num[j]==i)
                for(int k=0;k<SIZE;k++)
                    x[i][k]+=a[j].data[k];
    for(int i=0;i<10;i++)
        for(int k=0;k<SIZE;k++)
            x[i][k]/=1.*sum[i];                 //求平均值 
    for(int i=0;i<10;i++)
        for(int j=0;j<n;j++)
            if(num[j]==i)
                for(int k=0;k<SIZE;k++)
                    sig[i][k]+=sqr(a[j].data[k]-x[i][k]);
    for(int i=0;i<10;i++)
        for(int k=0;k<SIZE;k++){                //求标准差并且预先求出0和1的概率 
            sig[i][k]/=1.*sum[i];            
            sig[i][k]=sqrt(sig[i][k]);
            sig[i][k]+=eps;
//            p[i][k][0]=gauss(0,sig[i][k],x[i][k]);
            p[i][k][0]=log(1-x[i][k]);
//            p[i][k][1]=gauss(1,sig[i][k],x[i][k]);
            p[i][k][1]=log(x[i][k]);
        }
}

void test()                                 //测试集 
{
    cerr<<2<<endl;
    fprintf(out,"Resut :\n");
    int rate=0,tot=0,ans;
    while(fscanf(Ftest,"%d",&ans)>0){
//        cerr<<tot<<endl;
        tot++;
        b.read(Ftest);
        int c=-1;
        double ma=0;
        for(int i=0;i<10;i++){              //枚举数字0～9 
            double pr=sum[i]*1./n;
            for(int j=0;j<SIZE;j++)
                pr+=p[i][j][b.data[j]];     //计算f值 
            if(c==-1||pr>ma){
                ma=pr;
                c=i;
            }
        }
        fprintf(out,"%d ",c);
        rate+=c==ans;
    }
    fprintf(out,"\n assessment : %.2lf%%\n",rate*1./tot*100);
}

int main()
{
    train();
    cerr<<1<<endl;
    test();
    fclose(out);
    fclose(Ftrain);
    fclose(Ftest);
    return 0; 
}
