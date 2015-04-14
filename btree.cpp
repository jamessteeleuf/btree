#include "bTree.h"

bTree::bTree(int M){
	// Set how many keys can be in one node
	this->set_boundaries(M);

	// Initialize the tree as empty
	this->root = NULL;
}

void bTree::set_boundaries(int M){
	// Our nodes should run from M/2 to M
	this->lower_bound = M/2;
	this->upper_bound = M;
}

void bTree::traverse(){
	if(root != NULL){
		root->traverse();
	}
}

bool bTree::find(string key){
	return (root == NULL) ? false : root->find(key);
}

bTreeNode::bTreeNode(int M, bool is_leaf){
	// Set how many keys can be in one node
	this->set_boundaries(M);

	// We need to keep track of this to make deletion et al easier
	this->is_leaf = is_leaf;

	// Create the data structures to store our keys and children
	entries = new entry[this->upper_bound-1];
	children = new bTreeNode*[this->upper_bound];

	// We have no keys, so set this to 0
	number_of_keys = 0;
}

void bTreeNode::set_boundaries(int M){
	// Our nodes should run from M/2 to M
	this->lower_bound = M/2;
	this->upper_bound = M;
}

void bTreeNode::traverse(){
	int i;

	// Walk along each node
	for(i = 0; i < this->number_of_keys; i++){
		if(!this->is_leaf){
			children[i]->traverse();
		}
	}

	if(!this->is_leaf){
		children[i]->traverse();
	}
}


bTreeNode* bTreeNode::search(string key){
	int partition_index = 0;

	// Find the first key >= the search key
	while(partition_index < this->number_of_keys && key > this->entries[partition_index]){
		partition_index++;
	}

	// Check if our search key is the same as the partition key
	if(key == this->entries[partition_index].key){
		return this;
	}

	// Key isn't found and this is a leaf
	if(this->is_leaf){
		return NULL;
	}

	// Check our partition node for the key
	return children[i]->search(key);
}

void bTree::insert(string key, string value){
	// When the tree is empty, we can just insert the new key at the base
	if(this->root == NULL){
		this->root = new bTreeNode(this->upper_bound, true);
		root->entries[0].key = key;
		root->entries[0].value = value;
		root->number_of_keys = 0;
	}else{
		if(root->number_of_keys == this->upper_bound - 1){
			// If the tree is full, then make the old root the child of the new root, split it, then put the key where it should go
			bTreeNode *s = new bTreeNode(this->upper_bound, false);

			s->children[0] = root;

			s->split_child(0, root);

			int i = 0;
			if(s->entries[0].key < key){
				i++;
			}

			s->children[i]->insert_non_full(key, value);

			this->root = s;
		}else{
			this->root->insert_non_full(key, value);
		}
	}
}

void bTreeNode::insert_non_full(string key, string value){
	int i = n - 1; // Deal with our rightmost element

	if(this->is_leaf){
		// Move all greater keys up and find where to insert the key
		while(i >= 0 && this->entries[i].key > key){
			this->entries[i + 1].key = this->entries[i + 1].key;
			i--;
		}

		// Insert the keys
		this->entries[i + 1].key = key;
		this->number_of_keys++;
	}else{
		while(i >= 0 && this->entries[i].key > key){
			i--;
		}

		// If the child is full, split it
		if(this->children[i + 1]->number_of_keys == this->upper_bound - 1){
			this->split_child(i + 1, this->children[i + 1]);

			// Determine where to put the new key
			if(this->entries[i+1].key < key){
				i++;
			}
		}

		this->children[i+1]->insert_non_full(key);
	}
}

void bTreeNode::split_child(int i, bTreeNode* y){
	bTreeNode* z = new bTreeNode(this->upper_bound, y->is_leaf);
	z->number_of_keys = this->lower_bound - 1; // Set the # of keys to just below the min

	for(int j = 0; j < this->lower_bound - 1; j++){
		z->entries[j] = y->entries[this->lower_bound + j];
	}

	if(!y->is_leaf){
		for(int j = 0; j < this->lower_bound; j++){
			z->children[j] = y->children[j + this->lower_bound];
		}
	}

	y->number_of_keys = this->lower_bound - 1;

	// Move everything up
	for(int j = n; j >= i+1; j--){
		this->children[j + 1] = this->children[j];
	}

	// Insert the child
	this->children[i + 1] = z;

	// Move the entries up
	for(int j = n-1; j >= i; j--){
		this->entries[j + 1] = this->entries[j];
	}

	this->entries[i] = y->entries[t - 1];
	this->number_of_keys++;
}

int bTreeNode::find_key(string key){
	int index = 0;
	while(index < this->number_of_keys && this->entries[index].key < key){
		++index;
	}
	return index;
}

