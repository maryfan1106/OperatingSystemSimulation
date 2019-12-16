// Mary Fan
// CSCI 340 Final Project

#include "OperatingSystem.hpp"

#include <iostream>
using namespace std;

int main() {
    string ram_string;
    string disks_string;
    try {
        cout << "Enter RAM memory size: ";
        getline(cin, ram_string);
        cout << "Enter number of hard disks: ";
        getline(cin, disks_string);
        long long ram = stoll(ram_string);
        int disks = stoi(disks_string);
        OS operatingSystem{ram, disks};
        operatingSystem.Start();
    } catch(const std::exception &e) {
        std::cout << e.what() << std::endl;
    }

    return 0;
}