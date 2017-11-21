#include "listUserOnline.h"

node *head = NULL;

node *last = NULL;

node *current = NULL;
//is list empty
bool isEmpty() {
   return head == NULL;
}

int length() {
   int length = 0;
   node *current;
	
   for(current = head; current != NULL; current = current->next){
      length++;
   }
	
   return length;
}

//display the list in from first to last
void displayForward() {

   //start from the beginning
    node *ptr = head;
	
   //navigate till the end of the list
   printf("[ ");
	
   while(ptr != NULL) {        
      printf("(%s,%d)",ptr->key,ptr->data);
      ptr = ptr->next;
   }
	
   printf(" ]\n");
}

//insert link at the last location
void insertLast(char* key, int data) {
       
    node *link = ( node*) malloc(sizeof( node));
    strcpy(link->key,key);
    link->data = data;
    link->prev=NULL;
    link->next=NULL;
   if(isEmpty()) {
      head = last = link;
   } else {
        link->prev = last;
        last->next = link ;
		link->next= NULL;
		last=link;
   }
}

void deleteNode(node* del) {
    /* base case */
    if (head == NULL || del == NULL)
        return;
 
    /* If node to be deleted is head node */
    if (head == del)
        head = del->next;
 
    /* Change next only if node to be deleted is NOT 
       the last node */
    if (del->next != NULL)
        del->next->prev = del->prev;
 
    /* Change prev only if node to be deleted is NOT 
       the first node */
    if (del->prev != NULL)
        del->prev->next = del->next;
 
    /* Finally, free the memory occupied by del*/
    free(del);
}

void deleteNodeWithKey(char *key) {
    if (head == NULL)
        return;
 
    node* current = head;
    int i;
 
    for (int i = 1; current != NULL; i++) {
        if (strcmp(current->key, key) != 0) {
            current = current->next;
        } else {
            break;
        }
    }
    if (current == NULL)
        return;
 
    deleteNode(current);
}

char* deleteNodeWithValue(int data) {
    char* key;
    if (head == NULL)
        return "UNAUTHORIZED";
 
    node* current = head;
    int i;
 
    for (int i = 1; current != NULL; i++) {
        if (current->data != data) {
            current = current->next;
        } else {
            strcpy(key, current->key);
            break;
        }
    }
    if (current == NULL)
        return "UNAUTHORIZED";
 
    deleteNode(current);
    return key;
}