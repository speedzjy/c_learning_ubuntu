#include<iostream>
using namespace std;

int Gcd(int M,int N);
long int Pow(long int X, int N);
bool IsEven(int num);

int main(){
        int M=0,N=0,ans=0;
        cout<<"hello world!"<<endl;
        cin>>M>>N;
        ans=Pow(M,N);
        cout<<ans<<endl;
        return 0;
}

long int Pow(long int X, int N){
        if(N==0)
                return 1;
        if(N==1)
                return X;
        if(IsEven(N))
                return Pow(X*X,N/2);
        else
                return Pow(X*X,N/2)*X;
}

bool IsEven(int num){
        if(num % 2 == 0)
                return 1;
        else
                return 0; 
}

int Gcd(int M,int N){
        int Rem;
        //输入M>N,若M<N，则循环第一次时M，N互换
        while(N>0){               
                Rem=M%N;
                M=N;
                N=Rem;
        }
        return M;
}