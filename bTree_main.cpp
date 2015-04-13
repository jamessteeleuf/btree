#include<iostream>
#include"bTree.h"

using namespace std;
int main () {

	int op = 0;
	int size;
	string key;
	string value;
	cin>>size;

	bTree * BT = new bTree(size);
	while(op < 4) {
		cin >> op;
		switch(op) {
			case 0://Insert
				cin>>key;
				cin>>value;
				BT->insert(key,value);
				break;
			case 1://Delete
				cin>>key;
				BT->delete_key(key);
				break;
			case 2://Find
				cin>>key;
				if(BT->find(key,&value))
					cout<<value<<endl;
				else
					cout<<"Key Not Found\n";
				break;
			case 3://toString
				cout<<BT->toStr();
				break;
		}
	}
	return 0;
}
