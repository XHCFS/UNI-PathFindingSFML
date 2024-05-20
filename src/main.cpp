// Main.cpp
#include <iostream>
#include "FileReader.h"
#include "Graph.h"
#include "GUI.h"

int main() {
    // Read a single frame from file
    std::vector<std::vector<int>> frame;
    try {
        frame = readFrameFromFile("frame.txt");
    } catch (const std::runtime_error& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    // Process the frame to generate multiple frames
    std::vector<std::vector<std::vector<int>>> frames = processFrame(frame);

    // Display the frames
    displayFrames(frames);

    return 0;
}

