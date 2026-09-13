#include "../include/scheduler.h"
#include <algorithm>
#include <iostream>
#include <iomanip>
#include <queue>
#include <cstdint>

static void finalizeMetrics(std::vector<Process>& procs, SchedulingResult& result) {
    double totalWait = 0, totalTurnaround = 0, totalResponse = 0;
    for (auto& p : procs) {
        p.turnaround_time = p.completion_time - p.arrival_time;
        p.waiting_time = p.turnaround_time - p.burst_time;
        p.response_time = p.start_time - p.arrival_time;
        totalWait += p.waiting_time;
        totalTurnaround += p.turnaround_time;
        totalResponse += p.response_time;
    }
    result.processes = procs;
    result.avgWaitingTime = totalWait / procs.size();
    result.avgTurnaroundTime = totalTurnaround / procs.size();
    result.avgResponseTime = totalResponse / procs.size();
}

SchedulingResult runFCFS(std::vector<Process> procs) {
    SchedulingResult result;
    std::sort(procs.begin(), procs.end(), [](const Process& a, const Process& b) {
        return a.arrival_time < b.arrival_time;
    });

    int currentTime = 0;
    for (auto& p : procs) {
        if (currentTime < p.arrival_time) currentTime = p.arrival_time;
        p.start_time = currentTime;
        result.ganttChart.push_back({p.pid, p.burst_time});
        currentTime += p.burst_time;
        p.completion_time = currentTime;
    }
    finalizeMetrics(procs, result);
    return result;
}

SchedulingResult runSJF(std::vector<Process> procs) {
    SchedulingResult result;
    int n = procs.size();
    std::vector<bool> done(n, false);
    int currentTime = 0, completed = 0;

    while (completed < n) {
        int idx = -1, shortest = INT32_MAX;
        for (int i = 0; i < n; i++) {
            if (!done[i] && procs[i].arrival_time <= currentTime && procs[i].burst_time < shortest) {
                shortest = procs[i].burst_time;
                idx = i;
            }
        }
        if (idx == -1) { currentTime++; continue; } // CPU idle, no process has arrived yet

        procs[idx].start_time = currentTime;
        result.ganttChart.push_back({procs[idx].pid, procs[idx].burst_time});
        currentTime += procs[idx].burst_time;
        procs[idx].completion_time = currentTime;
        done[idx] = true;
        completed++;
    }
    finalizeMetrics(procs, result);
    return result;
}

SchedulingResult runPriority(std::vector<Process> procs) {
    SchedulingResult result;
    int n = procs.size();
    std::vector<bool> done(n, false);
    int currentTime = 0, completed = 0;

    while (completed < n) {
        int idx = -1, bestPriority = INT32_MAX;
        for (int i = 0; i < n; i++) {
            if (!done[i] && procs[i].arrival_time <= currentTime && procs[i].priority < bestPriority) {
                bestPriority = procs[i].priority;
                idx = i;
            }
        }
        if (idx == -1) { currentTime++; continue; }

        procs[idx].start_time = currentTime;
        result.ganttChart.push_back({procs[idx].pid, procs[idx].burst_time});
        currentTime += procs[idx].burst_time;
        procs[idx].completion_time = currentTime;
        done[idx] = true;
        completed++;
    }
    finalizeMetrics(procs, result);
    return result;
}

SchedulingResult runRoundRobin(std::vector<Process> procs, int quantum) {
    SchedulingResult result;
    std::sort(procs.begin(), procs.end(), [](const Process& a, const Process& b) {
        return a.arrival_time < b.arrival_time;
    });

    std::queue<int> readyQueue;
    int n = procs.size();
    std::vector<bool> inQueue(n, false);
    int currentTime = procs[0].arrival_time;
    int completed = 0;

    // seed queue with anything arriving at time 0 / first arrival
    for (int j = 0; j < n; j++) {
        if (procs[j].arrival_time <= currentTime && !inQueue[j]) {
            readyQueue.push(j);
            inQueue[j] = true;
        }
    }

    while (completed < n) {
        if (readyQueue.empty()) {
            currentTime++;
            for (int j = 0; j < n; j++)
                if (!inQueue[j] && procs[j].arrival_time <= currentTime) {
                    readyQueue.push(j); inQueue[j] = true;
                }
            continue;
        }

        int idx = readyQueue.front(); readyQueue.pop();
        if (procs[idx].start_time == -1) procs[idx].start_time = currentTime;

        int slice = std::min(quantum, procs[idx].remaining_time);
        result.ganttChart.push_back({procs[idx].pid, slice});
        currentTime += slice;
        procs[idx].remaining_time -= slice;

        // enqueue anyone who arrived during this slice (before re-adding current process)
        for (int j = 0; j < n; j++)
            if (!inQueue[j] && procs[j].arrival_time <= currentTime) {
                readyQueue.push(j); inQueue[j] = true;
            }

        if (procs[idx].remaining_time > 0) {
            readyQueue.push(idx);
        } else {
            procs[idx].completion_time = currentTime;
            completed++;
        }
    }
    finalizeMetrics(procs, result);
    return result;
}

void printResult(const std::string& algorithmName, const SchedulingResult& result) {
    std::cout << "\n=== " << algorithmName << " ===\n";
    std::cout << std::left << std::setw(6) << "PID" << std::setw(10) << "Arrival"
              << std::setw(8) << "Burst" << std::setw(12) << "Completion"
              << std::setw(10) << "Waiting" << std::setw(12) << "Turnaround"
              << std::setw(10) << "Response" << "\n";

    // sort by pid for readable output
    auto sorted = result.processes;
    std::sort(sorted.begin(), sorted.end(), [](const Process& a, const Process& b){ return a.pid < b.pid; });

    for (const auto& p : sorted) {
        std::cout << std::left << std::setw(6) << p.pid << std::setw(10) << p.arrival_time
                  << std::setw(8) << p.burst_time << std::setw(12) << p.completion_time
                  << std::setw(10) << p.waiting_time << std::setw(12) << p.turnaround_time
                  << std::setw(10) << p.response_time << "\n";
    }

    std::cout << "\nGantt chart: ";
    for (auto& slice : result.ganttChart) std::cout << "[P" << slice.first << ":" << slice.second << "] ";

    std::cout << "\n\nAvg Waiting Time:    " << std::fixed << std::setprecision(2) << result.avgWaitingTime;
    std::cout << "\nAvg Turnaround Time: " << result.avgTurnaroundTime;
    std::cout << "\nAvg Response Time:   " << result.avgResponseTime << "\n";
}
