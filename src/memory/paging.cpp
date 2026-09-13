#include "memory/paging.h"

#include <algorithm>
#include <list>
#include <unordered_set>

PagingSimulator::PagingSimulator(int frames)
    : frameCount(frames) {
}

int PagingSimulator::fifo(const std::vector<int>& pages) {
    if (frameCount <= 0) {
        return static_cast<int>(pages.size());
    }

    std::unordered_set<int> frames;
    std::list<int> queue;

    int pageFaults = 0;

    for (int page : pages) {
        // Page is already in memory
        if (frames.find(page) != frames.end()) {
            continue;
        }

        // Page fault
        pageFaults++;

        // Memory is full — remove the oldest page
        if (static_cast<int>(frames.size()) >= frameCount) {
            int oldest = queue.front();
            queue.pop_front();
            frames.erase(oldest);
        }

        // Load the new page
        frames.insert(page);
        queue.push_back(page);
    }

    return pageFaults;
}

int PagingSimulator::lru(const std::vector<int>& pages) {
    if (frameCount <= 0) {
        return static_cast<int>(pages.size());
    }

    std::list<int> frames;
    int pageFaults = 0;

    for (int page : pages) {
        auto it = std::find(frames.begin(), frames.end(), page);

        // Page is already in memory
        if (it != frames.end()) {
            // Move recently-used page to the back
            frames.erase(it);
            frames.push_back(page);
            continue;
        }

        // Page fault
        pageFaults++;

        // Memory is full — remove least recently used page
        if (static_cast<int>(frames.size()) >= frameCount) {
            frames.pop_front();
        }

        // Add page as most recently used
        frames.push_back(page);
    }

    return pageFaults;
}