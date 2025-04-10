#include <iostream>
#include <cstring>
#include <fstream>

using namespace std;


struct rbnode{
  rbnode* parent;
  rbnode* left;
  rbnode* right;
  int color; // 0 represents red, 1 represents black
  int data;
};

void add(rbnode* &root, rbnode* &r, rbnode* parent, int value);
void print(rbnode* &root, int depth);
void fixTree(rbnode* &r, rbnode* node);
void leftRotate();
void rightRotate();

int main()
{
  rbnode* root = NULL;
  char input[10];
  while (strcmp(input, "QUIT") != 0){
    cout << "ADD, PRINT, or QUIT: ";
    cin.get(input,10);
    cin.get();
    if (strcmp(input, "ADD") == 0){
      cout << "file or manual: ";
      char input2[20];
      cin.get(input2, 20);
      cin.get();
      if (strcmp(input2, "manual") == 0){
	int n;
	cout << "Enter a number to add: ";
	cin >> n;
	cin.ignore();
	add(root, root, NULL, n);
      }
      else if (strcmp(input2, "file") == 0){
	ifstream inputFile("nums.txt");
	int num;
	while(inputFile >> num){
	  add(root, root, NULL, num);
	  cout << "Added: " << num << endl;
	}
	inputFile.close();
      }
    }
    else if (strcmp(input, "PRINT") == 0){
      print(root, 0);
    }
  }

}

void add(rbnode* &root, rbnode* &r, rbnode* parent, int value)
{
  if (root == NULL){
    root = new rbnode();
    root->data = value;
    root->left = NULL;
    root->right = NULL;
    root->parent = parent;
    root->color = 0;
    fixTree(r, root);
    return;
  }
  else{
    parent = root;
    if (value < root->data){
      add(root->left, r, parent, value);
    }
    else{
      add(root->right, r, parent, value);
    }
  }
}

void print(rbnode* &root, int depth = 0){
  if (root == NULL){
    return;
  }
  print(root->right, depth + 1);

  for (int i = 0; i < depth; i++){
    cout << "   ";
  }

  cout << root->data << "(" << root->color << ")" << endl;

  print(root->left, depth + 1);
}

void fixTree(rbnode* &r, rbnode* node)
{
  if (node == r){
    node->color = 1;
  }
  /*
  else if (node->parent->color == 0){
    node->pare
  }
  */

  
  return;
}
