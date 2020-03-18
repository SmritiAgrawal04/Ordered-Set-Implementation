#include<iostream>
#include<stdio.h>

using namespace std;
// int count = 0;
int result = INT8_MAX;
int count = 0;
int min_diff = INT8_MAX;
int min_key = -1;

class node{
public:
	int key;
	int lc ,rc;
	node *left;
	node *right;
};

int cal_height(node* n){
	if(n == NULL)
		return 0;
	else
	return (1 + max(cal_height(n->left) , cal_height(n->right)));
}

int balance_factor(node* n){
	if(n == NULL)
		return 0;
	else
		return (cal_height(n->left) - cal_height(n->right));

}

int update_nodes(node* root){
	if(root == NULL)
		return 0;
	else{
		root->lc = 1 + update_nodes(root->left);
		root->rc = 1 + update_nodes(root->right);
		return (root->lc + root->rc - 1);
	}
	return 0;
}

void inOrder(node* n)  {  
	// cout << "in inOrder\n";
    if(n != NULL)  {    
        inOrder(n->left);
        cout << n->key  << " ";  
        inOrder(n->right);  
    }  
}

node* single_left(node* y){
	node* temp1 = y->right;
	node* temp2 = temp1->left;
	temp1->left = y;
	y->right = temp2;
	return temp1;
}

node* single_right(node* y){
	node* temp1 = y->left;
	node* temp2 = temp1->right;
	temp1->right = y;
	y->left = temp2;
	return temp1;
}

node* double_lr(node* y){
	node* temp = y->left;
	y->left = single_left(temp);
	y = single_right(y);
	return y;
}

node* double_rl(node* y){
	node* temp = y->right;
	y->right = single_right(temp);
	y = single_left(y);
	return y;
}

node* check_balfact(node* x , int bf , int var){
	if(bf < -1){
		if(var > x->right->key)
			x = single_left(x);
		else if(var < x->right->key)
			x = double_rl(x);
	}

	if(bf > 1){
		if(var < x->left->key)
			x = single_right(x);
		else if(var > x->left->key)
			x = double_lr(x);
	}
	return x;
}


node* find_predecessor(node* pre){
	while(pre->right != NULL)
		pre = pre->right;
	return pre;
}

bool set_search(node* root , int value){
	if(root == NULL)
		return false;
	if(root->key == value)
		return  true;
	
	if(root->key > value)
		return set_search(root->left , value);
	else
		return set_search(root->right , value);
	
}

float get_kth(node* root , int mid){
	float answer = root->key;
	
	if(root){
		node *current = root;

		while(current){
			if(current->lc == mid){
				answer = current->key;
				break;
			}	
		else if(mid > current->lc-1){
			mid = mid - current->lc ;
			current = current->right;
			}
		else
			current = current->left;
		}
	}
	return answer;
}

struct node* delete_node(node* root , int value){
	if(root == NULL){
		cout << "The set is now empty!\n";
		return NULL;
	}

	if(root->key == value){
		if(root->left == NULL && root->right == NULL){
			node *temp = root;
			root = NULL;
			free(temp);
		}
		else if(root->left == NULL || root->right == NULL){
			node *temp;
			if(root->left)
				temp = root->left;
			else
				temp = root->right;
			*root = *temp;
			free(temp);
		}
		else{
			node *temp = find_predecessor(root->left);
			root->key = temp->key;
			root->left = delete_node(root->left , temp->key);
		}
	}


	else if(root->key > value)
		root->left = delete_node(root->left , value);


	else
		root->right = delete_node(root->right , value);

	int bf = balance_factor(root);

	return check_balfact(root , bf , value);
}

void set_delete(node* root , int value){
	if(root == NULL){
		cout << "Set is empty!\n";
		return;
	}
	if(set_search(root , value))
		delete_node(root , value);

	else{
		cout << "Element not in set!\n";
		return;
	}
	
}

node* set_insert(node* n , int var){
	// cout << "in insert call\n";
	if(n == NULL){
		n = new node();
		n->key = var;
		n->left = NULL;
		n->right = NULL;

	}
	// cout << "hello";
	if(var < n->key)
		n->left = set_insert(n->left , var);
	
	else if(var > n->key)
		n->right = set_insert(n->right , var);
	else
		return n;

	int bf = balance_factor(n);

	return check_balfact(n , bf , var);
}

void set_closest(node* root , int var){
	if(root == NULL)
		return;

	if(root->key == var)
		min_key = root->key;

	int x = (root->key - var)>0 ? root->key - var : var - root->key;
	if(min_diff > x){
		min_diff = x;
		min_key = root->key;
	}

	if(var >= root->key)
		set_closest(root->right , var);	
	else
		set_closest(root->left , var);
}

int range_query(node *root , int start , int end){
	int answer = 0;

}

int main(){
	int var;
	node *root = NULL;
	int choice;
	
	while(1){
		cout << "1.Insert\n2.Search\n3.Delete\n4.Set Closest\n5.Range Query\n6.kth largest\n";
		cout << "Enter your choice: ";
		cin >> choice;

		if(choice == 1){
			cout << "Enter element to insert : ";
			cin >> var;
			if(!set_search(root , var))
				count++;
			// cout << "before insert call\n";
			root = set_insert(root , var);
			// cout << "after insert call\n";
			// inOrder(root);
			int t_count = update_nodes(root);
			cout << endl;
			// cout << "after predorder call\n";

		}

		else if(choice == 2){
			cout << "Enter element to search : ";
			cin >> var;
			bool is_present = set_search(root , var);
			// cout << "is present = "<<is_present;
			if(is_present)
				cout << "Element is present!" << endl;
			else
				cout << "Element is not present!" << endl;
		}

		else if(choice == 3){
			cout << "Enter element to delete : ";
			cin >> var;
			set_delete(root , var);
			inOrder(root);
			cout << endl;
		}

		else if(choice == 4){
			if(root == NULL)
				cout << "Set is empty!\n";
			else{
				cout << "Enter element for which closest value is to be searched : ";
				cin >> var;
				set_closest(root , var);
				cout << "Closest Value = " << min_key << endl;
			}
			
		}

		else if(choice == 5){
			if(root == NULL)
				cout << "Set is empty!\n";
			else{
				int start,end;
				cout << "Enter your range : ";
				cin >> start >> end;
				int result = range_query(root , start , end);
				cout << "No. of elements in given range = " << result << endl;
			}
		}

		else if(choice == 6){
			if(root == NULL)
				cout << "Set is empty!\n";
			else if(root->left == NULL && root->right == NULL)
				cout << root->key << endl;
			else{
				cout << "Enter the value of k: ";
				int k;
				cin >> k;
				float median = get_kth(root  , count - k +1);
				
				cout << "median = " << median << endl;
			}
		}

	
			
		else
			cout << "INVALID CHOICE!!\n";
	}
	

	return 0;
}