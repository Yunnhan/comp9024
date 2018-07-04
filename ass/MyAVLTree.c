#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#define get_height(node) ((node)==NULL?-1:(node)->height)
#define max(a,b) (a>b?a:b)
// all the basic data structures and functions are included in this template
// you can add your own auxiliary functions as you like

// data type for avl tree nodes
typedef struct AVLTreeNode {
	int key; //key of this item
	int  value;  //value (int) of this item
	int height; //height of the subtree rooted at this node
	struct AVLTreeNode *parent; //pointer to parent
	struct AVLTreeNode *left; //pointer to left child
	struct AVLTreeNode *right; //pointer to right child
} AVLTreeNode;

//data type for AVL trees
typedef struct AVLTree{
	int  size;      // count of items in avl tree
	AVLTreeNode *root; // root
} AVLTree;

// create a new AVLTreeNode
AVLTreeNode *newAVLTreeNode(int k, int v )
{
	AVLTreeNode *new;
	new = malloc(sizeof(AVLTreeNode));
	assert(new != NULL);
	new->key = k;
	new->value = v;
	new->height = 0; // height of this new node is set to 0
	new->left = NULL; // this node has no child
	new->right = NULL;
	new->parent = NULL; // no parent
	return new;
}

// create a new empty avl tree
AVLTree *newAVLTree()
{
	AVLTree *T;
	T = malloc(sizeof (AVLTree));
	assert (T != NULL);
	T->size = 0;
	T->root = NULL;
	return T;
}

//define single  rotation
AVLTreeNode *LLRotate(AVLTreeNode *T1)
{
	assert(T1!=NULL);
	AVLTreeNode *T2;

	T2 = T1->left;
	T2->parent = T1->parent;
	T1->left = T2->right;
	T2->right =T1;
	T1->parent = T2;
	T1->height = max(get_height(T1->left),get_height(T1->right))+1;
	T2->height = max(get_height(T2->left),T1->height)+1;

	return T2;
}

AVLTreeNode *RRRotate(AVLTreeNode *T1)
{
	assert(T1!=NULL);
	AVLTreeNode *T2;
	T2 = T1->right;
	T2->parent = T1->parent;
	T1->right = T2->left;
	T2->left =T1;
	T1->parent = T2;
	T1->height = max(get_height(T1->left),get_height(T1->right))+1;
	T2->height = max(get_height(T2->right),T1->height)+1;

	return T2;
}
//define double rotation
AVLTreeNode *LRRotate(AVLTreeNode *T1)
{	assert(T1!=NULL);
	T1->left = RRRotate(T1->left);
	return LLRotate(T1);
}

AVLTreeNode *RLRotate(AVLTreeNode *T1)
{	assert(T1!=NULL);
	T1->right = LLRotate(T1->left);
	return RRRotate(T1);
}

