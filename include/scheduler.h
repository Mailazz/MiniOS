#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <vector>
#include "process.h"

struct SchedulingResult {
    std::vector<Process> processes;
    std::vector<std::pair<int,int>> ganttChart; // (pid, timeSlice length) in execution order
    double avgWaitingTime;
    double avgTurnaroundTime;
    double avgResponseTime;
};

// Each function takes a COPY of the process list so runs don't interfere with each other.
SchedulingResult runFCFS(std::vector<Process> procs);
SchedulingResult runSJF(std::vector<Process> procs);              // non-preemptive
SchedulingResult runPriority(std::vector<Process> procs);          // non-preemptive, lower number = higher priority
SchedulingResult runRoundRobin(std::vector<Process> procs, int quantum);

void printResult(const std::string& algorithmName, const SchedulingResult& result);

#endif
