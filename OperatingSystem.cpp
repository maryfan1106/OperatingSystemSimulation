// Mary Fan
// CSCI 340 Final Project

#include "OperatingSystem.hpp"

OS::OS(long long ram, int disks): memory_{ram} {
    for (int i = 0; i < disks; i++) {
      disks_.push_back({});
    }
}

void OS::Start() {
    std::string input;
    while(std::getline(std::cin, input)) {
        std::stringstream input_stream(input);
        std::string command;
        input_stream >> command;
        if (command == "A") {
            try {
                std::string priority_string;
                std::string size_string;
                input_stream >> priority_string;
                input_stream >> size_string;
                int priority = stoi(priority_string);
                long long size = stoll(size_string);
                A(priority, size);
            } catch(const std::exception &e) {
                std::cout << e.what() << std::endl;
            }
        } else if (command == "fork") {
            fork();
        } else if (command == "exit") {
            exit();
        } else if (command == "wait") {
            wait();
        } else if (command == "d") {
            try {
                std::string disk_string;
                input_stream >> disk_string;
                int disk = stoi(disk_string);
                d(disk);
            } catch (const std::exception &e) {
                std::cout << e.what() << std::endl;
            }
        } else if (command == "D") {
            try {
                std::string disk_string;
                input_stream >> disk_string;
                int disk = stoi(disk_string);
                D(disk);
            } catch (const std::exception &e) {
                std::cout << e.what() << std::endl;
            }
        } else if (command == "S") {
            std::string show;
            input_stream >> show;
            S(show);
        } else {
            std::cout << "Invalid Command" << std::endl;
        }
    }
}

void OS::A(int priority, long long size) {
    if (memory_.AllocateMemory(size, pid_)) {
        Process new_process{0, pid_, size, priority};
        PCB_.insert({pid_, new_process});
        AddToReady({pid_, priority});
        pid_++;
    }
}

void OS::fork() {
    if (ready_queue_.empty()) {
        std::cout << "CPU is idle, can't fork" << std::endl;
    } else {
        auto itr = PCB_.find(ready_queue_.front().first);
        Process curr_process = itr->second;

        // create child process
        if (memory_.AllocateMemory(curr_process.GetSize(), pid_)) {
            Process new_process{curr_process.GetPid(), pid_, curr_process.GetSize(), curr_process.GetPriority()};
            PCB_.insert({pid_, new_process});
            AddToReady({pid_, new_process.GetPriority()});

            // add child to parent process
            itr->second.AddChild(pid_); 
            pid_++;
        }
    }
}

void OS::exit() {
    if (ready_queue_.empty()) {
        std::cout << "CPU is idle, can't exit" << std::endl;
    } else {
        // cascading termination: remove all children from all queues and memory (marking them as terminated)
        auto itr = PCB_.find(ready_queue_.front().first);
        Process curr_process = itr->second;
        Terminate(curr_process.GetPid());
        // add zombie for curr_process parent_pid
        auto parent_itr = PCB_.find(curr_process.GetParentPid());
        if(parent_itr != PCB_.end()){
            parent_itr->second.RemoveChild(curr_process.GetPid());
            Process parent_process = parent_itr->second;
            // check if parent was waiting, if yes, release parent
            for (int i = 0; i < waiting_.size(); i++) {
                if (waiting_[i].first == parent_process.GetPid()) {
                    if (parent_process.HasZombies()) {
                        waiting_.erase(waiting_.begin() + i);
                        AddToReady({parent_process.GetPid(), parent_process.GetPriority()});
                    }
                }
            }
        }
       
    }   
}

void OS::wait() {
    if (ready_queue_.empty()) {
        std::cout << "CPU is idle, can't wait" << std::endl;
    } else {
        auto itr = PCB_.find(ready_queue_.front().first);
        Process curr_process = itr->second;

        if (!curr_process.HasZombies()) {
            std::pair<int, int> curr_process = ready_queue_.front();
            RemoveFromReady(curr_process.first);
            waiting_.push_back(curr_process);
        }

    }
}

