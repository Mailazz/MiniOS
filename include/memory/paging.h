#ifndef PAGING_H
#define PAGING_H

#include <vector>

class PagingSimulator {
private:
    int frameCount;

public:
    PagingSimulator(int frames);

    int fifo(const std::vector<int>& pages);
    int lru(const std::vector<int>& pages);
};

#endif
