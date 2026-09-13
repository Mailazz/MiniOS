#include <iostream>
#include <vector>

#include "../include/process.h"
#include "../include/scheduler.h"
#include "../include/memory/paging.h"

// Sample CPU workload
std::vector<Process> sampleProcesses() {
    return {
        Process(1, 0, 8, 2),
        Process(2, 1, 4, 1),
        Process(3, 2, 9, 3),
        Process(4, 3, 5, 4),
        Process(5, 4, 2, 5)
    };
}

// Sample virtual-memory page reference string
std::vector<int> samplePages() {
    return {
        7, 0, 1, 2, 0, 3, 0,
        4, 2, 3, 0, 3, 2
    };
}

void runMemorySimulation() {
    std::cout << "\n=========================================\n";
    std::cout << " MiniOS - Memory Management Simulator\n";
    std::cout << "=========================================\n";

    const int frameCount = 3;
    auto pages = samplePages();

    std::cout << "\nPage Reference String:\n";

    for (int page : pages) {
        std::cout << page << " ";
    }

    std::cout << "\n\nNumber of Frames: " << frameCount << "\n";

    PagingSimulator simulator(frameCount);

    int fifoFaults = simulator.fifo(pages);
    int lruFaults = simulator.lru(pages);

    std::cout << "\nPage Replacement Results:\n";
    std::cout << "-----------------------------------------\n";
    std::cout << "Algorithm       Page Faults\n";
    std::cout << "-----------------------------------------\n";
    std::cout << "FIFO            " << fifoFaults << "\n";
    std::cout << "LRU             " << lruFaults << "\n";
    std::cout << "-----------------------------------------\n";

    if (lruFaults < fifoFaults) {
        std::cout << "\nLRU produced fewer page faults for this workload.\n";
    } else if (fifoFaults < lruFaults) {
        std::cout << "\nFIFO produced fewer page faults for this workload.\n";
    } else {
        std::cout << "\nBoth algorithms produced the same number of page faults.\n";
    }
}

int main() {
    std::cout << "=========================================\n";
    std::cout << " MiniOS - Operating System Simulator\n";
    std::cout << "=========================================\n";

    // =====================================
    // CPU SCHEDULING
    // =====================================

    std::cout << "\n\n=========================================\n";
    std::cout << " CPU Scheduling Simulator\n";
    std::cout << "=========================================\n";

    auto procs = sampleProcesses();

    std::cout << "\nWorkload:\n";
    std::cout << "PID  Arrival  Burst  Priority\n";

    for (const auto& p : procs) {
        std::cout << p.pid << "    "
                  << p.arrival_time << "        "
                  << p.burst_time << "      "
                  << p.priority << "\n";
    }

    auto fcfs = runFCFS(procs);
    printResult("First-Come, First-Served (FCFS)", fcfs);

    auto sjf = runSJF(procs);
    printResult("Shortest Job First (SJF)", sjf);

    auto prio = runPriority(procs);
    printResult("Priority Scheduling", prio);

    const int quantum = 3;

    auto rr = runRoundRobin(procs, quantum);
    printResult(
        "Round Robin (quantum=" + std::to_string(quantum) + ")",
        rr
    );

    // =====================================
    // CPU SCHEDULING COMPARISON
    // =====================================

    std::cout << "\n=========================================\n";
    std::cout << " CPU Scheduling Comparison\n";
    std::cout << "=========================================\n";

    std::cout << "Algorithm        AvgWait   AvgTurnaround   AvgResponse\n";

    std::cout << "FCFS             "
              << fcfs.avgWaitingTime << "\t   "
              << fcfs.avgTurnaroundTime << "\t\t"
              << fcfs.avgResponseTime << "\n";

    std::cout << "SJF              "
              << sjf.avgWaitingTime << "\t   "
              << sjf.avgTurnaroundTime << "\t\t"
              << sjf.avgResponseTime << "\n";

    std::cout << "Priority         "
              << prio.avgWaitingTime << "\t   "
              << prio.avgTurnaroundTime << "\t\t"
              << prio.avgResponseTime << "\n";

    std::cout << "Round Robin      "
              << rr.avgWaitingTime << "\t   "
              << rr.avgTurnaroundTime << "\t\t"
              << rr.avgResponseTime << "\n";

    // =====================================
    // MEMORY MANAGEMENT
    // =====================================

    runMemorySimulation();

    std::cout << "\n=========================================\n";
    std::cout << " MiniOS simulation completed.\n";
    std::cout << "=========================================\n";

    return 0;
}