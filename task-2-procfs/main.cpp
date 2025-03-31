#include <iostream>
#include <fstream>
#include <chrono>
#include <thread>
#include <string>
#include <unistd.h>

int main() {
    std::ofstream file("test.txt");
    
    if (!file.is_open()) {
        std::cerr << "Failed to open test.txt" << std::endl;
        return 1;
    }

    pid_t pid = getpid();
    
    std::cout << "File opened successfully. Process ID: " << pid << std::endl;

    // writing lines to the file
    for (int i = 1; i <= 1000; ++i) {
        file << "Line " << i << ": written at " << time(nullptr) << std::endl;
        file.flush();
        
        std::cout << "\rWriting line " << i << "/1000" << std::flush;
        
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    
    std::cout << "\nDone writing to file." << std::endl;
    file.close();
    
    return 0;
}