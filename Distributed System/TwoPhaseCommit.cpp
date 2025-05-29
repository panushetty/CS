#include<iostream>
#include<vector>
#include<thread>
#include <random>
#include<mutex>

using namespace std;


enum class NodeState{COMMITTED,ABORTED,READY};

mutex mtx;
vector<bool> votes(4, false);

class Node{
public:
    int id;
    NodeState state;

    Node(int id) : id(id), state(NodeState::READY) {}

    bool vote() {
        random_device rd;
        mt19937 gen(rd());
        uniform_int_distribution<> dist(1, 100);
        bool success = dist(gen) > 20; // 80% chance of success
        {
            lock_guard<mutex> lock(mtx);
            cout << "Node " << id << ": Voting " << (success ? "yes" : "no") << endl;
        }
        return success;
    }

    void execute(bool commit) {
        state = commit ? NodeState::COMMITTED : NodeState::ABORTED;
        lock_guard<mutex> lock(mtx);
        cout << "Node " << id << ": " << (commit ? "Committed" : "Aborted") << endl;
    }

};

class Coordinator{
    vector<Node*> nodes;
public:
    Coordinator(vector<Node*> nodes_list){
        nodes=nodes_list;
    }

    bool runPhase1(){
        for (Node* node : nodes) {
            lock_guard<mutex> lock(mtx);
            cout << "Coordinator: Sending prepare to node " << node->id << endl;
        }
        bool all_yes = true;
        for (int i=1 ;i<=3;i++){
            if(!votes[i]){
                all_yes=false;
                break;
            }
        }
        return all_yes;
    }

    void runTransaction(){
        //it runs the transation so first thing is phase 1
        bool success=runPhase1();
        {
            lock_guard<mutex> lock(mtx);
            cout << "Coordinator: " << (success ? "All nodes voted yes, sending commit" : "Node failed, sending abort") << endl;
        }
        for(Node* node :nodes){
            node->execute(success);
        }
        {
            lock_guard<mutex> lock(mtx);
            cout << "Transaction " << (success ? "committed successfully" : "aborted due to failure") << endl;
        }
    }

};



void nodeThread(Node* node) {
    bool vote = node->vote();
    lock_guard<mutex> lock(mtx);
    votes[node->id] = vote;
}

int main(){
    vector<Node*> nodes;
    for(int i=1;i<=3;i++){
        nodes.push_back(new Node(i));
    }
    Coordinator coord(nodes);
    vector<thread> threads;
    for(Node* node:nodes){
        threads.emplace_back(nodeThread,node);
    }

    for (thread& t : threads) {
        t.join();
    }
    coord.runTransaction();
    for (Node* node : nodes) {
        delete node;
    }
    nodes.clear();
    return 0;
}