void bTreeNode::remove(string key){
	int index = this->find_key(key);

	if(index < this->number_of_keys && this->entries[index].key == key){
		if(this->is_leaf){
			this->remove_from_leaf(index);
		}else{
			this->remove_from_non_leaf(index);
		}
	}else{
		if(this->is_leaf){
			return false;
		}

		bool flag = index == n;

		if(this->children[index]->number_of_keys < this->lower_bound){
			this->fill(index);
		}

		if(flag && index > this->number_of_keys){
			this->children[index-1]->remove(key);
		}else{
			this->children[index]->remove(key);
		}
	}
	return true;
}

bool bTreeNode::remove_from_leaf(int index){
	for(int i = index + 1; index < this->number_of_keys; ++i){
		this->entries[i-1] = this->entries[i];
	}
	this->number_of_keys--;
	return true;
}

bool bTreeNode::remove_from_non_leaf(int index){
	entry e = this->entries[index];

	if(children[index]->number_of_keys >= this->lower_bound){
		entry* predecessor = this->get_predecessor(index);
		this->entries[index] = predecessor;
		this->children[index]->remove(predecessor);
	}else if(this->children[index + 1]->number_of_keys >= this->upper_bound){
		entry* successor = this->get_successor(index);
		this->entries[index] = successor;
		this->children[index+1]->remove(successor);
	}else{
		this->merge(index);
		this->children[index]->remove(e);
	}
	return true;
}

entry* bTreeNode::get_predecessor(int index){
	bTreeNode* cur = this->children[index];
	while(!cur->is_leaf){
		cur = cur->children[cur->number_of_keys];
	}

	return cur->entries[cur->number_of_keys-1];
}

entry* bTreeNode::get_successor(int index){
	bTreeNode* cur = this->children[index + 1];
	while(!cur->is_leaf){
		cur = cur->children[0];
	}
	return cur->entries[0];
}

void bTreeNode::fill(int index){
	if(index != 0 && this->children[index - 1]->number_of_keys >= t){
		this->borrow_from_previous(index);
	}else if(index != n && this->children[index + 1]->number_of_keys >= t){
		this->borrow_from_next(index)
	}else{
		if(index != this->number_of_keys){
			this->merge(index);
		}else{
			this->merge(index - 1);
		}
	}
}

void bTreeNode::borrow_from_previous(int index){
	bTreeNode* child = this->children[index];
	bTreeNode* sibling = this->children[index - 1];

	for(int i = child->number_of_keys - 1; i >= 0; --i){
		child->entries[i + 1] = child->entries[i];
	}

	if(!child->is_leaf){
		for(int i = child->number_of_keys; i >= 0; --i){
			child->children[i + 1] = child->children[i];
		}
	}

	child->entries[0] = this->entries[index - 1];

	if(!is_leaf){
		child->children[0] = sibling->children[sibling->number_of_keys];
	}

	this->entries[index - 1] = sibling->entries[sibling->number_of_keys - 1];
	
	child->number_of_keys++;
	sibling->number_of_keys--;
}

void bTreeNode::borrow_from_next(int index){
	bTreeNode* child = this->children[index];
	bTreeNode* sibling = this->children[index + 1];

	child->entries[child->number_of_keys] = this->entries[index];

	if(!(child->is_leaf)){
		child->children[child->number_of_keys + 1] = sibling->children[0];
	}

	this->entries[index] = sibling->entries[0];

	for(int i = 1; i < sibling->number_of_keysn; ++i){
		sibling->entries[i-1] = sibling->entries[i];
	}

	if(!sibling->leaf){
		for(int i = 1; i <= sibling->number_of_keys; ++i){
			sibling->children[index-1] = sibling->children[i];
		}
	}

	child->number_of_keys++;
	sibling->number_of_keys--;
}

void bTreeNode::merge(int index){
	bTreeNode* child = this->children[index];
	bTreeNode* sibling = this->children[index + 1];

	child->entries[this->lower_bound - 1] = this->entries[index];

	for(int i = 0; i < sibling->number_of_keys; ++i){
		child->entries[i + this->lower_bound] = sibling->entries[i];
	}

	if(!child->is_leaf){
		for(int i = 0; i <= sibling->number_of_keys; ++i){
			child->children[i + this->lower_bound] = sibling->children[i];
		}
	}

	for(int i = index + 1; i < this->number_of_keys; ++i){
		this->entries[i - 1] = this->entries[i];
	}

	for(int i = index + 2; i <= this->number_of_keys; ++i){
		this->children[i - 1] = this->children[i];
	}

	child->number_of_keys += sibling->number_of_keys + 1;
	this->number_of_keys--;

	delete(sibling);
}

bool delete_key(string key){
	if(!this->root){
		return false;
	}

	bool result = this->root->remove(key);

	if(this->root->number_of_keys == 0){
		bTreeNode* temp = this->root;
		if(this->root->is_leaf){
			this->root = NULL;
		}else{
			this->root = this->root->children[0];
		}

		delete temp;
	}

	return result;
}