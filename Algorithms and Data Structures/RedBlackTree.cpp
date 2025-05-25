#include<iostream>
#include<string>
using namespace std;
enum Color {RED,BLACK};

string getColor(Color color){
    if (color==BLACK) return "BLACK";
    if (color==RED) return "RED";
}

struct Node{
    int value;
    Color color;
    Node* left;
    Node* right;
    Node* parent;

    Node(int val){
        value=val;
        color=RED;
        left=nullptr;
        right=nullptr;
        parent=nullptr;
    }
};

class RBTree{
private:
    Node* root;
    Node* NIL;
public:
    RBTree(){
        NIL=new Node(0);
        NIL->color=BLACK;
        NIL->left=NIL->right=NIL;
        NIL->parent=NIL;
        root=NIL;
    }

    int checkBlackHeight(Node* node) {
        if (node == NIL)
            return 1; // NIL is black
        int leftHeight = checkBlackHeight(node->left);
        int rightHeight = checkBlackHeight(node->right);
        if (leftHeight == -1 || rightHeight == -1 || leftHeight != rightHeight)
            return -1; // Invalid
        return leftHeight + (node->color == BLACK ? 1 : 0);
    }

    void verifyProperties() {
        int blackHeight = checkBlackHeight(root);
        if (blackHeight != -1)
            cout << "\nBlack-height property holds" << endl;
        else
            cout << "\nBlack-height property violated" << endl;
    }

    void rightRotate(Node* x){
        Node* y = x->left;
        x->left = y->right;
        if (y->right != NIL)
            y->right->parent = x;
        y->parent = x->parent;
        if (x->parent == NIL)
            root = y;
        else if (x == x->parent->right)
            x->parent->right = y;
        else
            x->parent->left = y;
        y->right = x;
        x->parent = y;
    }

    void leftRotate(Node* x){
        Node* y=x->right;
        x->right=y->left;
        if(y->left!=NIL){
            y->left->parent=x;
        }
        y->parent=x->parent;
        if(x->parent==NIL){
            root=y;
        }
        else if(x->parent->left==x){
            x->parent->left=y;
        }
        else{
            x->parent->right=y;
        }
        y->left=x;
        x->parent=y;

    }

    void fixViolation(Node* node){
        //if node is root
        if(node==root){
            node->color=BLACK;
            return;
        }
        //if parent is Black no violation
        if(node->parent==NIL || node->parent->color==BLACK) return;
        //if parent is red get grandparent and uncle
        Node* parent=node->parent;
        Node* grandparent=parent->parent;
        if (grandparent==NIL){
            parent->color=BLACK;
            return;
        }
        if(grandparent!=NIL){
            Node* uncle=(parent==grandparent->left)?grandparent->right:grandparent->left;
            if(uncle!=NIL && uncle->color==RED){
                parent->color=BLACK;
                uncle->color=BLACK;
                grandparent->color=RED;
                fixViolation(grandparent);
            }
            else {
                //cout<<"uncle is black";
                //uncle is black hence rotations are needed
                if(parent==grandparent->left){
                    if(node==parent->left){
                        //Left-Left
                        parent->color=BLACK;
                        grandparent->color=RED;
                        rightRotate(grandparent);
                    }
                    else{
                        //Left-Right
                        leftRotate(parent);
                        node = parent;
                        parent->color=BLACK;
                        grandparent->color=RED;
                        rightRotate(grandparent);
                    }
                }
                else{
                    if(node==parent->right){
                        //Right-Right
                        //cout<<"Right-Right";
                        parent->color=BLACK;
                        grandparent->color=RED;
                        leftRotate(grandparent);
                    }
                    else{
                        //Right-Left
                        rightRotate(parent);
                        node = parent;
                        parent->color=BLACK;
                        grandparent->color=RED;
                        leftRotate(grandparent);
                    }

                }
            }
        }
        root->color=BLACK;

    }

    //insert method
    void insert(int val){
        Node* newNode=new Node(val);
        newNode->left=NIL;
        newNode->right=NIL;
        if (root==NIL){
            //if root is empty
            //cout<<"Root is empty";
            newNode->parent=NIL;
            root=newNode;
            root->color=BLACK;
        }
        else{
            //cout<<"Root is not empty";
            //if root is not empty
            //insert node like BST
            Node* current=root;
            Node* parent=nullptr;
            while(current!=NIL){
                //Traversing till NIL hits.
                parent=current;
                if(val<current->value){
                    current=current->left;
                }
                else{
                    current=current->right;
                }
            }
            newNode->parent=parent;
            if(val<parent->value){
                parent->left=newNode;
            }
            else{
                parent->right=newNode;
            }
            //call fix violation
            //printInOrder(root);
            fixViolation(newNode);

        }

    }

    void printInOrder(Node* node){
        if(node!=NIL){
            printInOrder(node->left);
            cout << node->value<<" ";
            printInOrder(node->right);
        }
    }

    void printInOrder(){
        cout<<"\nAfter Insert ";
        printInOrder(root);
    }


};

int main() {
    RBTree tree;
    int values[] = {10, 20, 30, 40, 50, 25};
    for (int val : values) {
        tree.insert(val);
        tree.printInOrder(); // Debugging output
    }
    tree.verifyProperties(); // Check black-height
    return 0;
}
