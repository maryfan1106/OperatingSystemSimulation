// Mary Fan
// CSCI 340 Final Project

#ifndef OPERATINGSYSTEM_HPP
#define OPERATINGSYSTEM_HPP

#include "Memory.hpp"
#include "Process.hpp"

#include <iostream>
#include <unordered_map> 
#include <deque>
#include <vector>
#include <sstream>

class OS {
public:
    OS(long long ram, int disks);
    void Start();

private:
    void A(int priority, long long size);
    void fork();
    void exit();
    void wait();
    void d(int disk);
    void D(int disk);
    void S(std::string show);

    // operating system specific methods
    void AddToReady(std::pair<int, int>);
    void RemoveFromReady(int pid);
    void Terminate(int pid);

    int pid_ = 1;
    std::unordered_map<int, Process> PCB_;
    std::vector<std::deque<std::pair<int, int>>> disks_;
    std::deque<std::pair<int, int>> ready_queue_;
    std::vector<std::pair<int, int>> waiting_;
    Memory memory_;

};

#endif