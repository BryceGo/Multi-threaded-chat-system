//Bryce Golamco
//bgolamco@sfu.ca


#include "Assignment1.h"
#include <stdio.h>



#define ARRAY_LENGTH 100 //This is the number of Nodes in the array
#define LIST_LENGTH 10   // This is the number of Heads in the array
LIST array_of_lists[LIST_LENGTH];
Node node_array[ARRAY_LENGTH];

int total_nodes_size = 0;
int list_head_size = 0;
int start = 0;

int TotalHeads(){
    return list_head_size;
}

int TotalNodes(){
    return total_nodes_size;
}







LIST *ListCreate(){
    int i = 0;
	LIST *templist = NULL;

	if(start == 0){ //First time List Create is ever called
        for(i = 0; i<LIST_LENGTH; i++){
            array_of_lists[i].in_use = 0;
        }
	}
    start = 1;

    if(list_head_size >= LIST_LENGTH){
        return NULL;
    }

    for(i = 0; i < LIST_LENGTH; i++){ //Sets a free node
        if(array_of_lists[i].in_use == 0){
            templist = &array_of_lists[i];
            templist->in_use = 1;
            break;
        }
    }

	templist->head = NULL;
	templist->tail = NULL;
	templist->current = NULL;
	templist->size = 0;
    templist->before_start = 0;
    templist->beyond_end = 0;

    list_head_size++;
	return templist;
}

int ListCount(LIST *x){
    return (x->size );
}

void * ListFirst(LIST *x){
    x->current = x->head;
    x->beyond_end = 0;
    x->before_start = 0;
    return x->head;
}

void * ListLast(LIST *x){
    x->current = x->tail;
    x->beyond_end = 0;
    x->before_start = 0;
    return x->tail;
}

void * ListNext(LIST *x){
    if(x->current != NULL){
        x->current = x->current->next;
        if(x->current == NULL){
            x->beyond_end = 1;
            x->before_start = 0;
        }else{
            x->beyond_end = 0;
            x->before_start = 0;
        }
    }
    return x->current;
}

void * ListPrev(LIST *x){
    if(x->current != NULL){
        x->current = x->current->before;
        if(x->current == NULL){
            x->beyond_end = 0;
            x->before_start = 1;
        }else{
            x->beyond_end = 0;
            x->before_start = 0;
        }
    }
    return x->current;
}

void * ListCurr(LIST *x){
    return x->current;
}

int ListAdd(LIST *list, void * item){
    Node * temp = NULL;
    Node * temp_node = NULL;

    if(list == NULL){
        return -1;
    }
    if(total_nodes_size >= ARRAY_LENGTH){
        return -1;
    }

    temp_node = &node_array[total_nodes_size];

    if(list->current != NULL){
        temp = list->current->next;
        list->current->next = temp_node;
        temp_node->item = item;
        temp_node->before = list->current;
        temp_node->next = temp;
        if(temp != NULL){
            temp->before = temp_node;
        }
        if(list->current == list->tail){
            list->tail = list->current->next;
        }
        list->current = list->current->next;

    }else if (list->size == 0){
        temp_node->item = item;
        list->current = temp_node;
        list->head = temp_node;
        list->tail = temp_node;
    }else if(list->before_start == 1){
        list->before_start = 0;
        temp = temp_node;
        temp->item = item;
        temp->before = NULL;
        temp->next = list->head;
        list->head->before = temp;
        list->head = temp;
        list->current = temp;
    }else if (list->beyond_end == 1){
        list->beyond_end = 0;
        temp = temp_node;
        temp->item = item;
        temp->before = list->tail;
        temp->next = NULL;
        list->tail->next = temp;
        list->tail = temp;
        list->current = temp;
    }else{
        return -1;
    }
    temp_node->list_head = list;
    total_nodes_size++;
    list->size++;
    return 0;
}

