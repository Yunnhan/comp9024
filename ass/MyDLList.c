#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <ctype.h>
#include <string.h>


// data structures representing DLList
// data type for nodes
typedef struct DLListNode {
	int  value;  // value (int) of this list item
	struct DLListNode *prev;
	// pointer previous node in list
	struct DLListNode *next;
	// pointer to next node in list
} DLListNode;


//data type for doubly linked lists
typedef struct DLList{
	int  size;      // count of items in list
	DLListNode *first; // first node in list
	DLListNode *last;  // last node in list
} DLList;


// create a new DLListNode
DLListNode *newDLListNode(int it)
{
	DLListNode *new;
	new = malloc(sizeof(DLListNode));
	assert(new != NULL);
	new->value = it;
	new->prev = new->next = NULL;
	return new;
}


// create a new empty DLList
DLList *newDLList()
{
	struct DLList *L;
	L = malloc(sizeof (struct DLList));
	assert (L != NULL);
	L->size = 0;
	L->first = NULL;
	L->last = NULL;
	return L;
}

// create a DLList from a text file
// time complexity: No matter what the filename is, suppose there are N lines in the file(or to be read) and each line has M integers.
// the while loop will be executed N times , and the while loop nested in it will be executed  M times.
// Hence , the time complexity will be : O(MN)
DLList *CreateDLListFromFileDlist(const char *filename)
{
	DLList *L;
	L =  newDLList();
	DLListNode *now;
	DLListNode *new;
	char *c;
	char StrLine[1024];
	char *delim = " ;\n";
	if (strcmp(filename,"stdin")==0)
	{
		while(strlen(gets(StrLine))!=0)
		{
			c = strtok(StrLine,delim);
			if (L->first == NULL)
			{
				L->first = newDLListNode(atoi(c));
				L->size+=1;
				now = L->first;
				c = strtok(NULL, delim);
			}
			while (c != NULL)
			{
				new = newDLListNode(atoi(c));
				now->next = new;
				new->prev = now;
				L->size+=1;
				now = now->next;
				c = strtok(NULL, delim);
			}
		}
		L->last =now;
	}
	else
	{
		FILE *fp = fopen(filename,"r");
		while(!feof(fp))
		{
			fgets(StrLine,1024,fp);
			c = strtok(StrLine,delim);
			if (L->first == NULL){
				L->first = newDLListNode(atoi(c));
				L->size+=1;
				now = L->first;
				c = strtok(NULL, delim);
			}
			while (c != NULL)
			{
				new = newDLListNode(atoi(c));
				now->next = new;
				new->prev = now;
				L->size+=1;
				now = now->next;
				c = strtok(NULL, delim);
			}
		}
	L->last =now;
	fclose(fp);
	}
	return L;
}
// clone a DLList
// time complexity: Suppose the size of DLList u is N. The while loop will be executed for each node until reached the tail of the DLList u.
// So, total steps = O(5+7(N-1)) = O(N)
DLList *cloneList(struct DLList *u)
{
	DLList *L;
	L =  newDLList();
	DLListNode *current = u->first;
	DLListNode *new;
	while(current != NULL){
		if (L->first == NULL)
		{
			L->last = newDLListNode(current->value);
			L->first = L->last ;
			L->size+=1;
		}
		else
		{
			new =  newDLListNode(current->value);
			L->last->next = new;
			new->prev = L->last;
			L->last = L->last->next;
			L->size+=1;
		}
		current = current->next;
	}
	return L;
}

// check whether the value is already in the DLList u.
// time complexity :the while loop will be executed for each node to check whether the value of this node is equal to the parameter.
// So, total steps = O(3N) = O(N)
int isPresent(struct DLList *u, int data)
{
	DLListNode *current = u->first;
    while(current != NULL)
    {
        if( current->value == data )
            return 1;
        current = current->next;
    }
    return 0;
}

// compute the union of two DLLists u and v
// time complexity: Suppose the size of DLList u and DLList v are N and M separately. function cloneList() is O(N).
// The while loop will be executed M times for every node in v,and the function isPresent() is O(N).
// Hence , the time complexity will be : O(MN)
DLList *setUnion(struct DLList *u, struct DLList *v)
{
	DLList *L;
	DLListNode *new;
	DLListNode *ls2= v->first;
	L = cloneList(u);
	while (ls2 != NULL){
		if(isPresent(L,ls2->value)==0){
			new = newDLListNode(ls2->value);
			L->last->next = new;
			new->prev = L->last;
			L->last = L->last->next;
			L->size+=1;
		}
		ls2 = ls2->next;
	}
	return L;
}
// compute the intersection of two DLLists u and v
// time complexity: Suppose the size of DLList u and DLList v are N and M separately.while loop will be executed N times for every node in u,
// and function isPresent() is used in each loop.
// Hence , the time complexity will be : O(MN)
DLList *setIntersection(struct DLList *u, struct DLList *v)
{
	DLList *L;
	L =  newDLList();
	DLListNode *new;
	DLListNode *ls1= u->first;
	while(ls1 != NULL){
		if (isPresent(v,ls1->value)==1){
			if (L->first == NULL){
				L->last = newDLListNode(ls1->value);
				L->first = L->last ;
				L->size+=1;
			}
			else{
				new = newDLListNode(ls1->value);
				L->last->next = new;
				new->prev = L->last;
				L->last = L->last->next;
				L->size+=1;
			}
		}
		ls1 = ls1->next;
	}
	return L;
}
// free up all space associated with list
// time complexity: The while loop will be executed N times to free each node.
// So, total steps = O(4n)=O(n)
void freeDLList(struct DLList *L)
{
// put your code here
	assert(L!=NULL);
	DLListNode *current,*prev;
	current = L->first;
	while(current != NULL){
		prev = current;
		current = current->next;
		free(prev);
	}
	free(L);
}

// display items of a DLList
// time complexity analysis:suppose the size of DLList u is N,for the while loop,the time complexity is 2n,
// hence the time complexity will be O(n)
void printDLList(struct DLList *u)
{
 // put your code here
	DLListNode *node;
	node = u->first;

	while(node != u->last)
	{
		printf("%d\n",node->value);
		node = node->next;
	}
	printf("%d\n",u->last->value);

}

int main()
{

	 return 0;
}


