#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <unistd.h>

int main() {
    const int NUM_FILES = 10;
    std::vector<std::unique_ptr<std::ofstream>> files;
    
    pid_t pid = getpid();

    std::cout << "process id: " << pid << std::endl;
    
    // open files in a loop
    for (int i = 0; i < NUM_FILES; ++i) {
        std::string filename = "file_" + std::to_string(i) + ".txt";
        
        auto file_ptr = std::make_unique<std::ofstream>(filename);
        
        if (!file_ptr->is_open()) {
            std::cerr << "Failed to open " << filename << std::endl;
            continue;
        }
        
        *file_ptr << "This is file " << i << " created by process " << pid << std::endl;
        
        files.push_back(std::move(file_ptr));

        std::cout << "Opened file: " << filename << std::endl;
    }
    
    std::cout << "\nOpened " << files.size() << " files." << std::endl;
    std::cin.get();

    return 0;
}