int ListInsert(LIST *list, void * item){
    Node * temp = NULL;
    Node * temp_node = NULL; //The new node to be added

    if(list == NULL){
        return -1;
    }
    if(total_nodes_size >= ARRAY_LENGTH){ //means the node array is full
        return -1;
    }

    temp_node = &node_array[total_nodes_size];


    if(list->current != NULL){
        temp = list->current->before;

        temp_node->item = item;
        temp_node->before = temp;
        temp_node->next = list->current;
        list->current->before = temp_node;

        if(temp != NULL){
            temp->next = temp_node;
        }

        if(list->current == list->head){ //setting The pointers of the head and current pointer
            list->head = temp_node;
        }
        list->current = temp_node;

    }else if(list->size == 0){
        temp_node->item = item;
        list->current = temp_node;
        list->head = temp_node;
        list->tail = temp_node;
    }else if(list->before_start == 1){
        list->before_start = 0;
        temp = temp_node;
        temp->item = item;
        temp->before = NULL;
        temp->next = list->head;
        list->head->before = temp;
        list->head = temp;
        list->current = temp;
    }else if (list->beyond_end == 1){
        list->beyond_end = 0;
        temp = temp_node;
        temp->item = item;
        temp->before = list->tail;
        temp->next = NULL;
        list->tail->next = temp;
        list->tail = temp;
        list->current = temp;
    }else{
        return -1;
    }

    temp_node->list_head = list;
    total_nodes_size++;
    list->size++;
    return 0;
}

int ListAppend(LIST *list, void * item){
    Node * temp = NULL;


    if(list == NULL){
        return -1;
    }
    if(total_nodes_size >= ARRAY_LENGTH){ //if full
        return -1;
    }

    temp = &node_array[total_nodes_size];



    if(list->tail != NULL){
        temp->item = item;
        temp->before = list->tail;
        list->tail->next = temp;
        temp->next = NULL;
        list->tail = temp;
        list->current = temp;
        list->before_start = 0;
        list->beyond_end = 0;

    }else if (list->size == 0){ //Checks if list is empty
        temp->item = item;
        list->head = temp;
        list->current = temp;
        list->tail = temp;
        list->before_start = 0;
        list->beyond_end = 0;
    }else{
        return -1;
    }


temp->list_head = list;
total_nodes_size++;
list->size++;
return 0;
}

int ListPrepend(LIST *list,void * item){

    Node * temp = NULL;

    if(list == NULL){
        return -1;
    }

    if(total_nodes_size >= ARRAY_LENGTH){
        return -1;
    }

    temp = &node_array[total_nodes_size]; //New node to be added;


    if(list->head != NULL){
        temp->item = item;
        temp->next = list->head;
        temp->before = NULL;
        list->head->before = temp;

        list->head = temp;
        list->current = temp;
        list->before_start = 0;
        list->beyond_end = 0;

    }else if(list->size == 0){
        temp->item = item;
        list->head = temp;
        list->current = temp;
        list->tail = temp;
        list->before_start = 0;
        list->beyond_end = 0;
    }else{
        return -1;
    }


temp->list_head = list;
total_nodes_size++;
list->size++;
return 0;
}

