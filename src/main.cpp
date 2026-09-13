#include <iostream>
#include <vector>
#include "../include/process.h"
#include "../include/scheduler.h"

// Sample workload: 5 processes with varied arrival times, burst times, and priorities.
// Feel free to change these or read them from a file/stdin.
std::vector<Process> sampleProcesses() {
    return {
        Process(1, 0, 8, 2),
        Process(2, 1, 4, 1),
        Process(3, 2, 9, 3),
        Process(4, 3, 5, 4),
        Process(5, 4, 2, 5)
    };
}

int main() {
    std::cout << "=========================================\n";
    std::cout << " MiniOS - CPU Scheduling Simulator\n";
    std::cout << "=========================================\n";

    auto procs = sampleProcesses();

    std::cout << "\nWorkload:\n";
    std::cout << "PID  Arrival  Burst  Priority\n";
    for (auto& p : procs)
        std::cout << p.pid << "    " << p.arrival_time << "        "
                   << p.burst_time << "      " << p.priority << "\n";

    auto fcfs = runFCFS(procs);
    printResult("First-Come, First-Served (FCFS)", fcfs);

    auto sjf = runSJF(procs);
    printResult("Shortest Job First (SJF)", sjf);

    auto prio = runPriority(procs);
    printResult("Priority Scheduling", prio);

    int quantum = 3;
    auto rr = runRoundRobin(procs, quantum);
    printResult("Round Robin (quantum=" + std::to_string(quantum) + ")", rr);

    std::cout << "\n=========================================\n";
    std::cout << " Comparison Summary (lower is better)\n";
    std::cout << "=========================================\n";
    std::cout << "Algorithm        AvgWait   AvgTurnaround   AvgResponse\n";
    std::cout << "FCFS             " << fcfs.avgWaitingTime << "\t   " << fcfs.avgTurnaroundTime << "\t\t" << fcfs.avgResponseTime << "\n";
    std::cout << "SJF              " << sjf.avgWaitingTime << "\t   " << sjf.avgTurnaroundTime << "\t\t" << sjf.avgResponseTime << "\n";
    std::cout << "Priority         " << prio.avgWaitingTime << "\t   " << prio.avgTurnaroundTime << "\t\t" << prio.avgResponseTime << "\n";
    std::cout << "Round Robin      " << rr.avgWaitingTime << "\t   " << rr.avgTurnaroundTime << "\t\t" << rr.avgResponseTime << "\n";

    return 0;
}
