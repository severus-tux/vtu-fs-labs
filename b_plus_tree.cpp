//B-plus trees
#include <iostream>
#define order 4
#define min 2
#define q_size 128
using namespace std;

struct bplustree_node
{
		int *data; 		  		
		bplustree_node **child_ptr;
		bool leaf;		  		
		int n;	   			    
		bplustree_node *parent;
		bplustree_node *next;		
};
bplustree_node *root=NULL, *np=NULL, *x=NULL, *left_most_leaf;

bplustree_node* init()
{
	np=new bplustree_node;
	np->data=new int [order+1];
	np->child_ptr=new bplustree_node* [order+2];
	np->leaf=true;
	np->n=0;
	np->parent=NULL;
	np->next=NULL;
	for(int i=0;i<order+2;i++)
		np->child_ptr[i]=NULL;
	return np;
}

bool search(int k)
{
	bplustree_node *temp=left_most_leaf;
	
	while(temp!=NULL)
	{
		for(int i=0;i<temp->n;i++)
			if(temp->data[i]==k)
				return true;
		temp=temp->next;
	}
	return false;
}

void display ()	// level order traversal; with NULL as a marker for "\n"
{
	bplustree_node *queue[q_size];	// circular queue for level order traversal;
	int q_start=0, q_end=-1;
	int i;
	queue[++q_end]=root;
	queue[++q_end]=NULL;
	
	int temp;
	if(q_end < q_start)
		return;
	cout<<"\n";
	while(! ( q_end < q_start ))
	{
		if(queue[q_start]==NULL)
		{
			cout<<"\n";
			
			if(q_end>q_start)
				queue[++q_end]=NULL;
			q_start=(q_start+1)%q_size;
			continue;
		}
		cout <<"\033[1;31m [\033[0m";
		temp=q_end;
		for( i=0; i<queue[q_start]->n;i++)
		{
			cout <<" "<<queue[q_start]->data[i]<<" ";
			if(queue[q_start]->child_ptr[i]!=NULL)	
				queue[++q_end]=queue[q_start]->child_ptr[i];
		}
		if(queue[q_start]->child_ptr[i]!=NULL)	
				queue[++q_end]=queue[q_start]->child_ptr[i];
		cout <<"\033[1;31m]\033[0m";
		q_start=(q_start+1)%q_size;
	}
	cout<<"\n\n";
}

void split(bplustree_node *x)
{
	int pos=0,i=0;
	bplustree_node *temp=init();
	
	if(x->parent==NULL)
	{
		bplustree_node *new_root=init();
		new_root->leaf=false;
		new_root->child_ptr[0]=x;
		x->parent=new_root;
		root=new_root;
	}
	
	if(x->leaf)
	{
		temp->parent=x->parent;
		temp->leaf=x->leaf;
		for( i=min;i<=order;i++)
			temp->data[i-min]=x->data[i];
		x->n=min;
		temp->n=order-min+1;
		temp->next=x->next;
		x->next=temp;
	}
	else
	{		
		temp->parent=x->parent;
		temp->leaf=x->leaf;
		for( i=min+1;i<=order;i++)
		{
			temp->data[i-(min+1)]=x->data[i];
			temp->child_ptr[i-(min+1)]=x->child_ptr[i];
			temp->child_ptr[i-(min+1)]->parent=temp;
		}
		temp->child_ptr[i-(min+1)]=x->child_ptr[i];
		temp->child_ptr[i-(min+1)]->parent=temp;
		x->n=min;
		temp->n=order-min;
	}
	
	while(pos<x->parent->n+1 && x->parent->child_ptr[pos]!=x)
		pos++;
	
	for( i=x->parent->n;i>pos;i--)
	{
		x->parent->data[i]=x->parent->data[i-1];
		x->parent->child_ptr[i+1]=x->parent->child_ptr[i];
	}
	
	if (x->leaf)
		x->parent->data[pos]=temp->data[0];
	else
		x->parent->data[pos]=x->data[min];
		
	x->parent->child_ptr[pos+1]=temp;
	x->parent->n++;
	
	if(x->parent->n > order)
		split(x->parent);
	return;
}

void insert(bplustree_node *x,int key)
{
	int pos=0;
	
	while(pos<x->n && key>=x->data[pos])
			pos++;
			
	if(x->leaf)
	{
		for(int i=x->n;i>pos;i--)
			x->data[i]=x->data[i-1];
		x->data[pos]=key;
		x->n++;
		if(x->n>order)
			split(x);
	}
	else
		insert(x->child_ptr[pos],key);
}

int main()
{
	root=init();
	left_most_leaf=root;
	int ch,key;
	
	while(true)
	{
		cout<<"Select an option :\n\n"
			<<"\t1.Insert\n"
			<<"\t2.Search\n"
			<<"\t3.Display\n"
			<<"\t4.Exit\n"
			<<"\n[1-4] : ";
		cin >> ch;
		
		switch(ch)
		{
			case 1:
					cout<<"\nEnter element : ";
					cin>>key;
					insert(root,key);
				break;
			case 2 :
					cout<<"\nEnter element to search : ";
					cin>>key;
					if(search(key))
						cout<<"Element found!\n";
					else
						cout<<"Element not found! \n";
				break;
			case 3 : 
					display();
				break;
			case 4 : 
			default: return 0;
		}
	}
	return 0;
}
