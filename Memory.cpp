// Mary Fan
// CSCI 340 Final Project

#include "Memory.hpp"

Memory::Memory(long long ram) {
    memory_.push_back({0, ram-1, ram, 0});
}

bool Memory::AllocateMemory(long long size, int pid) {
    long long largest = size - 1;
    int location = -1;
    for (int i = 0; i < memory_.size(); i++) {
        if (memory_[i].pid_ == 0 && memory_[i].size_ > largest) {
            largest = memory_[i].size_;
            location = i;
        }
    }
    if (location == -1) {
        std::cout << "Out of memory: unable to create process" << std::endl;
        return false;
    } else {
        MEM free_memory = memory_[location];
        memory_.erase(memory_.begin() + location);
        MEM new_process_mem{free_memory.lower_, free_memory.lower_ + size - 1, size, pid};
        MEM new_free_memory{free_memory.lower_ + size, free_memory.upper_, free_memory.upper_ - free_memory.lower_ + size, 0};
        memory_.push_back(new_process_mem);
        memory_.push_back(new_free_memory);
        return true;
    }
}

void Memory::DeallocateMemory(int pid) {
    for (int i = 0; i < memory_.size(); i++) {
        if (memory_[i].pid_ == pid) {
          memory_[i].pid_ = 0;
        }
    }
    // combine adjacent memory gaps
}

void Memory::ShowMemory() {
    std::cout << "[Memory]" << std::endl;
    for (int i = 0; i < memory_.size(); i++) {
        if (memory_[i].pid_ != 0) {
            std::cout 
            << "    PID: " 
            << memory_[i].pid_ 
            << " " 
            << memory_[i].lower_
            << " - "
            << memory_[i].upper_
            << std::endl;
        }
    }
}