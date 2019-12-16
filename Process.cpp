// Mary Fan
// CSCI 340 Final Project

#include "Process.hpp"

Process::Process(int parent_pid, int pid, long long size, int priority): parent_pid_(parent_pid), pid_(pid), size_(size), priority_(priority) {
}

int Process::GetParentPid() {
    return parent_pid_;
}

int Process::GetPid() {
    return pid_;
}

long long Process::GetSize() {
    return size_;
}

int Process::GetPriority() {
    return priority_;
}

void Process::AddChild(int child_pid) {
  children_.push_back(child_pid);
}

std::vector<int> Process::GetChildren() {
    return children_;
}

bool Process::HasZombies() {
  if (zombies_ > 0) {
    zombies_--;
    return true;
  } else {
    return false;
  }
}

void Process::RemoveChild(int child_pid) {
  for (int i = 0; i < children_.size(); i++) {
    if (children_[i] == child_pid) {
      std::cout << "removing: " <<  children_[i] << " for " << "process: " << pid_  << std::endl;
      children_.erase(children_.begin() + i);
      zombies_++;
      break;
    }
  }
}

void Process::KillProcess() {
    status_ = false;
}