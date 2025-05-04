//Vigenere Cipher
#include <iostream>
#include<vector>
#include<string>
#include<queue>
using namespace std;


string generatekeytext(string message,string key){
    int x=message.size();
    for(int i=0;;i++){
        if(i==x){
            i=0;
        }
        if(message.size()==key.size()){
            break;
        }
        key.push_back(key[i]);
        
    }
    return key;
}

string encrypt(string message,string key){
    string encrypt="";
    for(int i=0;i<message.size();i++){
        if (isalpha(message[i])){
            char c =(message[i]+key[i])%26;
            c=c+'A';
            encrypt.push_back(c);
        }
        else{
            encrypt.push_back(message[i]);
        }
    }
    
    return encrypt;
}


string decrypt(string message,string key){
    string decrypt="";
    for(int i=0;i<message.size();i++){
        if (isalpha(message[i])){
            char c =(message[i]-key[i]+26)%26;
            c=c+'A';
            decrypt.push_back(c);
        }
        else{
            decrypt.push_back(message[i]);
        }
        
    }
    
    return decrypt;
}


int main() {
    string message,key;
    cout<<"Enter Message\n";
    getline(cin, message);
    cout<<"Enter Key\n";
    getline(cin, key);
    string keytext=generatekeytext(message,key);
    //cout<<keytext;
    string encrypt1=encrypt(message,keytext);
    cout<<"Encrypted text :"<<encrypt1<<endl;
    string decrypt1=decrypt(encrypt1,keytext);
    cout<<"Decrypted text :"<<decrypt1<<endl;
    
}