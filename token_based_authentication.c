// C program to insert a node in AVL tree 
#include<stdio.h> 
#include<stdlib.h> 
#include<string.h>
#include<time.h>
/*  Membership Levels   1: Platinum
                        2:Diamond
                        3:Gold
                        4:Regular
    Sub Access Levels   1: L1
                        2: L2
*/
struct Node 
{ 
	int uid; 
    char pass[25];
    int memLevel;
    int subLevel;
    struct tm tm;
    struct Node *left; 
	struct Node *right; 
	int height; 
}; 

typedef struct qnode
	{
	int priority;
	int token_no;
    int uid;
	struct qnode *next;
}qnode;

#define MAX 5
int a[11]={0,0,0,0,0,0,0,0,0,0,0};
int b[11]={0,0,0,0,0,0,0,0,0,0,0};

int max(int a, int b); 


int height(struct Node *N) 
{ 
	if (N == NULL) 
		return 0; 
	return N->height; 
} 
 
int max(int a, int b) 
{ 
	return (a > b)? a : b; 
} 


struct Node* newNode(int uid,char pass[],int memLevel,int subLevel) 
{ 
	struct Node* node = (struct Node*) 
						malloc(sizeof(struct Node)); 
	node->uid = uid;
    strcpy(node->pass,pass);
    node->memLevel=memLevel;
    node->subLevel=subLevel; 
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    tm.tm_year += 1900; 
    tm.tm_mon += 2; //tm.tm_mday
    node->tm=tm;
    //printf("\n now: %d-%d-%d %d:%d:%d\n", node->tm.tm_year , node->tm.tm_mon , node->tm.tm_mday, node->tm.tm_hour, tm.tm_min, tm.tm_sec);
    node->left = NULL; 
	node->right = NULL; 
	node->height = 1; // new node is initially added at leaf 
	return(node); 
} 


struct Node *rightRotate(struct Node *y) 
{ 
	struct Node *x = y->left; 
	struct Node *T2 = x->right; 
 
	x->right = y; 
	y->left = T2; 

	y->height = max(height(y->left), height(y->right))+1; 
	x->height = max(height(x->left), height(x->right))+1; 

	return x; 
} 

 
struct Node *leftRotate(struct Node *x) 
{ 
	struct Node *y = x->right; 
	struct Node *T2 = y->left; 
 
	y->left = x; 
	x->right = T2; 

	x->height = max(height(x->left), height(x->right))+1; 
	y->height = max(height(y->left), height(y->right))+1; 

	return y; 
} 

int getBalance(struct Node *N) 
{ 
	if (N == NULL) 
		return 0; 
	return height(N->left) - height(N->right); 
} 


struct Node* insert(struct Node* node,int uid,char pass[],int memLevel,int subLevel) 
{ 
    
	if (node == NULL) 
		return(newNode(uid,pass,memLevel,subLevel)); 

	if (uid < node->uid) 
		node->left = insert(node->left,uid,pass,memLevel,subLevel); 
	else if (uid > node->uid) 
		node->right = insert(node->right,uid,pass,memLevel,subLevel); 
	else // Equal keys are not allowed in BST 
		return node; 

	node->height = 1 + max(height(node->left), 
						height(node->right)); 

	int balance = getBalance(node); 

	if (balance > 1 && uid < node->left->uid) 
		return rightRotate(node); 
 
	if (balance < -1 && uid > node->right->uid) 
		return leftRotate(node); 

	if (balance > 1 && uid > node->left->uid) 
	{ 
		node->left = leftRotate(node->left); 
		return rightRotate(node); 
	} 

	if (balance < -1 && uid < node->right->uid) 
	{ 
		node->right = rightRotate(node->right); 
		return leftRotate(node); 
	} 

	return node; 
} 

void inOrder(struct Node *root) 
{ 
	if(root != NULL) 
	{
        inOrder(root->left);  
		printf("%d ", root->uid); 
		inOrder(root->right); 
	} 
}

struct Node* traverse(struct Node *root,int uid) 
{ 
	int done=0;
    while(root!=NULL && done==0)
    {
        if(root->uid==uid)
            done=1;
        else if (root->uid < uid)
        {
            root=root->right;
        }
        else
        {
            root=root->left;
        }
    }
    return root; 
} 

int present=0;

int movtoMem(struct Node* root,struct tm tm)
{
    //printf("\n%d-%d-%d \n", tm.tm_year, tm.tm_mon , tm.tm_mday);
    int level;
    //static present=0;
    if(root!=NULL)
    {
        if(tm.tm_mon==root->tm.tm_mon)
        {
            root->memLevel=4;
            printf("\nCustomer with userid %d has been moved to regular\n",root->uid);
            root->tm.tm_mon++;
            present++;
        }
        movtoMem(root->left,tm);
        movtoMem(root->right,tm);
    }
    
}

