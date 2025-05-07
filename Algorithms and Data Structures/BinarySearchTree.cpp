// Online C++ compiler to run C++ program online
#include <iostream>
using namespace std;

struct Node {
  int data;
  Node* left;
  Node* right;

  Node(int value) {
      data=value;
      left=nullptr;
      right=nullptr;
  }
};



class BST{
    private:
    Node* node=nullptr;
    void inorder(Node* node){
        if(node!=nullptr){
          inorder(node->left);
          cout<<node->data<<" ";
          inorder(node->right);
        }
    }

    Node* deleteNode(Node* node, int value){
        if(node==nullptr){
            return node;
        }
        if(node->data==value){
            if(node->left==nullptr){
                //No Children
                Node *temp=node->right;
                delete(node);
                return temp;
            }
            if(node->right==nullptr){
                Node *temp=node->left;
                delete(node);
                return temp;
            }

            Node* succ=successor(node);
            node->data=succ->data;
            node->right=deleteNode(node->right, succ->data);
        }
        else if (node->data>value){
            node->left=deleteNode(node->left,value);
        }else{
            node->right=deleteNode(node->right,value);
        }
        return node;
    }

    Node* insertNode(Node* node, int value ){
        if (node==nullptr){
            return new Node(value);
        }
        if (node->data==value){
            return node;
        }
        if (node->data>value){
            node->left=insertNode(node->left,value);
        }else{
            node->right=insertNode(node->right,value);
        }

        return node;
    }

    Node* successor(Node* node){
        node=node->right;
        while(node!=nullptr && node->left!=nullptr){
            node=node->left;
        }
        return node;
    }
    public:
    void deleten(int value){
        cout<<"After Delete "<<value<<" :";
        node=deleteNode(node,value);
        inorder(node);
        cout<<"\n";
    }

    void insert(int value){
        cout<<"After insert "<<value<<" :";
        node=insertNode(node,value);
        inorder(node);
        cout<<"\n";
    }





};


int main() {

    BST tree;
    tree.insert(50);
    tree.insert(34);
    tree.insert(67);
    tree.insert(500);
    tree.insert(1);
    tree.insert(0);
    tree.insert(2);
    tree.insert(44);
    tree.insert(46);
    tree.deleten(34);
    tree.deleten(500);
    tree.deleten(0);



    return 0;
}
