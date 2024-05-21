#include <SFML/Graphics.hpp>
#include <bits/stdc++.h>
using namespace std;

#define ROW 9
#define COL 9
#define CELL_SIZE 40 // Size of each cell in the grid

typedef pair<int, int> Pair;
typedef pair<double, pair<int, int>> pPair;

struct cell {
    int parent_i, parent_j;
    double f, g, h;
};

bool isValid(int row, int col) {
    return (row >= 0) && (row < ROW) && (col >= 0) && (col < COL);
}

bool isUnBlocked(int grid[][COL], int row, int col) {
    return (grid[row][col] == 1);
}

bool isDestination(int row, int col, Pair dest) {
    return (row == dest.first && col == dest.second);
}

double calculateHValue(int row, int col, Pair dest) {
    // Manhattan distance
    return abs(row - dest.first) + abs(col - dest.second);
}

void tracePath(cell cellDetails[][COL], Pair dest, sf::RenderWindow &window, sf::RectangleShape &cellShape) {
    printf("\nThe Path is ");
    int row = dest.first;
    int col = dest.second;

    stack<Pair> Path;

    while (!(cellDetails[row][col].parent_i == row && cellDetails[row][col].parent_j == col)) {
        Path.push(make_pair(row, col));
        int temp_row = cellDetails[row][col].parent_i;
        int temp_col = cellDetails[row][col].parent_j;
        row = temp_row;
        col = temp_col;
    }

    Path.push(make_pair(row, col));
    while (!Path.empty()) {
        pair<int, int> p = Path.top();
        Path.pop();
        printf("-> (%d,%d) ", p.first, p.second);

        cellShape.setPosition(p.second * CELL_SIZE, p.first * CELL_SIZE);
        cellShape.setFillColor(sf::Color::Blue);
        window.draw(cellShape);
        window.display();
        sf::sleep(sf::milliseconds(250));
    }
}

void aStarSearch(int grid[][COL], Pair src, Pair dest, sf::RenderWindow &window, sf::RectangleShape &cellShape) {
    if (isValid(src.first, src.second) == false) {
        printf("Source is invalid\n");
        return;
    }

    if (isValid(dest.first, dest.second) == false) {
        printf("Destination is invalid\n");
        return;
    }

    if (isUnBlocked(grid, src.first, src.second) == false || isUnBlocked(grid, dest.first, dest.second) == false) {
        printf("Source or the destination is blocked\n");
        return;
    }

    if (isDestination(src.first, src.second, dest) == true) {
        printf("We are already at the destination\n");
        return;
    }

    bool closedList[ROW][COL];
    memset(closedList, false, sizeof(closedList));

    cell cellDetails[ROW][COL];

    int i, j;

    for (i = 0; i < ROW; i++) {
        for (j = 0; j < COL; j++) {
            cellDetails[i][j].f = FLT_MAX;
            cellDetails[i][j].g = FLT_MAX;
            cellDetails[i][j].h = FLT_MAX;
            cellDetails[i][j].parent_i = -1;
            cellDetails[i][j].parent_j = -1;
        }
    }

    i = src.first, j = src.second;
    cellDetails[i][j].f = 0.0;
    cellDetails[i][j].g = 0.0;
    cellDetails[i][j].h = 0.0;
    cellDetails[i][j].parent_i = i;
    cellDetails[i][j].parent_j = j;

    set<pPair> openList;
    openList.insert(make_pair(0.0, make_pair(i, j)));

    bool foundDest = false;

    while (!openList.empty()) {
        pPair p = *openList.begin();
        openList.erase(openList.begin());

        i = p.second.first;
        j = p.second.second;
        closedList[i][j] = true;

        window.clear();
        for (int row = 0; row < ROW; row++) {
            for (int col = 0; col < COL; col++) {
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
        window.display();

        sf::sleep(sf::milliseconds(250));

        double gNew, hNew, fNew;

        int rowMov[] = {-1, 1, 0, 0, -1, -1, 1, 1};
        int colMov[] = {0, 0, -1, 1, -1, 1, -1, 1};

        for (int k = 0; k < 8; k++) {
            int row = i + rowMov[k];
            int col = j + colMov[k];

            if (isValid(row, col)) {
                if (isDestination(row, col, dest)) {
                    cellDetails[row][col].parent_i = i;
                    cellDetails[row][col].parent_j = j;
                    printf("The destination cell is found\n");
                    tracePath(cellDetails, dest, window, cellShape);
                    foundDest = true;
                    return;
                } else if (!closedList[row][col] && isUnBlocked(grid, row, col)) {
                    gNew = cellDetails[i][j].g + ((k < 4) ? 1.0 : 1.414);
                    hNew = calculateHValue(row, col, dest);
                    fNew = gNew + hNew;

                    if (cellDetails[row][col].f == FLT_MAX || cellDetails[row][col].f > fNew) {
                        openList.insert(make_pair(fNew, make_pair(row, col)));
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

    if (!foundDest) {
        printf("Failed to find the Destination Cell\n");
    }
}

int main() {
    int grid[ROW][COL] = {
        {1, 1, 1, 1, 1, 1, 1, 1, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 1, 1, 1, 1, 1, 1, 1, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 1, 1, 1, 1, 1, 1, 1, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 1, 1, 1, 1, 1, 1, 1, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 1, 1, 1, 1, 1, 1, 1, 1}
    };

    Pair src = make_pair(8, 8);
    Pair dest = make_pair(0, 0);

    sf::RenderWindow window(sf::VideoMode(COL * CELL_SIZE, ROW * CELL_SIZE), "A* Pathfinding Visualization");

    sf::RectangleShape cellShape(sf::Vector2f(CELL_SIZE - 1, CELL_SIZE - 1));
    cellShape.setOutlineThickness(1);
    cellShape.setOutlineColor(sf::Color::Black);

    aStarSearch(grid, src, dest, window, cellShape);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }
    }

    return 0;
}
