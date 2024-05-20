// Graph.cpp
#include "Graph.h"

std::vector<std::vector<std::vector<int>>> processFrame(const std::vector<std::vector<int>>& frame) {
    std::vector<std::vector<std::vector<int>>> frames;
    int numFrames = 10; // Number of frames to generate

    for (int i = 0; i < numFrames; ++i) {
        std::vector<std::vector<int>> newFrame = frame;

        // Simple processing: Shift each pixel value by i (mod 5)
        for (auto& row : newFrame) {
            for (auto& pixel : row) {
                pixel = (pixel + i) % 5;
            }
        }
        frames.push_back(newFrame);
    }

    return frames;
}

