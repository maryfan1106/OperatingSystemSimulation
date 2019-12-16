#include "Memory-Control.hpp"

int main() {
    MemoryMangement t(500);


    t.PrintMem();
    t.AddProcess(1,100);
    t.PrintMem();
    t.AddProcess(2,100);
    t.PrintMem();
    t.AddProcess(3,100);
    t.RemoveProcess(2);
    t.PrintMem();
    t.AddProcess(7,25);
    t.PrintMem();
    // t.AddProcess(3,100);
    // t.PrintMem();
    // t.AddProcess(4,100);
    // t.PrintMem();
    // t.AddProcess(5,100);
    // t.PrintMem();
    // t.RemoveProcess(2);
    // t.PrintMem();
    // t.RemoveProcess(4);
    // t.PrintMem();
    // t.RemoveProcess(3);
    // t.PrintMem();
}