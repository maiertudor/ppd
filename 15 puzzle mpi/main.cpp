#include <iostream>
#include <vector>
#include <unordered_set>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <limits.h>
#include <getopt.h>
#include <unistd.h>

#include "state.cpp"
#include "board.cpp"

State* start;
State* goal;
std::vector<State*> path;
int numThreads = 0;
int bucketMultiplier = -1;

#include "priorityqueue.cpp"
#include "tspriorityqueue.cpp"
#include "parallel.cpp"

int main(int argc, char *argv[]) {
    int size = 4;
    int moves = -1;
    std::string inputFile = "";
    int opt;
    while ((opt = getopt(argc, argv, "t:f:")) != -1) {
        switch (opt) {
            case 't':
                numThreads = atoi(optarg);
                break;
            case 'f':
                inputFile = optarg;
                break;
        }
    }

    if (!inputFile.empty()) {
        start = (State*)(new Board(inputFile));
    }
    std::cout << "Start board:" << std::endl;
    std::cout << start->toString() << std::endl;

    std::cout << "Running parallel with " << numThreads << " threads..." << std::endl;
    
    goal = (State*)(new Board());

    time_t start_t = time(0);

    parallel(numThreads);

    time_t end_t = time(0);
    double time = difftime(end_t, start_t);


    std::cout << "Optimal solution found!" << std::endl << std::endl;
    int length = path.size();
    for (int i = 0; i < length; i++) {
        std::cout << "Step " << i << ":" << std::endl;
        std::cout << path[i]->toString() << std::endl;
    }
    std::cout << "Length of path: " << length-1 << std::endl;
    std::cout << "Total time: " << time << "s" << std::endl;

    return 0;
}
