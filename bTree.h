#include <stdio.h>
#include <iostream>
#include <sstream>

using namespace std;

struct entry{
  string key;
  string value;
};

class bTreeNode{
private:
  int lower_bound;
  int upper_bound;
  bool is_leaf;
  bTreeNode **children;
  void set_boundaries(int M);
  void insert_non_full(string key, string value);
  void split_child(int i, bTreeNode* y);
  bool remove(string key);
  bool remove_from_leaf(int index);
  bool remove_from_non_leaf(int index);
  entry* get_predecessor(int index);
  entry* get_successor(int index);
  void fill(int index);
  void borrow_from_previous(int index);
  void borrow_from_next(int index);
  void merge(int index);
  int find_key(string key); 

public:
  bTreeNode(int M, bool is_leaf);

  // Walks the subtree
  void traverse(stringstream*);
  bool find(string key, string* value);
  entry** entries;
  int number_of_keys;
  friend class bTree;
};

class bTree {
private:
  bTreeNode* root;
  int lower_bound;
  int upper_bound;

  void set_boundaries(int M);
public:
  //Constructor
  bTree(int M);

  //Destructor
  ~bTree();

  //Inserts the value pair into the tree
  void insert(string key, string value);

  // Searches for the key in the tree, if found, it returns
  // true else returns false.
  // If the key is found, it stores the value in the *value variable
  bool find(string key, string *value);

  //Searches for the key and deletes it. Returns true if deletion was
  //successful, returns false if the key wasn't found
  bool delete_key(string key);

  // concatenates the contents of the tree from an inorder traversal
  // into an output stream with each string item followed by a 
  // newline character in the stream
  string toStr();

};
