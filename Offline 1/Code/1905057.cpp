/*
    step 1 :        Check if the Puzzle is Solvable by Inversion Count
    ---------         ---------------------------------------------------------------------

    Solvable Logic ::
    ----------------------
        1. Odd grid and Inversion count even
        2. Even grid :    a)  Inversion count even and Row index of blank space Odd.
                                 b) inversion count odd and Row index of blank space Even.

*/

    /// 1. If Not Solvable Return with a message.
    /// 2. If Solvable Then :  Step - 2

/*
    step 2 :        A Star Search Algorithm
    ----------       ---------------------------------

            1. Creating a NODE with the Initial PuzzleBoard
            2. Take a Priority queue for keeping the NODE
            3. Then Write a Custom Operator Function Named Operator() that will overload
                the built in comparator Operator()

                    logic :   A PuzzleBoard with smallest Priority(moves + heuristic) will be on TOP of the Priority Queue

            4.  PUSH the Initial Puzzle into The Priority Queue
            5. Then Loop through the Priority Queue until Its EMPTY
            6.  Get the TOP Element of the Priority Queue
            7. POP the TOP element from the Priority Queue
            8. Then insert this Node's PuzzleBoard into ClosedList
            9.  If the current PuzzleBoard is the FINAL PuzzleBoard then we break through the loop and we get our Answer
           10. Then we perform the  MOVES possible. // LEFT, RIGHT, UP, DOWN
           11. We swap the element during Performing Moves and CREATE a ChildBoard
           12. IF the ChildBoard is ALREADY in the closedList we don't PUSH the NODE into the Priority Queue
           13. IF not in the closedList we create a NODE and PUSH it in the Priority Queue
*/



#include<bits/stdc++.h>
using namespace std;

int gridSize,  blank_row_num, explored_node = 0, expanded_node = 0;
string heuristic;
vector<vector<int>> finalGrid;

// For the Possible 4 Moves (Left, Right, Up, Down)
int dirx[4] = {-1,1, 0, 0};     // Left, Right, Up, Down
int diry[4] = {0, 0,-1, 1};     // Up, Down, Left, Right


class Node {
private:
    vector<vector<int>> currentGrid;
    Node *parentNode;
    int moves, priority;

public:
    Node(vector<vector<int>> currentGrid, Node *parentNode, int moves, int priority) {
        this->currentGrid = currentGrid;
        this->parentNode = parentNode;
        this->moves = moves;
        this->priority = priority;
    }


    // setter and getter
    vector<vector<int>> &getCurrentGrid() {
        return this->currentGrid;
    }

    void setCurrentGrid(vector<vector<int>> currentGrid) {
        this->currentGrid = currentGrid;
    }

    void setParentNode(Node *parentNode) {
        this->parentNode = parentNode;
    }

    Node* getParentNode() {
        return this->parentNode;
    }

    void setMoves(int moves) {
        this->moves = moves;
    }

    int getMoves() {
        return this->moves;
    }

    void setPriority(int priority) {
        this->priority = priority;
    }

    int getPriority() {
        return this->priority;
    }

};


vector<Node*> closedList;

class Comparator {
public:
    bool operator()(Node* firstNode, Node* secondNode) {
        return firstNode->getPriority() > secondNode->getPriority();
    }
};


// Creating Initial Puzzle Grid from User Input
vector<vector<int>> createInitialGrid(vector<vector<int>> &puzzleGrid) {
    int gridData;
    for(int i = 0; i < gridSize; i++) {
        for(int j = 0; j < gridSize; j++) {
            cin >> gridData;
            if(gridData == 0) {
                blank_row_num = i + 1;
            }

            puzzleGrid[i][j] = gridData;
        }
    }

    return puzzleGrid;
}


// Creating Final Puzzle Grid
vector<vector<int>> createFinalGrid() {
    vector<vector<int>> puzzleGrid(gridSize, vector<int>(gridSize, 0));
    int gridData = 1;
    for(int i = 0; i < gridSize; i++) {
        for(int j = 0; j < gridSize; j++) {
            puzzleGrid[i][j] = gridData;
            gridData++;
        }
    }

    puzzleGrid[gridSize-1][gridSize-1] = 0;
    return puzzleGrid;
}


// Checks If the PuzzleGrid is Solvable or Not
bool checkSolvable(vector<vector<int>> &puzzleGrid) {
    int inversionCount = 0, counter = 0, prevData, NextData, length;

    vector<int> sourceGrid;
    length = gridSize * gridSize;
    sourceGrid.resize(length);

    // Converting 2D Array into 1D Array for easier computation
    for(int i = 0; i < gridSize; i++) {
        for(int j = 0; j < gridSize; j++) {
            sourceGrid[counter] = puzzleGrid[i][j];
            counter++;
        }
    }

    // Checking for inversion Count
    for (int i = 0; i < length; i++) {
        for (int j = i + 1; j < length; j++) {
            prevData = sourceGrid[i];
            NextData = sourceGrid[j];
            if (NextData != 0 && prevData > NextData) {
                inversionCount++;
            }
        }
    }

    cout << endl;
    cout << "Inversion count : " << inversionCount << endl;

    if(gridSize % 2 != 0 ) {
        if(inversionCount % 2 == 0)
            return true;
        else
            return false;
    }
    else {
        if((inversionCount % 2) == (blank_row_num % 2))
            return true;
        else
            return false;
    }
}


// Calculating Hamming Distance
int hammingDistance(vector<vector<int>> &puzzleGrid) {
    int hammingDistance = 0, solved_cellData = 1;
    for(int i = 0; i < gridSize; i++) {
        for(int j = 0; j < gridSize; j++) {
            if(puzzleGrid[i][j] != 0 && puzzleGrid[i][j] != solved_cellData) {
                hammingDistance++;
            }
            solved_cellData++;
        }
    }

    return hammingDistance;
}


