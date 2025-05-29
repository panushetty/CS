// File System
#include<iostream>
#include<string>
#include<vector>
#include <sstream>
#include <algorithm>
using namespace std;

enum NodeType{FIL,DIR};

struct Node{
    string name;
    NodeType type;
    string content;
    vector<Node*> children;
    Node* parent;

    Node(string n,NodeType t){
        name=n;
        type=t;
        content="";
        parent=nullptr;
    }
};

class FileSystem{
private:
    Node* root;

    vector<string> splitPath(const string& path){
        vector<string> parts;
        stringstream ss(path);
        string part;
        while(getline(ss,part,'/')){
            if(!part.empty()) parts.push_back(part);
        }
        return parts;
    }

    Node * findNode(const string& path){
        vector<string> parts=splitPath(path);
        Node* current=root;
        for(const string& part:parts){
            if(current->type!=DIR) return nullptr;
            bool found=false;
            for(Node* child : current->children){
                if(child->name==part){
                    current=child;
                    found=true;
                    break;
                }
            }
            if(!found) return nullptr;
        }
        return current;
    }

public:
    FileSystem(){
        root=new Node("/",DIR);
    }

    void createFile(string path,string content){
        size_t lastSlash=path.find_last_of("/");
        string dirpath=path.substr(0,lastSlash);
        //cout<<"dirpath is "<<dirpath<<endl;
        string filename=path.substr(lastSlash+1);
        //cout<<"filepath is "<<filepath<<endl;
        Node * dir=(dirpath=="")?root:findNode(dirpath);

        if(!dir || dir->type!=DIR){
            cout<<"Directory not found :"<<dirpath<<endl;
            return;
        }

        for(Node* child : dir->children){
            if(child->name==filename){
                cout<<"File already exists"<<endl;
                return;
            }
        }

        Node* file=new Node(filename,FIL);
        file->content=content;
        file->parent=dir;
        dir->children.push_back(file);
        cout << "File created: " << path << endl;

    }

    void mkDir(string path){
        size_t lastSlash=path.find_last_of("/");
        string dirpath=path.substr(0,lastSlash);
        string dirname=path.substr(lastSlash+1);

        Node * dir=(dirpath=="")?root:findNode(dirpath);

        if(!dir || dir->type!=DIR){
            cout<<"Directory not found :"<<dirpath<<endl;
            return;
        }

        for(Node* child : dir->children){
            if(child->name==dirname){
                cout<<"Directory already exists"<<endl;
                return;
            }
        }

        Node* newdir=new Node(dirname,DIR);
        newdir->parent=dir;
        dir->children.push_back(newdir);
        cout << "Directory created: " << path << endl;
    }

    void lister(string path){
        Node * dir=findNode(path);
        if(!dir || dir->type!=DIR){
            cout<<"Directory not found :"<<path<<endl;
            return;
        }
        cout<<"Contents of "<<path<<" : ";
        for(Node* child : dir->children){
            cout<<child->name<<" ";
        }
        cout<<endl;
        return;

    }

    void readFile(string path){
        Node * file=findNode(path);
        if(!file || file->type!=FIL){
            cout<<"File not found :"<<path<<endl;
            return;
        }
        cout<<"Contents of "<<path<<" :"<<file->content<<endl;
        return;

    }

    void deleteFile(string path) {
        Node* node = findNode(path);
        if (!node || node->type != FIL) {
            cout << "File not found: " << path << endl;
            return;
        }
        Node* parent = node->parent;
        parent->children.erase(std::remove(parent->children.begin(), parent->children.end(), node),parent->children.end());
        delete node;
        cout << "File deleted: " << path << endl;
    }

};

int main(){
    FileSystem fs;
    string command,path,content;
    while(true){
        cout<<">";
        cin>>command;
        //cout<<"\nCommand is "<<command<<endl;
        if(command=="exit"){
            break;
        }
        cin>>path;
        if(command=="create"){
            cin.ignore();
            getline(cin,content);
            fs.createFile(path,content);
        }
        if(command=="mkdir"){
            fs.mkDir(path);
        }
        if(command=="list"){
            fs.lister(path);
        }
        if(command=="read"){
            fs.readFile(path);
        }
        if(command=="delete"){
            fs.deleteFile(path);
        }

    }

    return 0;
}
