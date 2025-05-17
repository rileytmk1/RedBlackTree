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

// fix and rotate functions created with help of psuedo code from https://www.youtube.com/playlist?list=PL9xmBV_5YoZNqDI8qfOZgzbqahCUmUEin
// and https://www.youtube.com/watch?v=5IBxA-bZZH8&list=PL9xmBV_5YoZNqDI8qfOZgzbqahCUmUEin&index=3 (from resources listed on canvas)

// credit for delete and deleteFix functions to Michael Sambol's youtube playlist on redblack tree. (same playlist as above). Edited to deal with NULLs.

//function prototypes
void add(rbnode* &root, rbnode* &r, rbnode* parent, int value);
void print(rbnode* &root, int depth);
void fixTree(rbnode* &r, rbnode* node);
void leftRotate(rbnode* &root, rbnode* n);
void rightRotate(rbnode* &root, rbnode* n);
void search(rbnode* &root, int num);
void transplant(rbnode* p, rbnode* c, rbnode* &root);
void DELETE(rbnode* & root, int d, rbnode* &r);
void deleteFix(rbnode* x, rbnode* xParent, rbnode* &r); 

int main()
{
  //initialize root of the tree
  rbnode* root = NULL;
  char input[10];
  while (strcmp(input, "QUIT") != 0){
    cout << "ADD, PRINT, SEARCH, DELETE or QUIT: ";
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

    else if (strcmp(input, "SEARCH") == 0){
      int s;
      cout << "What number do you want to search for? " << endl;
      cin >> s;
      cin.ignore();
      search(root, s);
    }

    else if (strcmp(input, "DELETE") == 0){
      int d;
      cout << "What number do you want to delete? ";
      cin >> d;
      cin.ignore();
      DELETE(root, d, root);
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
  //get left child (will become new parent)
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

void transplant(rbnode* p, rbnode* c, rbnode* &root)
{
  if (p->parent == NULL){
    root = c;
  }
  else if (p == p->parent->left){
    p->parent->left = c;
  }
  else{
    p->parent->right = c;
  }
  if (c != NULL){
    c->parent = p->parent;
  }
  
}


void search(rbnode* &root, int num)
{
  // gone through tree and number was not found
  if (root == NULL) {
    cout << "Number not found." << endl;
    return;
  }

  //current node equal to number searched
  if (root->data == num){
    cout << "Number found: " << num << " is in the tree." << endl;
    return;
  }

  //search left subtree
  if (num < root->data){
    search(root->left, num);
  }
  //search right
  else{
    search(root->right, num);
  }
}

void DELETE (rbnode* & root, int d, rbnode* &r){
  // gone through tree and node is not found
  if (root == NULL){
    cout << "Number Not found." << endl;
    return;
  }
  //found node to delete
  if (root->data == d){
    rbnode* x = NULL; //replacement node
    rbnode* y = root; //node to be deleted
    
    
    char ogColor = y->color; //save color before transplants mess things around

    // Case: 1 only right child or no children
    if (root->left == NULL){
      x = root->right;
      transplant(root, root->right, r); // move right child into place of deleted node
    }

    // Case: 2 only left child
    else if (root->right == NULL){
      x = root->left;
      transplant(root, root->left, r); //move left child into place of deleted node
    }

    // Case: 3 has 2 children
    else {
      // save left subtree and color before transplants mess thins around.
      rbnode* ogLeft = root->left;
      ogColor = root->color;

      //find the inorder successor
      y = root->right;
      while (y->left != NULL){
	y = y->left;
      }
      
      char successorColor = y->color;
      x = y->right; // successor's right child

      // x's parent if x exists
      if (x != NULL){
	x->parent = y->parent;
      }

      // successor is not direct right child of current node
      if (y->parent != root){
	transplant(y, y->right, r); // move successor's right child into successor's place 
	y->right = root->right;
	if (y->right != NULL){
	  y->right->parent = y;
	}
      }

      else {
	//if successor is direct right child, update parent
	if (x != NULL){
	  x->parent = y;
	}
      }
      
      
      // move successor into deleted node's position
      transplant(root, y, r);

      //reattatch left subtree
      y->left = ogLeft;
      if (y->left != NULL){
	y->left->parent = y;
      }
      y->color = ogColor;

      ogColor = successorColor;
    }
    
    // fix tree if original color is black
    if (ogColor == 'B'){
      cout << "fixed called" << endl;
      rbnode* xParent = NULL;
      if (x != NULL){
	xParent = x->parent;
      }
      else{
	xParent = y->parent; //in case x is NULL
      }
      deleteFix(x, xParent, r);
    }
    
  }
  // search left if value is less than current node
  else if (d < root->data){
    DELETE(root->left, d, r);
  }

  // search right otherwise
  else{
    DELETE(root->right, d, r);
  }
  
  
}

//fixes tree to maintain red black properties
void deleteFix(rbnode* x, rbnode* xParent, rbnode* &r)
{
  //fix until x is root or x is red
  while (x != r && (x == NULL || x->color == 'B')){
    rbnode* parent = NULL;

    // in case x is NULL
    if (x != NULL){
      parent = x->parent;
    }
    else{
      parent = xParent;
    }
    

    if (parent == NULL){
      break;
    }

    // if x is a left child
    if (x == parent->left){
      rbnode* sibling = parent->right;

      //Case: 1 sibling is red
      if(sibling != NULL && sibling->color == 'R'){
	// make sibling black and parent red
	sibling->color = 'B';
	parent->color = 'R';
	// rotate around parent
	leftRotate(r, parent);
	// get new sibling
	sibling = parent->right;
	
      }
      // Case 2: sibling and both children are black or NULL
      if ((sibling == NULL) || ((sibling->left == NULL || sibling->left->color == 'B') && (sibling->right == NULL || sibling->right->color == 'B'))){
	if (sibling != NULL){
	  sibling->color = 'R'; //sibling becomes red
	}
	// move to parent and continue fixing tree
	x = parent;
	xParent = x->parent;
            
      }
      // Case 3/4: sibling has at least one red child
      else{
	//Case 3: sibling's right is black, left is red
	if (sibling->right == NULL || sibling->right->color == 'B'){
	  if (sibling->left != NULL){
	    sibling->left->color = 'B'; //left child becomes black
	  }
	  sibling->color = 'R'; //sibling becomes red
	  rightRotate(r, sibling); //rotate around sibling
	  sibling = parent->right; // get new sibling after rotation
	
	}
	// Case 4: sibling's right is red
	if (sibling != NULL){
	  sibling->color = parent->color;
	  parent->color = 'B';
	  if (sibling->right != NULL){
	    sibling->right->color = 'B';
	  }
	  leftRotate(r, parent);
	}
	//end loop after case 4
	x = r;
	xParent = NULL;
      
      }
    }
    else{ // x is a right child. Cases are just flipped for the other side
      rbnode* sibling = parent->left;
            
      if (sibling != NULL && sibling->color == 'R'){
	sibling->color = 'B';
	parent->color = 'R';
	rightRotate(r, parent);
	sibling = parent->left;
	
      }

      if ((sibling == NULL) || ((sibling->right == NULL || sibling->right->color == 'B') && (sibling->left == NULL || sibling->left->color == 'B'))){
	if (sibling != NULL){
	  sibling->color = 'R';
	}
	x = parent;
	xParent = x->parent;
      }
      else{
	if (sibling->left == NULL || sibling->left->color == 'B'){
	  if (sibling->right != NULL){
	    sibling->right->color = 'B';
	  }
	  sibling->color = 'R';
	  leftRotate(r, sibling);
	  sibling = parent->left;
	  
	}
	if (sibling != NULL){
	  sibling->color = parent->color;
	  parent->color = 'B';
	  if (sibling->left != NULL){
	    sibling->left->color = 'B';
	  }
	  rightRotate(r, parent);
	}
	x = r;
	xParent = NULL;
      }
    }
    
     
  }
  // Make sure x is black when everything is finished
  if (x != NULL){
    x->color = 'B';
  }
}
