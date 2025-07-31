#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
using namespace std;

struct Process {
    int id;
    int arrival_time;
    int execution_time;
    int priority;
    int completion_time;
    int turnaround_time;
    int waiting_time;
};

// Sort by arrival time for FCFS
bool compareArrival(Process a, Process b) {
    return a.arrival_time < b.arrival_time;
}

int main() {
    int n;
    cout << "Enter number of processes: ";
    cin >> n;

    vector<Process> processes(n);

    cout << "Enter processes (id arrival_time execution_time priority):\n";
    for(int i = 0; i < n; i++) {
        cin >> processes[i].id >> processes[i].arrival_time >> processes[i].execution_time >> processes[i].priority;
    }

    sort(processes.begin(), processes.end(), compareArrival);

    queue<Process*> readyQueue;
    int currentTime = 0, completed = 0;
    double totalTurnaround = 0;

    int i = 0; // index for arrival tracking

    while(completed < n) {
        // Add newly arrived processes to queue
        while(i < n && processes[i].arrival_time <= currentTime) {
            readyQueue.push(&processes[i]);
            i++;
        }

        if(!readyQueue.empty()) {
            Process* p = readyQueue.front();
            readyQueue.pop();

            if(currentTime < p->arrival_time)
                currentTime = p->arrival_time;

            currentTime += p->execution_time;
            p->completion_time = currentTime;
            p->turnaround_time = p->completion_time - p->arrival_time;
            p->waiting_time = p->turnaround_time - p->execution_time;

            totalTurnaround += p->turnaround_time;
            completed++;
        } else {
            currentTime++;
        }
    }

    cout << "\nID\tArrival\tExec\tPriority\tCompletion\tTurnaround\tWaiting\n";
    for(auto &p : processes) {
        cout << p.id << "\t" << p.arrival_time << "\t" 
             << p.execution_time << "\t" << p.priority << "\t\t"
             << p.completion_time << "\t\t" << p.turnaround_time 
             << "\t\t" << p.waiting_time << "\n";
    }

    cout << "\nMean Turnaround Time = " << (totalTurnaround / n) << endl;

    return 0;
}
