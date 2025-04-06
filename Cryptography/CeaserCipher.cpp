/*
Cryptography: Implement a Caesar Cipher
Task: Write functions to encrypt/decrypt a message with a user-defined shift (e.g., shift 3 on "ATTACK AT DAWN").
Instructions: Handle uppercase letters, preserve non-letters, test it.
Tools: Any language (C++ fits your style).
*/


#include <iostream>
#include <cctype>
#include<string>
using namespace std;

void clear_cin(){
    cin.clear();
    cin.ignore(1000, '\n');
}

string encrypt_decrypt(string msg,int options,int disp){
    disp=disp*options;
    disp=(disp%26+26)%26;
    string outmsg="";
    for (int i=0;i<msg.size();i++){
        if (isupper(msg[i])){
            outmsg+=char((msg[i]-'A'+disp)%26+'A');
        }
        else if(islower(msg[i])){
            outmsg+=char((msg[i]-'a'+disp)%26+'a');
        }
        else{
            outmsg+=msg[i];
        }
        
    }

    return outmsg;
}

int main() {
    // Write C++ code here
    string msg,outmsg;
    int options,disp;
    cout << "CAESARIS CIPHERI\n";
    cout << "Enter your message\n";
    getline(cin,msg);
    //cout<<msg;
    while(1){
        cout<<"\nWhat do you want to do ?\n";
        cout<<"Input '1' to encrypt\n";
        cout<<"Input '2' to decrypt\n";
        cin>>options;
        if (options==1){
            cout<<"Input shift for Encryption\n";
            cin>>disp;
            outmsg=encrypt_decrypt(msg,1,disp);
            cout<<outmsg;
            break;
        }
        else if (options==2){
            cout<<"Input shift for Decryption\n";
            cin>>disp;
            outmsg=encrypt_decrypt(msg,-1,disp);
            cout<<outmsg;
            break;
        }
        else{
            cout<<"Invalid number. Try again.\n";
            clear_cin();
        }
    }

    return 0;
}

