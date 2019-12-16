// Mary Fan
// CSCI 340 Final Project

#ifndef PROCESS_HPP
#define PROCESS_HPP

#include <iostream>
#include <vector>

class Process {
public:
    Process(int parent_pid, int pid, long long size, int priority);
    int GetParentPid();
    int GetPid();
    long long GetSize();
    int GetPriority();
    void AddChild(int child_pid);
    void RemoveChild(int child_pid);
    std::vector<int> GetChildren();
    bool HasZombies();
    void KillProcess();

private: 
    int parent_pid_;
    int pid_;
    long long size_;
    int priority_;
    std::vector<int> children_;
    int zombies_ = 0;
    bool status_ = true;
};

#endif