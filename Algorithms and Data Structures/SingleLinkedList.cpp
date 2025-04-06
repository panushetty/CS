/*
Algorithms and Data Structures: Implement a Singly Linked List
Task: Create a singly linked list with insert (at start) and delete (from end) functions. Test with 10, 20, 30.
Instructions: Use a Node struct/class, print after operations.
Tools: Any language (C++ works well).
*/


#include <iostream>
#include <cctype>
using namespace std;

void clear_cin(){
    cin.clear();
    cin.ignore(1000,'\n');
}

struct Node{
    int data;
    Node* next;    
};

class SingleLinkedList { 
    private:
        Node* head;
    public:
        SingleLinkedList(){
            head=nullptr;  
            //cout<<"In Constructor\n";
        }
        void insertAtStart(int value);
        void deleteAtEnd();
        void printList();
};


void SingleLinkedList::printList(){
    Node* current=head;
    if(head==nullptr){
        cout<<"Empty List\n";
        return;
    }
    cout<<"List : ";
    while(current!=nullptr){
        cout<<" "<<current->data;
        current=current->next;
    }
    cout<<endl;
}

void SingleLinkedList::insertAtStart(int value){
    //cout<<"In Add Method\n";
    Node* newNode = new Node();
    newNode->data=value;
    newNode->next=head;
    head=newNode;
}

void SingleLinkedList::deleteAtEnd(){
    //cout<<"In Delete Method\n";
    if(head==nullptr){
        cout<<"There is nothing to delete\n";
        return;
    }
    else if (head->next==nullptr){
        cout<<"Deleted "<<head->data;
        delete head;
        head=nullptr;
    }
    else{
        Node* current=head;
        Node* previous=nullptr;
        while(current->next!=nullptr){
            previous=current;
            current=current->next;
        }
        previous->next=nullptr;
        cout<<"Deleted "<<current->data<<"\n";
        delete current;
        cout<<"New Updated List\n";
        
    }
    printList();
}



int main() {
    // Write C++ code here
    int options,num;
    SingleLinkedList list;
    cout<<"Linked List\n";
    while(options!=4){
        
        cout<<"\nOptions\n";
        cout<<"1. Add\n";
        cout<<"2. Delete\n";
        cout<<"3. Print your list\n";
        cout<<"4. Exit\n";
        cout<<"Enter your options\n";
        if (!(cin>>options)){
            clear_cin();
        }
        switch(options){
                case 1:
                    cout<<"Options selected : ADD\n";
                    cout<<"Input a number\n";
                    if (!(cin>>num)){
                        cout<<"Not a number. Try again\n";
                        clear_cin();
                    }else{
                        list.insertAtStart(num);
                    }
                    break;
                case 2:
                    cout<<"Options selected : DELETE\n";
                    list.deleteAtEnd();
                    break;
                case 3:
                    cout<<"Options selected : PRINT\n";
                    list.printList();
                    break;
                case 4:
                    cout<<"Exiting....\n";
                    break;
                default:
                    cout<<"Invalid option. Try again\n";
                    break;
        }
                
        
        
    }
    

    return 0;
}