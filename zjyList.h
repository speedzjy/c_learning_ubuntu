#ifndef _ZJYLIST_H_ //如果没有引入头文件file.h

struct Node;
typedef struct Node *PtrtoNode; //  PtrtoNode为指向节点的指针
typedef PtrtoNode List;
typedef PtrtoNode Position;

int IsEmpty( List );
int IsLast(Position ,List  );
Position Find(int ,List );
Position FindPrevious(int xList );
void  Delete(int ,List );
void insert(int x, List L, Position P);
List CreateList();
void printList(List L);
void insertback(int x,List L);
List ReverseList( List L );

#endif














