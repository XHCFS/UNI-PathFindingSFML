#include "Graph.h"
#include <queue>
#include <cmath>
#include <stdexcept>

struct Node {
    int x, y;
    float gCost, hCost, fCost;
    Node* parent;

    Node(int x, int y, float gCost = 0, float hCost = 0, Node* parent = nullptr)
        : x(x), y(y), gCost(gCost), hCost(hCost), fCost(gCost + hCost), parent(parent) {}
};

bool operator<(const Node& a, const Node& b) {
    return a.fCost > b.fCost; // Priority queue uses greater than for min-heap
}

std::vector<Node*> getNeighbors(Node* node, const std::vector<std::vector<int>>& map) {
    std::vector<Node*> neighbors;
    int directions[8][2] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}, {-1, -1}, {-1, 1}, {1, -1}, {1, 1}};
    for (const auto& direction : directions) {
        int newX = node->x + direction[0];
        int newY = node->y + direction[1];
        if (newX >= 0 && newX < map[0].size() && newY >= 0 && newY < map.size() && map[newY][newX] != 1) {
            float cost = (direction[0] == 0 || direction[1] == 0) ? 10 : 14; // Edge or diagonal cost
            neighbors.push_back(new Node(newX, newY, cost));
        }
    }
    return neighbors;
}

float heuristic(int x1, int y1, int x2, int y2) {
    return std::abs(x1 - x2) + std::abs(y1 - y2); // Manhattan distance
}

std::vector<std::vector<std::vector<int>>> processFrame(const std::vector<std::vector<int>>& frame) {
    std::vector<std::vector<std::vector<int>>> frames;
    std::vector<std::vector<int>> map = frame;

    int startX = -1, startY = -1, endX = -1, endY = -1;

    // Find start (2) and end (3) positions
    for (size_t y = 0; y < map.size(); ++y) {
        for (size_t x = 0; x < map[y].size(); ++x) {
            if (map[y][x] == 2) {
                startX = x;
                startY = y;
            } else if (map[y][x] == 3) {
                endX = x;
                endY = y;
            }
        }
    }

    if (startX == -1 || startY == -1 || endX == -1 || endY == -1) {
        throw std::runtime_error("Start or end position not found in the map.");
    }

    std::priority_queue<Node> openSet;
    std::vector<std::vector<bool>> closedSet(map.size(), std::vector<bool>(map[0].size(), false));
    openSet.emplace(startX, startY, 0, heuristic(startX, startY, endX, endY));

    while (!openSet.empty()) {
        Node current = openSet.top();
        openSet.pop();

        if (closedSet[current.y][current.x]) {
            continue;
        }
        closedSet[current.y][current.x] = true;

        // Record the current step
        std::vector<std::vector<int>> currentMap = map;
        for (size_t y = 0; y < closedSet.size(); ++y) {
            for (size_t x = 0; x < closedSet[y].size(); ++x) {
                if (closedSet[y][x]) {
                    currentMap[y][x] = 4; // Red for covered nodes
                }
            }
        }
        currentMap[startY][startX] = 2; // Blue for start
        currentMap[endY][endX] = 3; // Blue for end
        frames.push_back(currentMap);

        if (current.x == endX && current.y == endY) {
            Node* node = &current;
            while (node != nullptr) {
                map[node->y][node->x] = 3; // Final path in blue
                node = node->parent;
            }
            frames.push_back(map);
            return frames;
        }

        auto neighbors = getNeighbors(&current, map);
        for (auto& neighbor : neighbors) {
            if (closedSet[neighbor->y][neighbor->x]) {
                continue;
            }
            float tentativeGCost = current.gCost + neighbor->gCost; // Edge or diagonal cost
            if (tentativeGCost < neighbor->gCost || !neighbor->parent) {
                neighbor->gCost = tentativeGCost;
                neighbor->hCost = heuristic(neighbor->x, neighbor->y, endX, endY);
                neighbor->fCost = neighbor->gCost + neighbor->hCost;
                neighbor->parent = new Node(current);
                openSet.push(*neighbor);
                map[neighbor->y][neighbor->x] = 2; // Green for available nodes
            }
        }
    }

    throw std::runtime_error("Path not found.");
}

