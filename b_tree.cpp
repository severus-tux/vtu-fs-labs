#include <iostream>
#define order 4
#define min 2
#define q_size 128
using namespace std;


struct btree_node
{
		int *data; 		  		
		btree_node **child_ptr; 
		bool leaf;		  		
		int n;	   			    
		btree_node *parent;		
};

btree_node *root=NULL, *np=NULL, *x=NULL;

btree_node* init()
{
	np=new btree_node;
	np->data=new int [order+1];
	np->child_ptr=new btree_node* [order+1];
	np->leaf=true;
	np->n=0;
	np->parent=NULL;
	for(int i=0;i<order+1;i++)
		np->child_ptr[i]=NULL;
	return np;
}

bool search(btree_node *x, int k)
{
	int i=0;
	while (i<x->n && k>x->data[i])
		i++;
	if (i<x->n && k==x->data[i])
		return true;
		
	else if(x->leaf)
		return false;
		
	else if(i>=x->n) 
		return false;
	
	else
		return search(x->child_ptr[i],k);
}

void sort (btree_node *x) 		// Sort the given array using bubble sort ( smallest element to top )
{
	int temp_data;
	btree_node *temp_node;
	
	for (int i=0;i < x->n;i++)
		for (int j=i;j < x->n;j++)
			if( x->data[j] < x->data[i] )
			{
				temp_data=x->data[i];
				x->data[i]=x->data[j];
				x->data[j]=temp_data;
				
				temp_node=x->child_ptr[i];
				x->child_ptr[i]=x->child_ptr[j];
				x->child_ptr[j]=temp_node;
			}	
}

void traverse(btree_node *x)	//pre-order traversal
{
	for (int i=0;i<x->n;i++)
		cout<<" "<<x->data[i]<<" ";
	cout <<"\n";
	
	if(!x->leaf)
		for(int i=0;i<x->n;i++)
			traverse(x->child_ptr[i]);
}

void display ()	// level order traversal; with NULL as a marker for "\n"
{
	btree_node *queue[q_size];	// circular queue for level order traversal;
	int q_start=0, q_end=-1;
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
		for(int i=0; i<queue[q_start]->n;i++)
		{
			cout <<" "<<queue[q_start]->data[i]<<" ";
			if(queue[q_start]->child_ptr[i]!=NULL)	
				queue[++q_end]=queue[q_start]->child_ptr[i];
		}
		cout <<"\033[1;31m] \033[0m";
		q_start=(q_start+1)%q_size;
	}
	cout<<"\n\n";
}

void split(btree_node *x)
{
	int pos=0;
	
	if(x->parent!=NULL)
		while (x->parent->child_ptr[pos]!=x)
			pos++;
	
	if(x->parent==NULL)
	{
		btree_node *new_root=init();
		new_root->data[0]=x->data[x->n];
		new_root->child_ptr[0]=x;
		x->parent=new_root;
		new_root->leaf=false;
		new_root->n=1;
		root=new_root;
	}
	
	btree_node *temp=init();
	temp->parent=x->parent;
	temp->leaf=x->leaf;
	
	for(int i=min;i<=order;i++)
	{
		temp->data[i-min]=x->data[i];
		temp->child_ptr[i-min]=x->child_ptr[i];
		if(temp->child_ptr[i-min]!=NULL)
			temp->child_ptr[i-min]->parent=temp; // XXXX
		x->child_ptr[i]=NULL;
	}
	x->n=min;
	temp->n=order-min+1;
	x->parent->data[x->parent->n]=temp->data[temp->n-1];
	x->parent->child_ptr[x->parent->n]=temp;
	x->parent->data[pos]=x->data[x->n-1];
	x->parent->n++;
	
	sort(x->parent);
	
	if(x->parent->n > order)
		split(x->parent);
}

void insert(btree_node *x,int key)
{
	if(x->leaf)
	{
		x->data[x->n]=key;
		x->n++;
		sort(x);
		if(x->n > order)
			split(x);
	}
	else
	{
		int pos=0;
		while(key>x->data[pos] && pos<x->n) 
			pos++;
		
		if(pos==x->n)	// promotion
		{
			x->data[x->n-1]=key;
			pos--;
		}
		insert(x->child_ptr[pos],key);
	}
}

int main()
{
	root=init();
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
					if(search(root,key))
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
