#include <stdio.h>
#include <iostream>

class bTreeNode{
  std::string* keys;
  int minimum_degree;
  bTreeNode **child_pointers;
  int number_of_keys;
  bool is_leaf;

  bTreeNode(int minimum_degree, bool leaf);
  void traverse();
  bTreeNode *search(std::string key);
  friend class bTree;
};

class bTree {
private:
  //Your private stuff to implement the B Tree
  bTreeNode *root;
  int minimum_degree;
public:
  //Constructor
  bTree();

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