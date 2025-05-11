#include <iostream>
#include <thread>
#include <vector>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <chrono>
#include <atomic>

using namespace std;

struct Message {
    int id; // ID of the node sending the message
    bool election; // True if this is an election message
};

class Node {
public:
    int id;
    bool active;
    int leader_id;
    thread t;
    static vector<queue<Message>> messages; // Shared message queues (one per node)
    static mutex mtx;
    static condition_variable cv;
    static mutex cout_mtx; // For synchronized output
    static atomic<int> ready_count; // Count of threads ready to wait
    static condition_variable ready_cv; // Signal when all threads are ready

    Node(int id) : id(id), active(true), leader_id(-1) {}

    void run(int num_nodes) {
        {
            unique_lock<mutex> lock(mtx);
            ready_count++;
            ready_cv.notify_all();
        }

        while (active) {
            num_nodes=messages.size();
            unique_lock<mutex> lock(mtx);
            // Signal readiness before waiting
            if (ready_count < num_nodes) {
                ready_cv.wait(lock, [this, num_nodes] { return ready_count >= num_nodes || !active; });
            }
            cv.wait(lock, [this] { return !messages[id].empty() || !active; });
            if (!active) break;

            Message msg = messages[id].front();
            messages[id].pop();
            lock.unlock();

            if (msg.election) {
                int next = (id + 1) % num_nodes;
                if (msg.id != id) {
                    // Forward higher ID
                    lock.lock();
                    messages[next].push({max(msg.id,id), true});
                    lock.unlock();
                    cv.notify_all();
                    /*
                    {
                        lock_guard<mutex> cout_lock(cout_mtx);
                        cout << "Node " << id << " forwarding election message "<<"to Node "<<next<<"with ID " << max(msg.id,id)<<"But min is :"<<min(msg.id,id)<<endl;
                    }
                    */
                } else if (msg.id == id) {
                    // My ID came back, I'm the leader
                    leader_id = id;
                    {
                        lock_guard<mutex> cout_lock(cout_mtx);
                        cout << "Node " << id << " elected as leader. Hum First Hum First\n";
                    }
                    // Notify all nodes
                    for (int i = 0; i < num_nodes; i++) {
                        if (i != id) {
                            lock.lock();
                            messages[i].push({id, false});
                            lock.unlock();
                            cv.notify_all();
                        }
                    }
                }
            }
            else {
                // Leader announcement
                leader_id = msg.id;
                {
                    lock_guard<mutex> cout_lock(cout_mtx);
                    cout << "Node " << id << ": Leader is " << leader_id << endl;
                }
            }
        }
        {
            lock_guard<mutex> cout_lock(cout_mtx);
            cout << "Node " << id << " exiting."<<endl;
        }
    }
};



vector<queue<Message>> Node::messages;
mutex Node::mtx;
condition_variable Node::cv;
mutex Node::cout_mtx;
atomic<int> Node::ready_count(0);
condition_variable Node::ready_cv;

int main() {
    int num_nodes = 5;
    vector<Node> nodes;
    Node::messages.resize(num_nodes);

    // Create nodes
    for (int i = 0; i < num_nodes; i++) {
        nodes.emplace_back(i);
    }

    // Start node threads
    for (int i = 0; i < num_nodes; i++) {
        nodes[i].t = thread(&Node::run, ref(nodes[i]), num_nodes);
    }

    // Wait for all threads to be ready
    {
        unique_lock<mutex> lock(Node::mtx);
        Node::ready_cv.wait(lock, [&] { return Node::ready_count >= num_nodes; });
    }

    // First election: each node sends its ID
    cout << "First Election:\n";
    for (int i = 0; i < num_nodes; i++) {
        unique_lock<mutex> lock(Node::mtx);
        int next = (i + 1) % num_nodes;
        Node::messages[next].push({i, true});
        lock.unlock();
        /*
        {
            lock_guard<mutex> cout_lock(Node::cout_mtx);
            cout << "Main: Sent election message from Node " << i << " to Node " << next << endl;
        }
        */
        Node::cv.notify_all();
    }

    // Wait for the first election to complete
    this_thread::sleep_for(chrono::seconds(1));

    // Simulate leader failure (Node 4)
    int leader_id = 4;
    {
        lock_guard<mutex> cout_lock(Node::cout_mtx);
        cout << "Leader " << leader_id << " has failed\n";
    }
    nodes[leader_id].active = false;
    {
        unique_lock<mutex> lock(Node::mtx);
        Node::cv.notify_all();
    }
    nodes[leader_id].t.join();


    //Node::ready_count=0;
    // Second election
    cout << "Second Election:\n";
    for (int i = 0; i < num_nodes; i++) {
        nodes[i].leader_id=-1;
    }
    Node::messages.pop_back();
    nodes.pop_back();

    for (int i = 0; i < num_nodes; i++) {
        if (nodes[i].active) {
            unique_lock<mutex> lock(Node::mtx);
            int next = (i + 1) % num_nodes;
            if (nodes[next].active) {
                Node::messages[next].push({i, true});
                {
                    lock_guard<mutex> cout_lock(Node::cout_mtx);
                    cout << "Main: Sent election message from Node " << i << " to Node " << next << endl;
                }
            }
            lock.unlock();
            Node::cv.notify_all();
        }
    }

    // Wait for the second election to complete
    this_thread::sleep_for(chrono::seconds(1));

    // Terminate all threads
    for (int i = 0; i < num_nodes; i++) {
        if (nodes[i].active) {
            nodes[i].active = false;
            {
                unique_lock<mutex> lock(Node::mtx);
                Node::cv.notify_all();
            }
            nodes[i].t.join();
        }
    }

    return 0;
}
