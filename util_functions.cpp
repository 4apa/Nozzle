#include "nozzle_lib.h"
std::string GetCurrentDir ()       //get the address of current directory (for Windows)
{
    constexpr size_t maxDir = 300; //maximum address size
    char currentDir[maxDir];
    GetModuleFileName(NULL, currentDir, maxDir);
    std::string s = std::string(currentDir);
    for (int i = s.size()-1; i >=0; --i)
    {
        if (s[i] == '\\' || s[i] == '/')
            return s.substr(0, i);
    }
    return "\\";
}

void keep_window_open ()  //keeps window open until pressing 'q' button
{
    std::cin.clear();
    std::cout << "To EXIT  press 'q' -> Enter\n";
    for (char ch; std::cin >> ch; )
        if (ch == 'q') return;
}

void err(std::string s)   //error message
{
std::cerr << s;
keep_window_open();
}

