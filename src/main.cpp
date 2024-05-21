// Main.cpp
#include <iostream>
#include <string>
#include "FileReader.h"
#include "Graph.h"
#include "GUI.h"

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <filename>" << std::endl;
        return 1;
    }

    std::string filename = argv[1];

    std::vector<std::vector<int>> frame;
    try {
        frame = readFrameFromFile(filename);
    } catch (const std::runtime_error& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    std::vector<std::vector<std::vector<int>>> frames = processFrame(frame);

    displayFrames(frames);

    return 0;
}

