#ifndef _ZJYSTACK_H_ //如果没有引入头文件file.h

struct Node;
typedef struct Node *PtrtoNode; //  PtrtoNode为指向节点的指针
typedef PtrtoNode Stack;

int IsEmpty_S( Stack S);
Stack CreateStack(void);
void MakeEmpty(Stack S);
void Pop(Stack S);

#endif