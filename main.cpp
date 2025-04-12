#include <iostream>
#include <cstring>
#include <fstream>

using namespace std;


struct rbnode{
  rbnode* parent;
  rbnode* left;
  rbnode* right;
  char color; // R = red, B = black, NULL is considered BLACK
  int data;
};

//fix and rotate functions created with help of psuedo code from https://www.youtube.com/playlist?list=PL9xmBV_5YoZNqDI8qfOZgzbqahCUmUEin

//function prototypes
void add(rbnode* &root, rbnode* &r, rbnode* parent, int value);
void print(rbnode* &root, int depth);
void fixTree(rbnode* &r, rbnode* node);
void leftRotate(rbnode* &root, rbnode* n);
void rightRotate(rbnode* &root, rbnode* n);

int main()
{
  //initialize root of the tree
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
  // once reached empty node set value to be added to that node
  if (root == NULL){
    root = new rbnode();
    root->data = value;
    root->left = NULL;
    root->right = NULL;
    root->parent = parent;
    root->color = 'R';
    fixTree(r, root); //fix violations after every insertion
    return;
  }
  else{
    parent = root; //keep track of parent
    if (value < root->data){ //go down left subtree if less than current node
      add(root->left, r, parent, value);
    }
    else{
      add(root->right, r, parent, value); //go down right subtree if greater than current node
    }
  }
}

//print tree sideways
void print(rbnode* &root, int depth = 0){
  //stop when reached end of subtree
  if (root == NULL){
    return;
  }
  //recurse through right subtree
  print(root->right, depth + 1);

  
  for (int i = 0; i < depth; i++){ // prints indents based on current depths
    cout << "   ";
  }

  cout << root->data << "(" << root->color << ")" << endl; //print value and its color

  print(root->left, depth + 1); //recurse through left subtree
}

void leftRotate(rbnode* &root, rbnode* x)
{
  // get right child (will become new parent)
  rbnode* y = x->right;
  x->right = y->left; // y's left subtree becomes x's right subtree
  if (y->left != NULL){
    y->left->parent = x; //update parent pointer if subtree exists
  }
  y->parent = x->parent; //link y to x's parent
  if (x->parent == NULL){ // if x is root y becomes root
    root = y;
  }
  else if (x == x->parent->left){
    x->parent->left = y;
  }
  else{
    x->parent->right = y;
  }
  y->left = x; //put x on to the left of y
  x->parent = y;
}

void rightRotate(rbnode* &root, rbnode* y)
{
  //get left child (will beocme new parent)
  rbnode* x = y->left;
  //x right subtree becomes y's left
  y->left = x->right;
  if(x->right != NULL){
    x->right->parent = y;
  }
  x->parent = y->parent; //link x to y's parent
  if(y->parent == NULL){
    root = x;
  }
  else if (y == y->parent->left){
    y->parent->left = x;
  }
  else{
    y->parent->right = x;
  }
  x->right = y; //put y on x's right
  y->parent = x;
}

void fixTree(rbnode* &r, rbnode* node)
{
  // loop until node is the root or parent is black (no violations)
  while (node != r && node->parent->color == 'R'){
    //parent is grandparent's left
    if (node->parent == node->parent->parent->left){
      rbnode* uncle = node->parent->parent->right;
      if (uncle != NULL && uncle->color == 'R'){ // parent and uncle are red
	// Recolor
	node->parent->color = 'B';
	uncle->color = 'B';
	node->parent->parent->color = 'R';
	//set to grandparent to check for new violation
	node = node->parent->parent;
      }
      //uncle is black
      else{
	// if node is on the right, "straighten" path
	if (node == node->parent->right){
	  node = node->parent;
	  leftRotate(r, node); //left rotate to make "straight" left path
	}
	//right rotate grandparent to balance
	node->parent->color = 'B';
	node->parent->parent->color = 'R';
	rightRotate(r, node->parent->parent);
      }
    }
    //parent is grandparent's right
    else{
      rbnode* uncle = node->parent->parent->left;
      // uncle and parent are red
      if (uncle != NULL && uncle->color == 'R'){
	//Recolor
	node->parent->color = 'B';
	uncle->color = 'B';
	node->parent->parent->color = 'R';
	//set to grandparent to check for new violation
	node = node->parent->parent;
      }
      // uncle is black
      else{
	//if node is on the left "straighten" path
	if (node == node->parent->left){
	  node = node->parent;
	  rightRotate(r, node); //rotate right to make "straight" right path
	}

	node->parent->color = 'B';
	node->parent->parent->color = 'R';
	leftRotate(r, node->parent->parent); // left rotate grandparent to balance
      }
    }
    
  }
  //root stays black
  r->color = 'B';

}
