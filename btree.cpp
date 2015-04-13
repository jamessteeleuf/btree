struct entry{
	string key;
	string value;
	struct node* next;
};

struct node{
	int m;
	entry* b;

	node(int M){
		m = 0;
		b = entry[M];
	}
};

bTree::bTree(int M){
	// Set boundaries on how many keys we can store in a node
	this->set_boundaries(M);

	// Initialize our number of nodes and height to 0
	this->count = 0;
	this->height = 0;

	// Create our root node with our upper bound
	this->head = new node(this->upper_bound);
}

void bTree::set_boundaries(int M){
	// We limit the number of keys to between M/2 and M
	this->upper_bound = M;
	this->lower_bound = M/2;
}

bool bTree::find(string key, string *value){
	return this->find_recursive(this->head, key, value, this->height);
}

bool bTree::find_recursive(node h, string key, string* value, int height){
	int j;

	// External nodes have 0 height
	if(height == 0){
		// Check each of the links on the external node for our key
		for(j = 0; j < h->m; j++){
			if(key == h->b[j].key){
				value = h->b[j].value;
				return true;
			}
		}
	}else{
		// Internal nodes have a height greater than 0
		// Loop through each of the links on the node until we find the first key larger than the search key, then recursively check the previous link.
		for(j = 0; j < h->m; j++){
			if((j+1 == h->m) || (key < h->b[j+1].key)){
				return this->find_recursive(h->b[j].next, key, value, height-1);
			}
		}
	}
	return false;
}

void bTree::insert(string key, string value){
	// Try to insert the node recursively
	node u = insert_recursive(this->head, key, value, this->height);
	
	if(u == NULL){
		return;
	}

	node* t = new node(this->upper_bound);
	t->m = 2;
	t->b[0].key = this->head->b[0].key;
	t->b[1].key = u->b[0].key;

	t->b[0].next = this->head;
	t->b[1].next = u;
	this->head = t;
	this->height++;
}

node* bTree::insert_recursive(node* h, string key, string value, int height){
	int i, j;
	entry t;

	if(height == 0){
		for(j = 0; j < h->m; j++){
			if(key < h->b[j].key){
				break;
			}
		}
	}else{
		for(j = 0; j < h->m; j++){
			if((j+1 == h->m) || (key < h->b[j+1].key)){
				node* u;
				u = this->insert_recursive(h->b[j++].next, key, value, height - 1);
				if(u == NULL){
					return NULL;
				}
				t.key = u->b[0].key;
				t.next = u;
				break;
			}
		}
	}

	for(i = h->m; i > j; i--){
		h->b[i] = h->b[i-1];
	}
	h->b[j] = t;

	if(++h->m < this->upper_bound){
		return NULL;
	}else{
		return this->split(h);
	}
}

node* bTree::split(node* h){
	node* t = new node(this->upper_bound);
	for(int j = 0; j < this->lower_bound; j++){
		t->b[j] = h->b[this->lower_bound + j];
	}
	h->m = this->lower_bound;
	t->m = this->lower_bound;
	return t;
}


/*
node* bTree::get_node_by_key(string key){
	return this->get_node_by_key_recursive(this->head, key, this->height);
}

node* bTree::get_node_by_key_recursive(node* h, string key, int height){
	int j;

	// External nodes have 0 height
	if(height == 0){
		// Check each of the links on the external node for our key
		for(j = 0; j < h->m; j++){
			if(key == h->b[j].key){
				return h;
			}
		}
	}else{
		// Internal nodes have a height greater than 0
		// Loop through each of the links on the node until we find the first key larger than the search key, then recursively check the previous link.
		for(j = 0; j < h->m; j++){
			if((j+1 == h->m) || (key < h->b[j+1].key)){
				return this->get_node_by_key_recursive(h->b[j].next, key, height-1);
			}
		}
	}
	return NULL;
}

bool bTree::delete_key(string key){
	node* key_node = this->get_node_by_key(key);
	bool in_node = false;

	for(int i = 0; i < key_node->m; i++){
		if(key_node->b[i] == z){
			in_node = true;
		}
	}

	if(!in_node){
		return false;
	}else{
		if()
	}
}*/