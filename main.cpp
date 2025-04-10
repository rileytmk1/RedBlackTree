#include <iostream>
#include <cstring>

using namespace std;


struct rbnode{
  rbnode* parent;
  rbnode* left;
  rbnode* right;
  int color;
  int data;
};

void add(rbnode* root, int value);
void print(rbnode* root);

int main()
{
  rbnode* root = NULL;
  char input[10];
  cout << "ADD, PRINT, or QUIT: ";
  cin.get(input,10);
  cin.get();
  while (strcmp(input, "QUIT") != 0){
    if (strcmp(input, "ADD") == 0){
      cout << "file or manual: ";
      char input2[20];
      cin.get(input2, 20);
      cin.get();
      if (strcmp(input2, "manual") == 0){

      }
    }
  }

}

void add(rbnode* &root, int value)
{

}

void print(rbnode* &root, int depth = 0){
  if (root == NULL){
    return;
  }
  print(root->right, depth + 1);

  for (int i = 0; i < depth; i++){
    cout << "   "l
  }

  cout << root->value << "(" << root->color << ")" << endl;

  print(root->left, depth + 1);
}
