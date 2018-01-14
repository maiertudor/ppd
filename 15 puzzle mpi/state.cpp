class State {

public:
    int heapIdx;
    
    int getEstimatedCost() {
        return estimatedCost;
    }

    void setLengthFromRoot(int g) {
        lengthFromRoot = g;
        setEstimatedCost();
    }

    int getLengthFromRoot() {
        return lengthFromRoot;
    }

    int getHeuristic() {
        return heuristicCost;
    }

    void setPrev(State* s) {
        prev = s;
    }

    State* getPrev() {
        return prev;
    }

    void addOpen() {
        inOpen = true;
    }

    void removeOpen() {
        inOpen = false;
    }

    bool checkOpen() {
        return inOpen;
    }

    size_t getHash() {
        return hashval;
    }
    
    bool operator<(State& other) {
        if (getEstimatedCost() < other.getEstimatedCost()) return true;
        if ((getEstimatedCost() == other.getEstimatedCost()) && (getLengthFromRoot() > other.getLengthFromRoot())) return true;
        return false;
    }

    virtual std::vector<State*> getNeighbors() = 0;

    virtual bool operator==(const State& other) = 0;

    virtual std::string toString() = 0;

    virtual ~State() {}

protected:
    int estimatedCost;
    int lengthFromRoot;
    int heuristicCost;
    State* prev;
    bool inOpen;
    size_t hashval;

    void initNew(int gval) {
        lengthFromRoot = gval;
        heuristicCost = computeHeuristic();
        setEstimatedCost();
        prev = NULL;
        inOpen = false;
        hashval = computeHash();
    }

    void setEstimatedCost() {
        estimatedCost = lengthFromRoot + heuristicCost;
    }

    virtual int computeHeuristic() = 0;

    virtual size_t computeHash() = 0;

};

struct stateHash {
public:
	size_t operator()(State* s) const {
		return s->getHash();
	}
};

struct stateEqual {
public:
	bool operator()(State* s1, State* s2) const {
		return *s1 == *s2;
	}
};
