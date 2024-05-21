#include <SFML/Graphics.hpp>
#include <array>
#include <cmath>
#include <limits>
#include <queue>
#include <set>
#include <stack>
#include <vector>

using namespace std;

constexpr int ROW = 9;
constexpr int COL = 9;
constexpr int CELL_SIZE = 40; // Size of each cell in the grid

using Pair = pair<int, int>;
using pPair = pair<double, Pair>;

struct Cell {
    int parent_i, parent_j;
    double f, g, h;
};

bool isValid(int row, int col) {
    return (row >= 0) && (row < ROW) && (col >= 0) && (col < COL);
}

bool isUnBlocked(const array<array<int, COL>, ROW>& grid, int row, int col) {
    return (grid[row][col] == 1);
}

bool isDestination(int row, int col, Pair dest) {
    return (row == dest.first && col == dest.second);
}

double calculateHValue(int row, int col, Pair dest) {
    return abs(row - dest.first) + abs(col - dest.second);
}

void tracePath(const array<array<Cell, COL>, ROW>& cellDetails, Pair dest, vector<Pair>& path) {
    int row = dest.first;
    int col = dest.second;
    stack<Pair> Path;

    while (!(cellDetails[row][col].parent_i == row && cellDetails[row][col].parent_j == col)) {
        Path.emplace(row, col);
        int temp_row = cellDetails[row][col].parent_i;
        int temp_col = cellDetails[row][col].parent_j;
        row = temp_row;
        col = temp_col;
    }

    Path.emplace(row, col);
    while (!Path.empty()) {
        path.push_back(Path.top());
        Path.pop();
    }
}

bool aStarStep(const array<array<int, COL>, ROW>& grid, Pair src, Pair dest, vector<Pair>& path, 
               array<array<bool, COL>, ROW>& closedList, array<array<Cell, COL>, ROW>& cellDetails, 
               set<pPair>& openList) {
    if (!openList.empty()) {
        auto p = *openList.begin();
        openList.erase(openList.begin());

        int i = p.second.first;
        int j = p.second.second;
        closedList[i][j] = true;

        constexpr array<int, 8> rowMov = {-1, 1, 0, 0, -1, -1, 1, 1};
        constexpr array<int, 8> colMov = {0, 0, -1, 1, -1, 1, -1, 1};

        for (int k = 0; k < 8; ++k) {
            int row = i + rowMov[k];
            int col = j + colMov[k];

            if (isValid(row, col)) {
                if (isDestination(row, col, dest)) {
                    cellDetails[row][col].parent_i = i;
                    cellDetails[row][col].parent_j = j;
                    tracePath(cellDetails, dest, path);
                    return true;
                } else if (!closedList[row][col] && isUnBlocked(grid, row, col)) {
                    double gNew = cellDetails[i][j].g + ((k < 4) ? 1.0 : 1.414);
                    double hNew = calculateHValue(row, col, dest);
                    double fNew = gNew + hNew;

                    if (cellDetails[row][col].f == numeric_limits<double>::max() || cellDetails[row][col].f > fNew) {
                        openList.emplace(fNew, make_pair(row, col));
                        cellDetails[row][col].f = fNew;
                        cellDetails[row][col].g = gNew;
                        cellDetails[row][col].h = hNew;
                        cellDetails[row][col].parent_i = i;
                        cellDetails[row][col].parent_j = j;
                    }
                }
            }
        }
    }

    return false;
}

int main() {
    array<array<int, COL>, ROW> grid = {{
        {1, 1, 1, 1, 1, 1, 1, 1, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 1, 1, 1, 1, 1, 1, 1, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 1, 1, 1, 1, 1, 1, 1, 1},
        {0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 1, 1, 1, 1, 1, 1, 1, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 1, 1, 1, 1, 1, 1, 1, 1}
    }};

    Pair src = make_pair(8, 8);
    Pair dest = make_pair(0, 0);

    sf::RenderWindow window(sf::VideoMode(COL * CELL_SIZE, ROW * CELL_SIZE), "A* Pathfinding Visualization");

    sf::RectangleShape cellShape(sf::Vector2f(CELL_SIZE - 1, CELL_SIZE - 1));
    cellShape.setOutlineThickness(1);
    cellShape.setOutlineColor(sf::Color::Black);

    vector<Pair> path;

    array<array<bool, COL>, ROW> closedList{};
    for (auto& row : closedList) {
        row.fill(false);
    }

    array<array<Cell, COL>, ROW> cellDetails{};
    for (auto& row : cellDetails) {
        for (auto& cell : row) {
            cell.f = numeric_limits<double>::max();
            cell.g = numeric_limits<double>::max();
            cell.h = numeric_limits<double>::max();
            cell.parent_i = -1;
            cell.parent_j = -1;
        }
    }

    int i = src.first, j = src.second;
    cellDetails[i][j].f = 0.0;
    cellDetails[i][j].g = 0.0;
    cellDetails[i][j].h = 0.0;
    cellDetails[i][j].parent_i = i;
    cellDetails[i][j].parent_j = j;

    set<pPair> openList;
    openList.emplace(0.0, make_pair(i, j));

    bool foundDest = false;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        if (!foundDest) {
            foundDest = aStarStep(grid, src, dest, path, closedList, cellDetails, openList);
        }

        window.clear();
        for (int row = 0; row < ROW; ++row) {
            for (int col = 0; col < COL; ++col) {
                cellShape.setPosition(col * CELL_SIZE, row * CELL_SIZE);
                if (grid[row][col] == 0) {
                    cellShape.setFillColor(sf::Color::Black);
                } else if (closedList[row][col]) {
                    cellShape.setFillColor(sf::Color::Red);
                } else if (row == src.first && col == src.second) {
                    cellShape.setFillColor(sf::Color::Green);
                } else if (row == dest.first && col == dest.second) {
                    cellShape.setFillColor(sf::Color::Magenta);
                } else {
                    cellShape.setFillColor(sf::Color::White);
                }
                window.draw(cellShape);
            }
        }

        // Draw the path
        if (foundDest) {
            for (const auto& p : path) {
                cellShape.setPosition(p.second * CELL_SIZE, p.first * CELL_SIZE);
                cellShape.setFillColor(sf::Color::Blue);
                window.draw(cellShape);
            }
        }

        window.display();
        sf::sleep(sf::milliseconds(250));
    }

    return 0;
}
