#include <iostream>
#include <thread>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <string>
using namespace std;

class ThreadSafeQueue{
    private:
        queue<string> q;
        mutex btex;
        condition_variable cv;
    public:
        ThreadSafeQueue(){
            
        }
        void push(string data){
            unique_lock<mutex> lock(btex);
            q.push(data);
            lock.unlock();
            cv.notify_one();
        }
        
        string pop(){
            unique_lock<mutex>lock(btex);
            cv.wait(lock, [this] { return !q.empty(); });
            string msg=q.front();
            q.pop();
            lock.unlock();
            return msg;
            
        }
};

void sender(ThreadSafeQueue& q){
    string msg="Hello from Process 1";
    q.push(msg);
}

void receiver(ThreadSafeQueue& q ){
    string msg=q.pop();
    cout<<"Message received :"<<msg;
}

int main() {
    ThreadSafeQueue q;  // One queue
    thread t1(sender, ref(q));  
    thread t2(receiver, ref(q));
    t1.join();
    t2.join();
    return 0;
}