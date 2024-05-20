#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <stdexcept>

std::vector<std::vector<int>> readFrameFromFile(const std::string& filename) {
    std::vector<std::vector<int>> frame;
    std::ifstream file(filename);

    if (!file.is_open()) {
        throw std::runtime_error("Unable to open file");
    }

    std::string line;
    while (std::getline(file, line)) {
        if (line.empty()) {
            continue;
        }

        std::istringstream iss(line);
        std::vector<int> row;
        int value;
        while (iss >> value) {
            row.push_back(value);
        }
        frame.push_back(row);
    }

    return frame;
}

