//Bryce Golamco
//bgolamco@sfu.ca









#ifndef ASSIGNMENT1_H
#define ASSIGNMENT1_H


typedef struct node{
   void * item;
   struct node * next;
   struct node * before;
   struct list * list_head;
}Node;


typedef struct list{
    Node * head;
    Node * tail;
    Node * current;
    int before_start;
    int beyond_end;
    int in_use;
    int size;
    //Node list_array[ARRAY_LENGTH];
}LIST;

//LIST *list_heads[LIST_LENGTH];


LIST *ListCreate(); // Makes a new empty list, and returns its reference on success. Returns a NULL pointer on failure
int ListCount(LIST *list); // returns the number of items in list
void *ListFirst(LIST *list); // returns a pointer to the first item in list and makes the first item the current item

void *ListLast(LIST *list);
void *ListNext(LIST *list);
void *ListPrev(LIST *list);
void *ListCurr(LIST *list);
int ListAdd(LIST *list,void *item);
int ListInsert(LIST *list,void *item);
int ListAppend(LIST *list,void *item);
int ListPrepend(LIST *list,void *item);
void *ListRemove(LIST *list);
void ListConcat(LIST *list1,LIST *list2);
void ListFree(LIST *list,void (*itemFree)(void *));
void *ListTrim(LIST *list);
void *ListSearch(LIST *list, int (*comparator)(void*, void*), void *comparisonArg);

int TotalNodes();//returns the number of nodes
int TotalHeads();// returns the number of heads

#endif // ASSIGNMENT1_H
