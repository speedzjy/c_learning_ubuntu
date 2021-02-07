#include<iostream>
#include<stddef.h>
#include "zjyList.h"
#include "zjyStack.h"
#include<stdlib.h>
#include<malloc.h>
using namespace std;

int Gcd(int M,int N);
long int Pow(long int X, int N);
bool IsEven(int num);

int main(){
        int M=0,N=0,W=0,ans=0;
        string str1,str2;
        str1="hello world!";
        str2="hello"" "" world!";
        cout<<"str1:"<<str1<<endl;
        cout<<"str2:"<<str2<<endl;
        List L;
        L=CreateList();
        cout<<"input number to insert"<<endl;

        cin>>M>>N>>W;
        insertback(M,L);
        insertback(N,L);
        
        insertback(W,L);
        printList(L);
        L=ReverseList(L);
        printList(L);
        //ans=Pow(M,N);
        cout<<"debug success"<<endl;
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