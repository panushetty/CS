#include <iostream>
#include <vector>
#include <string>
using namespace std;



enum class TokenType {
    NUMBER, OPERATOR, LPAREN, RPAREN,END
};


struct Node{
    TokenType type;
    string value;
    Node* right= nullptr;;
    Node* left= nullptr;;
};


struct Token {
    TokenType type;
    string value;
};

Token token_builder(TokenType type,string value){
   Token tt;
   tt.type=type;
   tt.value=value;
   
   return tt;
}

string gettokenType(TokenType type){
    if (type==TokenType::NUMBER){
        return "NUMBER";
    }
    if (type==TokenType::OPERATOR){
        return "OPERATOR";
    }
    if (type==TokenType::LPAREN){
        return "LPAREN";
    }
    if (type==TokenType::RPAREN){
        return "RPAREN";
    }
    if (type==TokenType::END){
        return "END";
    }
    else{
        return "";
    }
}


vector<Token> lexer(string& input) {
    vector<Token> tokens;
    string currentnumber = "";
    bool hasError = 0;
    string errMessage = "";
    int i;
    for (int i = 0; i < input.size(); i++) {
        char c = input[i];
        
        if (isdigit(c)) {
            currentnumber += c;
        }
        else if (c == '+' || c == '-' || c == '*' || c == '/' || c == '(' || c == ')') {
            if (!currentnumber.empty()) {
                tokens.push_back(token_builder(TokenType::NUMBER,currentnumber));
                currentnumber = "";
            }
            if (c == '+' || c == '-' || c == '*' || c == '/'){
                tokens.push_back(token_builder(TokenType::OPERATOR,string(1, c))); 
            }
            if(c == '(' ){
                tokens.push_back(token_builder(TokenType::LPAREN,string(1, c)));
            }
            if(c == ')' ){
                tokens.push_back(token_builder(TokenType::RPAREN,string(1, c)));
            }
            
        }
        else if (c == ' ') {
            if (!currentnumber.empty()) {
                tokens.push_back(token_builder(TokenType::NUMBER,currentnumber));
                currentnumber = "";
            }
        }
        else {
            hasError = 1;
            errMessage = "Invalid Character '" + string(1, c) + "' at position " + to_string(i + 1);
        }
    }
    if (!currentnumber.empty()) {
        tokens.push_back(token_builder(TokenType::NUMBER,currentnumber));
        currentnumber = "";
    }
    if (hasError) {
        cout << errMessage;
    }
    return tokens;
}

class TokenStream {
private:
    vector<Token> tokens;
    size_t index;
public:
    TokenStream(const vector<Token>& t){
        tokens=t;
        index=0;
    }
    Token getnexttoken();
    Token peeknexttoken();
    bool hasmoretokens();
};


Token TokenStream::getnexttoken(){
    if(index<tokens.size()){
        return tokens[index++];
    }
    else{
        return token_builder(TokenType::END,"END");
    }
};

Token TokenStream::peeknexttoken(){
    if(index<tokens.size()){
        return tokens[index];
    }
    else{
        return token_builder(TokenType::END,"END");
    }
};

bool TokenStream::hasmoretokens(){
    return index < tokens.size();
};


Node* parseExpr(TokenStream& stream);
Node* parseTerm(TokenStream& stream);
Node* parseFactor(TokenStream& stream);

Node* parseExpr(TokenStream& stream){
    Node* leftNode=parseTerm(stream);
    while(stream.hasmoretokens()){
        Token t1=stream.peeknexttoken();  
        //cout<<"In Parse Expr for "<<t1.value<<"\n";
        if(t1.type==TokenType::OPERATOR && (t1.value=="+" || t1.value=="-")){
            Token t1=stream.getnexttoken();
            Node* rightNode=parseExpr(stream);
            Node* opNode=new Node{TokenType::OPERATOR, t1.value, nullptr, nullptr};
            opNode->left=leftNode;
            opNode->right=rightNode;
            leftNode=opNode;
        }
        else{
            break;
        }
    }
    //cout<<"Return from parseExpr"<<leftNode->value<<"\n";
    return leftNode;
}

Node* parseTerm(TokenStream& stream){
    Node* leftNode = parseFactor(stream);
    while(stream.hasmoretokens()){
        Token t1=stream.peeknexttoken();
        //cout<<"In Parse Term for "<<t1.value<<"\n";
        if(t1.type==TokenType::OPERATOR && (t1.value=="*" || t1.value=="/")){
            Token t1=stream.getnexttoken();
            Node* rightNode=parseTerm(stream);
            Node* opNode=new Node{TokenType::OPERATOR, t1.value, nullptr, nullptr};
            opNode->left=leftNode;
            opNode->right=rightNode;
            leftNode=opNode;
        }
        else{
            break;
        }
    }
    //cout<<"Return from parseTerm"<<leftNode->value<<"\n";
    return leftNode;
}

Node* parseFactor(TokenStream& stream){
    Token t1=stream.peeknexttoken();
    //cout<<"In Parse Factor for "<<t1.value<<"\n";
    if (t1.type==TokenType::NUMBER){
        Token t1=stream.getnexttoken();   
        Node* n = new Node{TokenType::NUMBER, t1.value, nullptr, nullptr};
        //cout<<"Return from parseFactor with a number"<<n->value<<"\n";
        return n;
    }
    else if (t1.type==TokenType::LPAREN){
        Token t1=stream.getnexttoken();
        Node* exprNode=parseExpr(stream);
        Token nextToken = stream.peeknexttoken();
        if (nextToken.type==TokenType::RPAREN){
            Token t1=stream.getnexttoken();
            //cout<<"Return from parseFactor with a RPAREN end"<<exprNode->value<<"\n";
            return exprNode;
        }
        else{
            cout<<"Syntax Error: Unbalanced parentheses";
            cout<<""<<exprNode->value<<"\n";
            return nullptr;
        }
    }
    else {
        //cout<<"Return from parseFactor with no value"<<"\n";
        return nullptr;
    }
}


void printTree(Node* node) {
    if (!node) return;
    if(!(node->type==TokenType::NUMBER)){
    cout << "(" << node->value;
    }
    else{
        cout <<" "<< node->value;
    }
    printTree(node->left);
    printTree(node->right);
    if(!(node->type==TokenType::NUMBER)){
    cout << ")";
    }
}


void printNode(Node* node, int depth = 0) {
    if (node) {
        std::cout << string(depth * 2, ' ') << "Type: " << gettokenType(node->type)
                  << ", Value: " << node->value << ", Depth: "<<depth<<endl;
        printNode(node->left, depth + 1);
        printNode(node->right, depth + 1);
    }
}

int main(){
    string input;
    cout << "Enter a String\n";
    getline(cin, input);
    vector<Token> t1=lexer(input);
    
    for(int i=1;i<t1.size();i++){
        if((t1[i].type==t1[i-1].type) && (t1[i].type==TokenType::OPERATOR)){
            cout<<"Syntax Error: Consecutive operators";        
        }
        //gettokenType
    }
    TokenStream ts=TokenStream(t1);
    //cout<<ts.getnexttoken().value<<"      "<<ts.peeknexttoken().value<<"\n";
    Node* node=parseExpr(ts);
    //printNode(node);
    printTree(node);
    
    
    return 0;
}