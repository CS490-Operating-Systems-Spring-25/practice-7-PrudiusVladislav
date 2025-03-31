#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <chrono>
#include <thread>
#include <filesystem>
#include <sstream>
#include <unistd.h>

int read_file_max() {
    std::ifstream file("/proc/sys/fs/file-max");
    int max_files = 0;
    
    if (file.is_open()) {
        file >> max_files;
        file.close();
    }
    
    return max_files;
}

int main() {
    const int BATCH_SIZE = 1000;
    const int TARGET_FILES = 100000;
    
    std::vector<std::ofstream> files;
    files.reserve(TARGET_FILES);
    
    int max_files = read_file_max();
    pid_t pid = getpid();

    std::cout << "system file descriptor limit: " << max_files << std::endl;
    std::cout << "process ID: " << pid << std::endl;
    
    std::cout << "attempting to open " << TARGET_FILES << " files in batches of " << BATCH_SIZE << "..." << std::endl;
    
    int opened_files = 0;
    int current_batch = 0;
    
    std::filesystem::create_directory("limit_test");
    
    try {
        while (opened_files < TARGET_FILES) {
            std::cout << "Batch " << ++current_batch << ": Opening files " 
                      << opened_files << " to " << (opened_files + BATCH_SIZE - 1) << std::endl;
            
            int success_count = 0;
            
            for (int i = 0; i < BATCH_SIZE; ++i) {
                std::string filename = "limit_test/file_" + std::to_string(opened_files + i) + ".txt";
                
                files.emplace_back(filename);
                
                if (files.back().is_open()) {
                    files.back() << "Test file #" << (opened_files + i) << std::endl;
                    success_count++;
                } else {
                    std::cerr << "Failed to open file #" << (opened_files + i) << std::endl;
                    files.pop_back();
                }
            }
            
            opened_files += success_count;
            std::cout << "Successfully opened " << success_count << " files in this batch" << std::endl;
            std::cout << "Total open files: " << opened_files << std::endl;
            
            // if no files were opened in the batch -> we hit the limit
            if (success_count == 0) {
                std::cout << "Hit system limit at " << opened_files << " files" << std::endl;
                break;
            }
            
            // check file descriptor count
            std::ostringstream cmd;
            cmd << "ls -l /proc/" << getpid() << "/fd | wc -l";
            std::cout << "current file descriptior count: ";
            system(cmd.str().c_str());
            
            std::cout << "Sleeping for 1 second..." << std::endl;
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    } catch (const std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }
    
    std::cout << "\nReached " << opened_files << " open files." << std::endl;
    std::cout << "Press Enter to close all files and exit..." << std::endl;
    std::cin.get();
    
    return 0;
}