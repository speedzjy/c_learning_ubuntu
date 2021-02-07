#include "zjyList.h"
#include<stddef.h>
#include<stdlib.h>
#include<malloc.h>
// struct Node;
// typedef struct Node *PtrtoNode; //  PtrtoNode为指向节点的指针
// typedef PtrtoNode List;
// typedef PtrtoNode Position;

struct Node
{
        int       Element;
        Position  Next;
};

int IsEmpty( List L ){
        return L->Next == NULL ;
}

int IsLast(Position P,List L){  //L 在这个函数中并没有用到
        return P->Next==NULL;
}

// 返回X所在的位置指针，找不到则返回空
Position Find(int x,List L){
        Position P;
        P=L->Next;
        while(P!=NULL && P->Element!=x){
                P=P->Next;
        }
        return P;
}
//找出含有x的前驱元P
Position FindPrevious(int x,List L){
        Position P;
        P=L;
        while(P->Next!=NULL && P->Next->Element!=x){
                P=P->Next;
        }
        return P;
}

void  Delete(int x,List L){
        Position P,TmpCell;
        P=FindPrevious(x,L);
        if(IsLast(P,L)!=1){
                TmpCell=P->Next;
                P->Next=TmpCell->Next;
                free(TmpCell);
        }
        return;
}

// 后插入,L不需要用到
void insert(int x, List L, Position P){
        Position TmpCell;
        TmpCell=(Position)malloc(sizeof(Node));
        if(TmpCell==NULL)
                printf("Fetal error:ou of space!!!\n");
        TmpCell->Element=x;
        TmpCell->Next=P->Next;
        P->Next=TmpCell;
        return ;
}

void insertback(int x,List L){
        Position P,TmpCell;
        P=L;
        TmpCell=(Position)malloc(sizeof(Node));
        while(P->Next!=NULL)
                P=P->Next;
        P->Next=TmpCell;
        TmpCell->Element=x;
        TmpCell->Next=NULL;
        return;
}

void DeleteList(List L){
        Position P,Tmp;
        
        P=L->Next;
        L->Next=NULL;
        while(P!=NULL){
                Tmp=P->Next;
                free(P);
                P=Tmp;
        }
}

List CreateList(){
        List L;
        L=(List)malloc(sizeof(List));
        if(L==NULL)
                printf("Out of space!\n");
        L->Next=NULL;
        return L;
}

void printList(List L){
        if(L->Next==NULL){
                printf("List is empty!\n");
                return;
        }
        Position cur=L->Next;
        printf("List is:");
        while(cur!=NULL){
                printf("%d",cur->Element);
                if(cur->Next!=NULL)
                        printf("->");
                cur=cur->Next;
        }
        printf("\n");
        return;
}

List ReverseList( List L ){
        Position P;
        P=L->Next;
        Position CurrentPos, NextPos, PreviousPos;
        PreviousPos = NULL;
        CurrentPos = P;
        NextPos = P->Next;
        while( NextPos != NULL ){
                CurrentPos->Next = PreviousPos;
                PreviousPos = CurrentPos;
                CurrentPos = NextPos;
                NextPos = NextPos->Next;
        }
        CurrentPos->Next = PreviousPos;
        L->Next=CurrentPos;
        return L;
}












