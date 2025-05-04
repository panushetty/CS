// Round Robin Scheduler
#include <iostream>
#include<vector>
#include<string>
#include<queue>
using namespace std;

struct Process {
    string id;           // Process ID (e.g., "P1")
    int arrivalTime;    // When the process arrives
    int burstTime;      // Total CPU time required
    int remainingTime;  // Remaining CPU time during simulation
    int completionTime; // Time when process finishes
    
    Process(string pid,int arrival,int burst){
        id=pid;
        arrivalTime=arrival;
        burstTime=burst;
        remainingTime=0;
        completionTime=0;
    }
};


void simulateRoundRobin(vector<Process> processes,int quantum){
    queue<Process> processq;
    int completed,currentTime=0;
    vector<string> timeline;
    vector<bool> added(processes.size(), false);
    
    while(completed<processes.size()){
        for(int i=0;i<processes.size();i++){
            if(!added[i] && processes[i].arrivalTime<=currentTime){
                processq.push(processes[i]);
                added[i]=true;
            }
        }
        
        if (processq.empty()) {
            currentTime++;
            continue;
        }
        
        Process current = processq.front();
        processq.pop();
        int index=-1;
        for(int i=0;i<processes.size();i++){
            if(processes[i].id==current.id){
                index=i;
                break;
            }
        }
        int execTime = min(quantum, processes[index].remainingTime);
        processes[index].remainingTime-=execTime;
        
        timeline.push_back(to_string(currentTime) + "-" + to_string(currentTime + execTime) + ": " + current.id);
        
        currentTime += execTime;
        
        if (processes[index].remainingTime == 0) {
            processes[index].completionTime = currentTime;
            completed++;
        } else {
            processq.push(processes[index]);
        }
        
    }
    double totalWaiting = 0, totalTurnaround = 0;
    cout << "Timeline: ";
    
    for (const string& entry : timeline) {
        cout << entry;
        if (&entry != &timeline.back()) cout << ", ";
    }
    
    for (Process& p : processes) {
        int turnaround = p.completionTime - p.arrivalTime;
        int waiting = turnaround - p.burstTime;
        totalWaiting += waiting;
        totalTurnaround += turnaround;
        cout << p.id << ": Waiting Time = " << waiting << ", Turnaround Time = " << turnaround << endl;
    }
    
    cout << "Average Waiting Time: " << totalWaiting / processes.size() << endl;
    cout << "Average Turnaround Time: " << totalTurnaround / processes.size() << endl;
    
}

int main() {
    vector<Process> processes;
    int n,arrival,burst,quantum;
    cout<<"Enter the number of processes\n";
    cin>>n;
    
    for (int i=0;i<n;i++){
        string id="P"+to_string(i+1);
        processes.push_back(Process(id, 0, 0));
    }
    cout<<"Enter arrival time of processes in a single line\n";
    for (int i=0;i<n;i++){
        cin>>arrival;
        processes[i].arrivalTime=arrival;
    }
    
    cout<<"Enter burst time of processes in a single line\n";
    for (int i=0;i<n;i++){
        cin>>burst;
        processes[i].burstTime=burst;
        processes[i].remainingTime=burst;
    }
    cout << "Enter time quantum: \n";
    cin >> quantum;
    

    cout << "Processes:" << endl;
    for (const Process& p : processes) {
        cout << "ID: " << p.id 
             << ", Arrival: " << p.arrivalTime 
             << ", Burst: " << p.burstTime 
             << ", Remaining: " << p.remainingTime 
             << ", Completion: " << p.completionTime << endl;
    }
    simulateRoundRobin(processes, quantum);
    
    
}