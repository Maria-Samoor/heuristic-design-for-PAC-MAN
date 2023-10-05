#include <iostream>
#include <ctime>
#include <random>
#include <algorithm>

using namespace std;

struct GameState {
    pair<int, int> pacman;
    int grid[7][7]; 
    pair<int, int> power_bills[2];
    pair<int, int> ghost;
    pair<int, int>* bills;
    int numBills;
};

int abs_diff(int a, int b) {
    return (a > b) ? (a - b) : (b - a);
}

pair<int, int> generateRandomPosition() {
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<int> dis(0, 6);
    return make_pair(dis(gen), dis(gen));
}

bool isTooClose(const pair<int, int>& pos1, const pair<int, int>& pos2) {
    return (abs_diff(pos1.first, pos2.first) < 2) && (abs_diff(pos1.second, pos2.second) < 2);
}

void initializeGameState(GameState& state) {
    state.pacman = generateRandomPosition();
    
    do {
        state.ghost = generateRandomPosition();
    } while (state.ghost == state.pacman);

    do {
        state.power_bills[0] = generateRandomPosition();
    } while (state.power_bills[0] == state.pacman || state.power_bills[0] == state.ghost);

    do {
        state.power_bills[1] = generateRandomPosition();
    } while (state.power_bills[1] == state.pacman || state.power_bills[1] == state.ghost || state.power_bills[1] == state.power_bills[0]);

    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<int> dis(0, 10);
    state.numBills = dis(gen);

    state.bills = new pair<int, int>[state.numBills];

    for (int k = 0; k < state.numBills; ++k) {
        bool tooClose = false;
        do {
            state.bills[k] = generateRandomPosition();

            for (int i = 0; i < k; ++i) {
                if (isTooClose(state.bills[k], state.bills[i])) {
                    tooClose = true;
                    break;
                }
            }

            if (tooClose) {
                break;
            }
            
            for (int i = 0; i < 2; ++i) {
                if (isTooClose(state.bills[k], state.power_bills[i])) {
                    tooClose = true;
                    break;
                }
            }
        } while (state.bills[k] == state.pacman || state.bills[k] == state.ghost || state.bills[k] == state.power_bills[0] || state.bills[k] == state.power_bills[1] || tooClose);
    }

}

int calculateManhattanDistance(const pair<int, int>& pos1, const pair<int, int>& pos2) {
    return abs_diff(pos1.first, pos2.first) + abs_diff(pos1.second, pos2.second);
}

int calculateHeuristic(const GameState& state) {
    int minDistanceToBills = 10000;
    int minDistanceToPowerBills = 10000;
    int distanceToGhost = calculateManhattanDistance(state.pacman, state.ghost);
    if ( 2< distanceToGhost && distanceToGhost <= 3) {
        return 30;
    }
    else if (distanceToGhost <= 2) {
        return 40;
    }
    for (int k = 0; k < state.numBills; ++k) {
        int distance = calculateManhattanDistance(state.pacman, state.bills[k]);
        minDistanceToBills = min(minDistanceToBills, distance);
    }

    for (int i = 0; i < 2; ++i) {
        int distance = calculateManhattanDistance(state.pacman, state.power_bills[i]);
        if (distance <= 3) {
            return distance;
        }
        minDistanceToPowerBills =min(minDistanceToPowerBills, distance);
    }

    int heuristicValue =min(min(minDistanceToBills, minDistanceToPowerBills), distanceToGhost);

    return heuristicValue;
}

int main() {
    srand(time(NULL));
    GameState game;
    initializeGameState(game);
    cout << "Heuristic Value: " << calculateHeuristic(game) << endl;
    cout << "\nDistance to Ghost: " << calculateManhattanDistance(game.pacman, game.ghost) << endl;

    for (int i = 0; i < 2; ++i) {
        cout << "Distance to Power Bill " << i + 1 << ": " << calculateManhattanDistance(game.pacman, game.power_bills[i]) << endl;
    }

    for (int i = 0; i < game.numBills; ++i) {
        cout << "Distance to Bill " << i + 1 << ": " << calculateManhattanDistance(game.pacman, game.bills[i]) << endl;
    }

    return 0;
}