void updateMem(struct Node* root)
{
    int uid,memLevel;
    char pass[25];
    printf("\n Enter userId");
    scanf("%d",&uid);
    printf("\n Enter password");
    scanf("%s",pass);
    struct Node* temp;
    temp=traverse(root,uid);
    if(temp==NULL)
        printf("\n Invalid userid\n");
    else
    {
        if(strcmp((temp->pass),pass))
            printf("\n Invalid password\n");
        else
        {
            printf("\nEnter new membership level \n1:Platinum 2:Diamond 3:Gold 4:Regular");
            scanf("%d",&memLevel);
            temp->memLevel=memLevel;
            //make token invalid(delete)
            time_t t = time(NULL);
            struct tm tm = *localtime(&t);
            tm.tm_year += 1900; 
            tm.tm_mon += 2; //tm.tm_mday
            temp->tm=tm;
            printf("\n Succesfully Updated!! \n Your renewal date is : %d-%d-%d \n", tm.tm_year, tm.tm_mon , tm.tm_mday);
        }
    }
    
}

qnode* insert_token(qnode* front,int priority,int uid)
{
	qnode *tmp,*q;
    int no=1;
	tmp = (qnode *)malloc(sizeof(qnode));
	tmp->priority = priority;
    tmp->uid=uid;
	/*Queue is empty or item to be added has priority more than first item*/
	if( front == NULL || priority < front->priority )
	{
		tmp->next = front;
		front = tmp;
	}
	else
	{
		q = front;
        no++;
		while( q->next != NULL && q->next->priority <= priority )
		{
            q=q->next;
            no++;
        }
		tmp->next = q->next;
		q->next = tmp;
	}
    tmp->token_no=no;
    tmp=tmp->next;
    while(tmp!=NULL)
    {
        tmp->token_no+=1;
        tmp=tmp->next;
    }
    printf("\nToken generated succesfully\n");
    return front;

}

void genToken(struct Node* node,qnode** active_list,qnode** wait_list)
{
    int mlevel,slevel,p;
    mlevel=node->memLevel;
    slevel=node->subLevel;
    p=(mlevel*2)+slevel;
    if(a[p]==MAX)
    {
        b[p]++;
        *wait_list=insert_token(*wait_list,p,node->uid);
    }
    else
    {
        *active_list=insert_token(*active_list,p,node->uid);   
    }
}

void display(qnode* front)
{
	qnode *ptr;
	ptr = front;
	if(front == NULL)
		printf("Queue is empty\n");
	else
	{	
		//printf("Queue is :\n");
		printf("User_ID       Token_number\n");
		while(ptr != NULL)
		{
			printf("%5d        %5d\n",ptr->uid,ptr->token_no);
			ptr = ptr->next;
		}
	}
}

int main() 
{ 
qnode *active_list = NULL;
qnode *wait_list = NULL;
struct Node *root = NULL; 
int memLevel,subLevel;
char pass[25];
root = insert(root,123,"123",1,1);
int uid=(rand()%1000)+1;
root = insert(root,uid,"1234",2,2);
uid=(rand()%1000)+1; 
root = insert(root,uid,"abc",3,2); 
uid=(rand()%1000)+1;
root = insert(root, uid,"xyz",4,2);
uid=(rand()%1000)+1; 
root = insert(root,uid,"1234",1,1);
uid=(rand()%1000)+1; 
root = insert(root,uid,"12345",1,2);

root->left->tm.tm_mon=4;

int choice=0;
    while(choice!=6)
    {
        printf("\nCHOOSE A OPERATION \n1:Generate Token \n2:Add new customer \n3:Display active and waiting \n4:Move to membership \n5:Update membership \n6:EXIT");
        scanf("%d",&choice);
        if(choice==1)
        {
            printf("\nEnter your userid");
                scanf("%d",&uid);
                printf("\nEnter your password");
                scanf("%s",pass);
                struct Node* temp = traverse(root,uid);
                if(temp==NULL)
                    printf("\n Invalid userid\n");
                else
                {
                    if(strcmp((temp->pass),pass))
                        printf("\n Invalid password\n");
                    else
                    {
                        genToken(temp,&active_list,&wait_list);   
                    }
                    
                }
        }
        else if(choice==2)
        {
            printf("\nCreate password");
            scanf("%s",pass);
            printf("\nEnter membership level \n1:Platinum 2:Diamond 3:Gold 4:Regular");
            scanf("%d",&memLevel);
            printf("\nEnter subAccess level \n1:L1 2:L2");
            scanf("%d",&subLevel);
            uid=(rand()%1000)+1; 
            root = insert(root,uid,pass,memLevel,subLevel);
            printf("\n Your useId is: %d",uid);
            
        }
        else if(choice==3)
        {
            printf("\nActive list\n");
            display(active_list);
            printf("******************************************");
            printf("\nWaiting list\n");
            display(wait_list);
        }
        else if(choice==4)
        {
            time_t t = time(NULL);
            struct tm tm = *localtime(&t);
            tm.tm_year += 1900; 
            tm.tm_mon += 1;
            movtoMem(root,tm);
            if(present==0)
                printf("\nNo customers with expired membership\n");
            present=0;
        }
        else if (choice==5)
        {
            updateMem(root);          
        }
        else if(choice==6)
        {

        }
        else
        {
            printf("Please enter valid option");
        }
    }



// printf("\n Inorder traversal of the constructed AVL tree is \n"); 
// inOrder(root);
// time_t t = time(NULL);
// struct tm tm = *localtime(&t);
// printf("now: %d-%d-%d %d:%d:%d\n", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);

return 0; 
} 
