#include "zjyList.h"
#include<stddef.h>
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