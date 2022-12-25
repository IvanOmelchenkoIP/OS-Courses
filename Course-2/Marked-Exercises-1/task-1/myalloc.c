#include<stdio.h>
#include<stddef.h>
#include "mymalloc.h"


void initialize(){
 freeList->size=20000-sizeof(struct block);
 freeList->free=1;
 freeList->next=NULL;
}


void split(struct block *fitting_slot,size_t size){
 struct block *new=(void*)((void*)fitting_slot+size+sizeof(struct block));
 new->size=(fitting_slot->size)-size-sizeof(struct block);
 new->free=1;
 new->next=fitting_slot->next;
 fitting_slot->size=size;
 fitting_slot->free=0;
 fitting_slot->next=new;
}


void merge(){
 struct block *curr,*prev;
 curr=freeList;
 while((curr->next)!=NULL){
  if((curr->free) && (curr->next->free)){
   curr->size+=(curr->next->size)+sizeof(struct block);
   curr->next=curr->next->next;
  }
  prev=curr;
  curr=curr->next;
 }
}


void *MyMalloc(size_t noOfBytes){
 struct block *curr,*prev;
 void *result;

 //add your code below this line
 if (!freeList->size) initialize();
 curr = freeList;

 while (curr->next != NULL) {
   if (curr->size >= noOfBytes && curr->free) break;
   prev = curr;
   curr = curr->next;
 }

 if (curr->size == noOfBytes) {
   curr->free = 0;
   return curr;
 } else if (curr->size > noOfBytes) {
   split(curr, noOfBytes);
   curr->free = 0;
   return curr;
 } else {
   printf("Sorry. No sufficient memory to allocate\n");
   return NULL;
 }
 //add your code above this line
}


void MyFree(void* ptr){

 //add your code below this line
 if (ptr >= (void*)memory && ptr <= (void *)(memory + 20000)) {
   struct block *curr = ptr;
   curr->free = 1;
   merge();
 }
 //add your code above this line
}