void OS::d(int disk) {
    if (disk >= disks_.size()) {
        std::cout << "Invalid disk" << std::endl;
    } else {
        if (ready_queue_.empty()) {
            std::cout << "CPU is idle, no process avaliable to use disk" << std::endl;
        } else {
            std::pair<int, int> curr_process = ready_queue_.front();
            RemoveFromReady(curr_process.first);
            disks_[disk].push_back(curr_process);
        }
    }
}

void OS::D(int disk) {
    if (disk >= disks_.size()) {
        std::cout << "Invalid disk" << std::endl;
    } else {
        if (disks_[disk].empty()) {
            std::cout << "Disk is currently idle" << std::endl;
        } else {
            std::pair<int, int> disk_process = disks_[disk].front();
            disks_[disk].pop_front();
            AddToReady(disk_process);
        }
    }
}

void OS::S(std::string show) {
    if (show == "r") {
        if (ready_queue_.empty()) {
            std::cout << "CPU is idle" << std::endl;
        } else {
            std::cout 
                << "[CPU]" << std::endl
                << "    PID: " << ready_queue_.front().first << "  Priority: " << ready_queue_.front().second << std::endl;
            std::cout << "[Ready Queue]" << std::endl;
            for (int i = 1; i < ready_queue_.size(); i++) {
                std::cout << "    PID: " << ready_queue_[i].first << "  Priority: " << ready_queue_[i].second << std::endl;
            }
        }
    } else if (show == "i") {
        std::cout << "[Disks]" << std::endl;
        for(int i = 0; i < disks_.size(); i++) {
            std::cout << "    Disk " << i << ": " << std::endl;
            if (disks_[i].empty()) {
                std::cout << "        Disk is idle" << std::endl;               
            } else {
                for (int j = 0; j < disks_[i].size(); j++) {
                  std::cout << "        PID: " << disks_[i][j].first << "  Priority: " << disks_[i][j].second << std::endl;   
                }
            }
        }        
    } else if (show == "m") {
        memory_.ShowMemory();
    } else {
        std::cout << "Invalid Command" << std::endl;
    }
}

void OS::AddToReady(std::pair<int, int> new_process) {
    if (ready_queue_.empty() || ready_queue_.front().second < new_process.second) {
        ready_queue_.push_front(new_process);
    } else {
        ready_queue_.push_back(new_process);
    }
}

void OS::RemoveFromReady(int pid) {
    // will usually be the first item (current running process)
    for (int i = 0; i < ready_queue_.size(); i++) {
        if (ready_queue_[i].first == pid) {
            ready_queue_.erase(ready_queue_.begin() + i);
        }
    }
    // find highest priority
    int highest = -1;
    int location = -1;
    for (int i = 0; i < ready_queue_.size(); i++) {
        if (ready_queue_[i].second > highest) {
            highest = ready_queue_[i].second;
            location = i;
        }
    }
    if (location != -1) {
        // move highest priority to front
        std::pair<int, int> new_highest = ready_queue_[location];
        ready_queue_.erase(ready_queue_.begin() + location);
        ready_queue_.push_front(new_highest);
    }
}

void OS::Terminate(int pid) {
    auto itr = PCB_.find(pid);
    Process curr_process = itr->second;
    // cascading termination
    for (int i = 0; i < curr_process.GetChildren().size(); i++) {
        std::cout << curr_process.GetChildren()[i] << std::endl;
        Terminate(curr_process.GetChildren()[i]);
    }
    
    memory_.DeallocateMemory(pid);
    itr->second.KillProcess();

    // remove from all queues
    // remove from ready queue
    RemoveFromReady(pid);
    // remove from waiting queue
    for (int i = 0; i < waiting_.size(); i++) {
        if (waiting_[i].first == pid) {
            waiting_.erase(waiting_.begin() + i);
        }
    }
    // remove from all disks
    for (int i = 0; i < disks_.size(); i++) {
        for (int j = 0; j < disks_[i].size(); j++) {
            if (disks_[i][j].first == pid) {
                disks_[i].erase(disks_[i].begin() + j);
            }
        }
    }
}