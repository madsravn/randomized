#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <algorithm>

std::vector<std::string> loadfile(std::string filename) {
    std::ifstream myfile(filename);
    std::vector<std::string> lines;

    if(myfile.is_open()) {
        for( std::string line; std::getline(myfile, line);) {
            lines.push_back(line);
        }

    } else {
        std::cout << "File could not be opened." << std::endl;
    }
    return lines;
}

// Non-const-reference
bool equal(std::vector<std::string> v1, std::vector<std::string> v2) {
    if(v1.size() != v2.size()) {
        return false;
    }
    std::sort(v1.begin(), v1.end(), std::greater<std::string>());
    std::sort(v2.begin(), v2.end(), std::greater<std::string>());
    for(int i = 0; i < v1.size(); ++i) {
        if(v1.at(i) != v2.at(i)) {
            return false;
        }
    }
    return true;
}


int main() {


    auto v1 = loadfile("data1a");
    auto v2 = loadfile("data1b");
    std::cout << "Files are " << (equal(v1,v2) ? "" : "not ") << "equal." << std::endl;
    return 0;
}
