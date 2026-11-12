/* Tanvir Ahmed CS1 - Lab 3 "Linked Lists" */

/* Your job is to complete writing the reverse(..) and insertToPlace(..) functions */

/* for your submission on codegrade, upload both main.c and main.h files */

#include<stdio.h>
#include<stdlib.h>

#include "main.h"

//this function takes an item and insert it in the linked list pointed by root.
node*  insert_front(node *root, int item)
{
	node *temp;
	//create a new node and fill-up the node
	temp= (node *) malloc(sizeof(node));
	temp->data=item;
	temp->next=NULL;
	if(root==NULL) //if there is no node in the linked list
        root=temp;
    else //there is an existing linked list, so put existing root after temp
    {
        temp->next = root; //put the existing root after temp
        root = temp; //make the temp as the root!
    }
  return root;

}

void display(node* t)
{
  printf("\nPrinting your linked list.......");

	while(t!=NULL)
	{
		printf("%d ",t->data);
		t=t->next;
	}

}

//complete this function
node* reverse(node* head) {
 // return only head if list empty or only one node
 if(head==NULL || head->next==NULL)
     return head;

 node* mainList = head->next;
 node* reversedList = head;
 reversedList=NULL;

 while (mainList!=NULL) {
     node* temp = mainList;
     mainList = mainList->next;

     temp->next = reversedList;
     reversedList=temp;
 }
 return reversedList;
}

//complete this function
void insertToPlace(node* list, int val, int place) 
{
	
}


int main()
{
	node *root=NULL; //very important line. Otherwise all function will fail
	node *t;
	int ch,ele,v, del;
	while(1)
	{
		printf("\nMenu: 1. insert at front, 2. reverse list 3. Insert to place 0. exit: ");
	    scanf("%d",&ch);
		if(ch==0)
		{
			printf("\nGOOD BYE>>>>\n");
			break;
		}
		if(ch==1)
		{
			printf("\nEnter data (an integer): ");
			scanf("%d",&ele);
			root = insert_front(root, ele);

            display(root);

		}
		if(ch==2)
		{
			root = reverse(root);
			printf("List reversed.\n");
			display(root);

		}
		if(ch==3)
		{
		    int place;
			printf("\nEnter data (an integer) and place (>1) separated by space: ");
			scanf("%d %d",&ele, &place);
			insertToPlace(root, ele, place);

            display(root);
		}

	}
  return 0;
}