void *ListRemove(LIST *list){
    Node * tempn = NULL; // temporary pointer pointing to the next node (if there is any)
    Node * tempb = NULL; // Temporary pointer pointing to the node before it (if there is any)
    Node * temp = NULL;
    Node * temp_node = NULL;
    void * tempitem = NULL;

    if(list == NULL){
        return NULL;
    }


    if(list->current != NULL){
        list->size--;
        total_nodes_size--;
        tempb = list->current->before;
        tempn = list->current->next;
        temp = list->current;
        temp_node = &node_array[total_nodes_size];

        if((tempb == NULL) && (tempn == NULL)) {
            list->current = NULL;
            list->head = NULL;
            list->tail = NULL;

            tempitem = temp->item;
            temp->item = NULL;
            temp->next = NULL;
            temp->before = NULL;
            list->before_start = 0;
            list->beyond_end = 0;
            return tempitem;
        }else if(tempb == NULL){
            list->head = temp->next;
            list->current = list->current->next;
            list->current->before = NULL;


            tempitem = temp->item;

            if(temp == temp_node){
                temp->next = NULL;
                temp->before = NULL;
                temp->item = NULL;
            }else{
                temp->next = temp_node->next;
                temp->before = temp_node->before;
                temp->item = temp_node->item;
            }

            if(temp_node->next != NULL){
                temp_node->next->before = temp;
            }
            if(temp_node->before != NULL){
                temp_node->before->next = temp;
            }
            if(temp_node == temp_node->list_head->tail){
                temp_node->list_head->tail = temp;
            }

            temp_node->next = NULL;
            temp_node->before = NULL;
            temp_node->item = NULL;
            list->before_start = 0;
            list->beyond_end = 0;

            return tempitem;
        }else if(tempn == NULL){
            list->current = list->current->before; // sets to the node before it instead of next
            list->tail = temp->before;
            list->tail->next = NULL;


            tempitem = temp->item;
            if(temp == temp_node){
                temp->next = NULL;
                temp->before = NULL;
                temp->item = NULL;
            }else{
                temp->next = temp_node->next;
                temp->before = temp_node->before;
                temp->item = temp_node->item;
            }

            if(temp_node->next != NULL){
                temp_node->next->before = temp;
            }
            if(temp_node->before != NULL){
                temp_node->before->next = temp;
            }
            if(temp_node == temp_node->list_head->head){
                temp_node->list_head->head = temp;
            }

            temp_node->next = NULL;
            temp_node->before = NULL;
            temp_node->item = NULL;
            list->before_start = 0;
            list->beyond_end = 0;

            return tempitem;
        }else{
            tempb->next = tempn;
            tempn->before = tempb;

            list->current = list->current->next;
            tempitem = temp->item;

            if(temp == temp_node){
                temp->next = NULL;
                temp->before = NULL;
                temp->item = NULL;
            }else{
                temp->next = temp_node->next;
                temp->before = temp_node->before;
                temp->item = temp_node->item;
            }

            if(temp_node->next != NULL){
                temp_node->next->before = temp;
            }
            if(temp_node->before != NULL){
                temp_node->before->next = temp;
            }
            if(temp_node == temp_node->list_head->head){
                temp_node->list_head->head = temp;
            }
            if(temp_node == temp_node->list_head->tail){
                temp_node->list_head->tail = temp;
            }

            temp_node->next = NULL;
            temp_node->before = NULL;
            temp_node->item = NULL;
            list->before_start = 0;
            list->beyond_end = 0;

            return tempitem;
        }
    }else{
        return NULL;
    }
}

void ListConcat(LIST * list1, LIST *list2){

    if(list1 == NULL || list2 == NULL){
        return;
    }

    ListLast(list1);
    ListFirst(list2);

    if((list1->tail != NULL) && (list2->head != NULL)){
        list2->head->before = list1->tail;
        list1->tail->next = list2->head;
        list1->tail = list2->tail;
    }else if(list1->tail == NULL){ // This means list1 is empty
        list1 = list2;
    }

    if(list2 != NULL){
        list2->in_use = 0;
    }
    list2 = NULL;
    list_head_size--;

    return;
}

void ListFree(LIST * list, void (*itemFree)(void *)){
    void * temp = NULL;

    ListLast(list);
    list->tail = NULL;
    while(list->current != NULL){
	temp = ListRemove(list); 
        if(temp != NULL){
		(*itemFree)(temp);
	}
        
    }

    list->in_use = 0;
    list_head_size--;
    list = NULL;
    return;
}

void *ListTrim(LIST * list){
    void * item;
    ListLast(list);
    item = ListRemove(list);
    ListLast(list);
    return item;
}

void *ListSearch(LIST *list, int (*comparator)(void*, void*), void *comparisonArg){
	int i =0;
	if(list->current == NULL){
		return NULL;
	}
	
	do{
		if((*comparator)(list->current->item,comparisonArg) == 0){
			return list->current->item;		
		}	
	}while(ListNext(list) != NULL);
return NULL;
}