//rebalancing after insert/remove
 void Restructure(AVLTree *T,AVLTreeNode *next)
 {
	 AVLTreeNode *parent;
	 parent = next->parent;
	 while (parent!=NULL){                      // balance fix
		parent->height = max(get_height(parent->left),get_height(parent->right))+1;
		int parent_bf = get_height(parent->left) - get_height(parent->right);
		if (parent_bf == 0) break;
		else if (parent_bf == 1 || parent_bf == -1){
			next = parent;
			parent = parent->parent;
		}
		else{                   // parent_bf == 2 || -2
			int node_pd = get_height(next->left) - get_height(next->right);
			if (parent_bf== 2){
				if (node_pd == -1)  {
					if (parent->parent == NULL) T->root = LRRotate(parent);
					else parent->parent->right =LRRotate(parent);
				}
				else {
					if (parent->parent == NULL) T->root = LLRotate(parent);
					else{
						if (parent->key > (parent->parent->key))
						(parent->parent->right)=LLRotate(parent);
						else (parent->parent->left)=LLRotate(parent);
					}
				}
			}
			else{
				if(node_pd == -1) {
					if (parent->parent == NULL) T->root = RRRotate(parent);
					else parent->parent->right = RRRotate(parent);
				}
				else {
					if (parent->parent == NULL) T->root = RLRotate(parent);
					else {
						if (parent->key > (parent->parent->key))
						parent->parent->right = RLRotate(parent);
						else parent->parent->left = RLRotate(parent);
					}
				}
			}
			parent = parent->parent;
		}
	}
}

 //time complexity:Suppose the size of T is N, the height of T is log(N). it's O(logN)
 int InsertNode(AVLTree *T, int k, int v)
 {
 	AVLTreeNode *node;
 	AVLTreeNode *next;
 	AVLTreeNode *parent;
 	if (T->root == NULL)  {     //base case
 		node = newAVLTreeNode(k,v);
 		T->root = node;
 		T->root->height =1;
 		T->size +=1;
 		return 1;
 	}
 	node = T->root;				//find the parent node of new node
 	while (node!=NULL){

 		parent = node;
 		if (k < node->key){
 			node = node->left;
 		}
 		else if (k > node->key){
 			node = node->right;
 		}
 		else{
 			return 0;    //do not insert
 		}
 	}
 	next = newAVLTreeNode(k,v);          //insert new node
 	T->size +=1;
 	if (k > parent->key){
 		parent->right = next;
 		next ->parent = parent;
 		}
 	else{
 		parent->left = next;
 		next->parent = parent;
 		}
 	Restructure(T,next);

 	return 1;
 }

 //time complexity:Suppose there are N items, call insertNode function for every node. It's O(NlogN)
 AVLTree *CreateAVLTree(const char *filename)
 {
 	AVLTree *T;
 	T = newAVLTree();
 	int a,b,c;
 	char StrLine[1024];
 	int k,v;
	char *delim = " ;\n";
	char *sub_delim = "(,.)";
	char *str,*temp;
	char* pSave = NULL;
	str = malloc(100);
	temp =malloc(100);
 	if (strcmp(filename,"stdin")==0){
		while(strlen(gets(StrLine))!=0)      //read lines
		{
			str = strtok_r(StrLine,delim,&pSave);  //read items
			while(str!=NULL){
				strcpy(temp,str);
				int len = strlen(temp);
				if (len<5){                      //length for a valid format,i.e.(k,v),at least is 5
					printf("invalid input\n");
					exit(1);
				}

				temp= strtok(temp,"(");
				if (strlen(temp)==len){          //check whether there is a '(' in items
					printf("no (");
					exit(1);
				}
				int len1 = strlen(temp);
				temp = strtok(temp,",");
				if (strlen(temp)==len1){          //check whether there is a ',' in items
					printf("no ,");
					exit(1);
				}
				k = atoi(temp);                  //convert char into integer
				if (k==0 && *temp!='0'){         //check key is a integer or not
					printf("not integer");
					exit(1);
				}
				temp = strtok(NULL,")");
				int len2 = strlen(temp);
				if(len-len1-2==len2){            //check whether there is a ')' in items
					printf("invalid input\n");
					return T;
				}
				v=atoi(temp);
				if (v==0 && *temp!='0'){             //check value is a integer or not
					printf("not integer");
					exit(1);
				}

				a = InsertNode(T,k,v);
				if (a == 0){
				 	printf("error");
				 	return NULL;
				}
				str=strtok_r(NULL,delim,&pSave);
			}
		}
 	}
 	else{
 		FILE *fp = fopen(filename,"r");
 		if (fp==NULL){
 			perror('fopen');
 			exit(1);
 		}
 		while(!feof(fp)){
 			fscanf(fp,"(%d,%d)\n",&b,&c);
 			a = InsertNode(T,b,c);
 			if (a == 0){
 				printf("error");
 				return NULL;
 			}
 		}
 		fclose(fp);
 	}
 	return T;
 }

//Separately store key and value of the tree node into two array in inorder traversal order.
//Time complexity: Suppose the size of tree is N, it's O(N).
 void TreeToList(AVLTreeNode *node,int key[],int value[],int *index)
 {
 	if(node == NULL) return;
 	TreeToList(node->left,key,value,index);
 	key[*index]= node->key;
 	value[*index] = node->value;
 	(*index)++;
 	TreeToList(node->right,key,value,index);
 }

 //Merge two array into one array in ascending order.
 //Time complexity: Suppose the sizes of two arrays is N and M respectively, it's O(N+M).
 void MergeTwoList(int arr1[],int arr2[],int arr3[],int a, int b){
 	int i=0,j=0,k=0;
 	while(i<a && j<b){
 		if(arr1[i]<arr2[j]) arr3[k++]=arr1[i++];
 		else arr3[k++]=arr2[j++];
 	}
 	while(i<a){
 		arr3[k++]=arr1[i++];
 	}
 	while(j<b){
 		arr3[k++]=arr2[j++];
 	}
 //	return arr3;
 }

 //time complexity: Suppose the size of T is N, it's O(N)
 AVLTree *CloneAVLTree(AVLTree *T)
 {
	AVLTree *T2;
 	T2 = newAVLTree();
 	if(T==NULL) return T2;
 	int arr_k[T->size],arr_v[T->size];
 	int i=0,k;
 	TreeToList(T->root,arr_k,arr_v,&i);
 	for (int a=0;a<T->size;a++){
 		k = InsertNode(T2,arr_k[a],arr_v[a]);
 		if(k==0) printf("error %d",a);
 	}
 	return T2;
 }

 //time complexity: Suppose N and M are the sizes of T1 and T2.
 // O(N+M+(M+N)+(M+N)+(M+N)) = O(M+N)
 AVLTree *MergeTwoAVLTrees(AVLTree *T1, AVLTree *T2)
 {
 	AVLTree *T3;
 	T3 = newAVLTree();
 	int m=T1->size,n=T2->size;
 	int arr1_key[m],arr1_value[m];
 	int i =0;
 	TreeToList(T1->root,arr1_key,arr1_value,&i);

 	int arr2_key[n],arr2_value[n];
 	int j =0;
 	TreeToList(T2->root,arr2_key,arr2_value,&j);

 	int arr3_key[n+m],arr3_value[n+m];
 	MergeTwoList(arr1_key,arr2_key,arr3_key,m,n);
 	MergeTwoList(arr1_value,arr2_value,arr3_value,m,n);

 	for(int k=0;k<m+n;k++)
 	{
 		j = InsertNode(T3,arr3_key[k],arr3_value[k]);
 		if(j==0) printf("error %d",k);
 	}
 	return T3;
 }

 //find the immediately inorder successor of node.
