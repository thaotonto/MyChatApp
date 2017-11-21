#ifndef LISTUSER_H
#define LISTUSER_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>


typedef struct node_{
   int data;
   char key[20];
	
   struct node_ *next;
   struct node_ *prev;
} node;

bool isEmpty();
int length();
void displayForward();
void insertLast(char* key, int data);
void deleteNode(node* del);
void deleteNodeWithKey(char *key);
char* deleteNodeWithValue(int data);

#endif