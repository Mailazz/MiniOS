#ifndef PROCESS_H
#define PROCESS_H

#include <string>

// Process Control Block (PCB)
// Represents everything the "OS" needs to know about a process.
enum class State { NEW, READY, RUNNING, TERMINATED };

struct Process {
    int pid;
    int arrival_time;
    int burst_time;      // total CPU time needed
    int remaining_time;  // used by preemptive algorithms (Round Robin)
    int priority;         // lower number = higher priority
    State state;

    // Metrics computed after scheduling
    int start_time = -1;      // time it first got the CPU (for response time)
    int completion_time = 0;
    int waiting_time = 0;
    int turnaround_time = 0;
    int response_time = -1;

    Process(int pid_, int arrival_, int burst_, int priority_ = 0)
        : pid(pid_), arrival_time(arrival_), burst_time(burst_),
          remaining_time(burst_), priority(priority_), state(State::NEW) {}
};

std::string stateToString(State s);

#endif
