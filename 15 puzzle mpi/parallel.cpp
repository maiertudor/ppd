TSPriorityQueue<State*, stateHash, stateEqual> priorityQueue;
bool isFinished = false;

bool isShortestGoalState() {
    State* element;
    if ((element = priorityQueue.find(goal)) == NULL) {
        return false;
    }

    int estimatedCost = element->getEstimatedCost();

    for (int i = 0; i < numThreads; i++) {
        //compare the estimated cost of the goal state found with the lowest among all costs' of the other threads that are running
        if (priorityQueue.getMinKey(i) < estimatedCost) {
            return false;
        }
    }
    return true;
}

void* parallelThread(void* arg) {
    int thread_id = (int)(long long)(arg);

    while (1) {
        
        if (thread_id == 0) {
            if (isShortestGoalState()) {
                isFinished = true;
                return NULL;
            }
        }

        State* current = NULL;
        while (current == NULL) {
            if (isFinished) {
                return NULL;
            }
            current = priorityQueue.pop(thread_id);
        }
        
        current->removeOpen();
        std::vector<State*> neighbors = current->getNeighbors();
        
        for (int i = 0; i < neighbors.size(); i++) {
            State* neighbor = neighbors[i];
            priorityQueue.push(neighbor, current);
        }
    }

    return NULL;
}

void createPath() {
    std::vector<State*> tempPath;

    State* current = priorityQueue.find(goal);

    while (current != NULL && current != start) {
        tempPath.push_back(current);
        current = current->getPrev();
    }
            
    tempPath.push_back(start);
            
    int pathLength = tempPath.size();
    for (int i = 0; i < pathLength; i++) {
        path.push_back(tempPath[pathLength-1-i]);
    }

    return;
}

void parallel(int numThreads) {

    srand(time(NULL));

    priorityQueue.init(numThreads);

    pthread_t threads[numThreads];

    priorityQueue.push(start, NULL);
    for (int i = 0; i < numThreads; i++) {
        pthread_create(&threads[i], NULL, &parallelThread, (void*)(long long)i);
    }

    for (int i = 0; i < numThreads; i++) {
        pthread_join(threads[i], NULL);
    }

    createPath();
}
