#include "zjyStack.h"
#include<stddef.h>
#include<stdlib.h>
#include<malloc.h>

// struct Node;
// typedef struct Node *PtrtoNode; 
// typedef PtrtoNode Stack;

struct Node{
        int Element;
        PtrtoNode Next;
};

int IsEmpty_S(Stack S){
        return S->Next==NULL;
}

Stack CreateStack(void){
        Stack S;
        S=(Stack)malloc(sizeof(Stack));
        if(S==NULL)
                printf("Out of space！\n");
        MakeEmpty(S);
        return S;
}

void MakeEmpty(Stack S){
        if (S==NULL)
                printf("Must usr CreateStack first");
        else{
                while(IsEmpty_S(S)!=1)
                        Pop(S);
        }
}

void Pop(Stack S){
        PtrtoNode FirstCell;
        if(IsEmpty_S(S))
                printf("S is Empty!\n");
        else{
                FirstCell=S->Next;
                S->Next=S->Next->Next;
                free(FirstCell);
        }
}

void Push(int x,Stack S){
        PtrtoNode NewCell;
        NewCell=(Stack)malloc(sizeof(Stack));
        if(NewCell==NULL)
                printf("Out of space!\n");
        else
        {
                NewCell->Element=x;
                NewCell->Next=S->Next;
                S->Next=NewCell;
        }
        return ;
}

int Top(Stack S){
        if(!IsEmpty_S(S))
                return S->Next->Element;
        printf("S is Empty!\n");
        return 0;      
}