// Calculating Manhattan Distance
int manhattanDistance(vector<vector<int>> &puzzleGrid) {
    int manhattanDistance = 0, rowMovement, colMovement;
    for(int i = 0; i < gridSize; i++) {
        for(int j = 0; j < gridSize; j++) {
            if(puzzleGrid[i][j] != 0) {
                rowMovement = abs(((puzzleGrid[i][j] - 1) / gridSize) - i);
                colMovement = abs(((puzzleGrid[i][j] - 1)  % gridSize) - j);
                manhattanDistance += rowMovement + colMovement;
            }
        }
    }

    return manhattanDistance;
}


// Calculating heuristic of the Puzzle Grid according to User input
int calculateHeuristic(vector<vector<int>> &puzzleGrid) {
    if(heuristic == "h")
        return hammingDistance(puzzleGrid);
    else
        return manhattanDistance(puzzleGrid);
}


// Printing the Puzzle Grid when We get the Solve
void printGrid(vector<vector<int>> &puzzleGrid) {
    for(int i = 0; i < gridSize; i++) {
        for(int j = 0; j < gridSize; j++) {
            cout << puzzleGrid[i][j] << " ";
        }
        cout << endl;
    }
    cout << endl;
}


// Recursively prints all the steps
void printSteps(Node *node, int moves) {
    if(node == nullptr)
        return;
    printSteps(node->getParentNode(), moves-1);
    if(moves == 0)
        cout << "Initial Board" << endl;
    else
        cout << "Move " << moves << ":" << endl;

    printGrid(node->getCurrentGrid());
}


// Performing A Star Search Algorithm
void performAStarSearch(vector<vector<int>> &puzzleGrid) {
    priority_queue<Node*, vector<Node*>, Comparator> pq;
    int priority = calculateHeuristic(puzzleGrid);
    Node* sourceNode = new Node(puzzleGrid, nullptr, 0, priority);
    pq.push(sourceNode);
    expanded_node++;

    while(!pq.empty()) {

        Node* topNode = pq.top();
        pq.pop();
        closedList.push_back(topNode);
        explored_node++;

        if(topNode->getCurrentGrid() == finalGrid) {
            cout << "No of Moves : " << topNode->getMoves() << endl << endl;
            cout << "Steps :" << endl << endl;
            printSteps(topNode, topNode->getMoves());
            cout << "No of Explored nodes : " << explored_node << endl;
            cout << "No of Expanded nodes : " << expanded_node << endl;
            break;
        }
        vector<vector<int>> grid = topNode->getCurrentGrid();

        bool flag;
        for(int i = 0; i < gridSize; i++) {
            for(int j = 0; j < gridSize; j++) {
                if(grid[i][j] != 0) continue;
                for(int k = 0; k < 4; k++) {
                    flag = false;
                    int row = i + dirx[k];
                    int col = j + diry[k];
                    if(row < 0 || row >= gridSize || col < 0 || col >= gridSize)
                        continue;
                    vector<vector<int>> childGrid = grid;
                    swap(childGrid[row][col], childGrid[i][j]);
                    for(int i = 0; i < closedList.size(); i++) {
                        if(closedList[i]->getCurrentGrid() == childGrid)
                            flag = true;
                    }

                    if(flag)
                        continue;
                    priority = calculateHeuristic(childGrid) + topNode->getMoves() + 1;
                    Node* childNode = new Node(childGrid, topNode, topNode->getMoves() + 1, priority);
                    pq.push(childNode);
                    expanded_node++;
                }
            }
        }
    }
}


// driver Code
int main() {

    cout << "Enter the grid size : ";
    cin >> gridSize;

    vector<vector<int>> initialGrid(gridSize, vector<int>(gridSize, 0));
    initialGrid = createInitialGrid(initialGrid);

    bool isSolvable = checkSolvable(initialGrid);
    if(!isSolvable) {
        cout << "Puzzle not Solvable " << endl;
        return 0;
    }
    else {
        cout << "Puzzle is Solvable" << endl << endl;
        cout << "Choose your heuristic :  (h / m)" << endl;
        cin >> heuristic;
        if(heuristic == "h") {
            cout << "Heuristic : Hamming Distance "  << endl;
            cout << "Hamming Distance : " << hammingDistance(initialGrid) << endl << endl;
        }
        else {
            cout << "Heuristic : Manhattan Distance " << endl;
            cout << "Manhattan Distance : " << manhattanDistance(initialGrid) << endl << endl;
        }

        finalGrid = createFinalGrid();
        performAStarSearch(initialGrid);
    }
}

/*
3
0 1 3
4 2 5
7 8 6

[ 4 moves ]
[ Hamming - explored = 5, expanded = 10  ,  9.455 s ]
[ Manhattan - explored = 5, expanded = 10  ,  9.677 s ]


3
1 5 0
7 6 4
2 3 8

[22 moves ]
[ Hamming - explored = 8456 , expanded = 13518  ,  22.754 s ]
[ Manhattan - explored = 1011, expanded = 1608,  5.513 s ]


4
1 2 3 4
5 6 7 0
8 9 10 11
12 13 14 15
[30 moves]


3
8 2 0
5 4 7
3 6 1

3
8 2 7
5 0 6
4 3 1

4
4 1 2 3
5 9 7 0
8 6 10 11
12 13 14 15
[40 steps]


4
14 8 12 1
9 6 4 3
7 2 5 11
10 0 13 15
[48 steps]


*/

