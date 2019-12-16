// Mary Fan
// CSCI 340 Final Project

#ifndef MEMORY_HPP
#define MEMORY_HPP

#include <vector>
#include <iostream>

class Memory {
public:
    Memory(long long ram);
    bool AllocateMemory(long long size, int pid);
    void DeallocateMemory(int pid);
    void ShowMemory();

private:
    class MEM {
    public:
        MEM(long long lower, long long upper, long long size, int pid)
        : lower_(lower), upper_(upper), size_(size), pid_(pid) {}
        long long lower_;
        long long upper_;
        long long size_;
        int pid_;
    };
    std::vector<MEM> memory_;     
};

#endif