AVLTreeNode *FindMin(AVLTreeNode *node)
{
	if(node!=NULL)
	{
		while(node->left){
			node = node->left;
		}
	}
	return node;
}

//time complexity: Suppose the size of T is N, the height of T is log(N). it's O(logN)
AVLTreeNode *Search(AVLTree *T, int k)
{
	AVLTreeNode *node;
	node = T->root;
	while(node!=NULL){
		if(k == node->key){
			return node;
		}
		else if(k< node->key){
			node=node->left;
		}
		else node=node->right;
	}
	return NULL;
}

//inorder traversal
void Inorder(AVLTreeNode *node)
{
	if (node!=NULL)
	{
		Inorder(node->left);
		printf("(%d,%d)\n",node->key,node->value);
		Inorder(node->right);
	}
}

// time complexity:print every node of tree T. suppose the size of T in N. it's O(N).
void PrintAVLTree(AVLTree *T)
{
	if (T!=NULL){
		Inorder(T->root);
	}
}

// time complexity:Suppose the size of T is N, the height of T is log(N). it's O(logN)
 int DeleteNode(AVLTree *T, int k)
{
	AVLTreeNode *node;
	AVLTreeNode *child;
	AVLTreeNode *parent;
	AVLTreeNode *temp;
	if(T->root == NULL) return 0;
	node = Search(T,k);
	if (node==NULL) return 0;
	if (node==T->root){
		T->root =NULL;
		return 1;
	}
	parent = node->parent;

	if (node->height == 0){          //delete a leaf node
		if(node->key > parent->key) {
			parent->right =NULL;
			child = parent->left;
		}
		else{
			parent->left = NULL;
			child = parent->right;
		}
		free(node);
		Restructure(T,child);
	}

	else if (node->left==NULL || node->right==NULL){          //only one child,child must be a leaf
		if(node->left == NULL)  child = node->right;
		else child = node->left;
		if (parent==NULL){                //delete the root of the tree
			T->root = child;
			child->parent = NULL;
			free(node);
		}
		else{
			if(node->key > parent->key) parent->right = child;
			else parent->left = child;
			child->parent = parent;
			free(node);
			Restructure(T,child);
		}
	}
	else{							 //have two children
		temp = node;
		node = FindMin(node);    //node has no left child
		child = node->right;
		temp->key = node->key;
		temp->value = node->value;
		parent = node->parent;
		if (parent==temp){
			parent->right = child;
		}
		else{
			parent->left = child;
		}
		if (child != NULL)  child->parent = parent;
		free(node);
		if(child) Restructure(T,child);
		else Restructure(T,parent);
	}
	T->size -=1;
	return 1;
}

void FreeAVLTreeNode(AVLTreeNode *node)
{
	if (node==NULL) return;
	FreeAVLTreeNode(node->left);
	FreeAVLTreeNode(node->right);
	free(node);
}

// time complexity:Suppose the size of AVLTree is N,the FreeAVLTreeNode function will be executed for every
//node of Tree,Since it's O(N)
void FreeAVLTree(AVLTree *T)
{
	if (T==NULL) return;
	FreeAVLTreeNode(T->root);
	free(T);
}

int main() //sample main for testing
{
 int i,j;
 AVLTree *tree1, *tree2, *tree3, *tree4;
 AVLTreeNode *node1;
 tree1=CreateAVLTree("stdin");
 printf("tree1:\n");
 PrintAVLTree(tree1);
 FreeAVLTree(tree1);
 tree2=CreateAVLTree("File1.txt");
 printf("tree2:\n");
 PrintAVLTree(tree2);
 tree3=CloneAVLTree(tree2);
 printf("tree3:\n");
 PrintAVLTree(tree3);
 FreeAVLTree(tree2);
 PrintAVLTree(tree2);
 tree4=newAVLTree();
 j=InsertNode(tree4, 10, 10);
 for (i=0; i<15; i++)
  {
   j=InsertNode(tree4, i, i);
   if (j==0) printf("(%d, %d) already exists\n", i, i);
  }
  printf("tree4:\n");
  PrintAVLTree(tree4);
  node1=Search(tree4,20);
  if (node1!=NULL)
    printf("key= %d value= %d\n",node1->key,node1->value);
  else
    printf("Key 20 does not exist\n");

  for (i=17; i>0; i--)
  {
    j=DeleteNode(tree4, i);
	if (j==0)
	  printf("Key %d does not exist\n",i);
  }
 printf("tree4:\n");
 PrintAVLTree(tree4);
 FreeAVLTree(tree4);
 return 0;
}

