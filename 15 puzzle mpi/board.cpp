class Board : public State {

public:
    int** board;
    int size;
    int emptyRow;
    int emptyCol;

    // make goal board
    Board() {
        size = 4;

        board = new int*[size];
        for (int i = 0; i < size; i++) {
            board[i] = new int[size];
            int row = 1+i*size;
            for (int j = 0; j < size; j++) {
                board[i][j] = row+j;
            }
        }
        board[size-1][size-1] = 0;
        emptyRow = size-1;
        emptyCol = size-1;

        initNew(0);
    }

    // initialize board based on file
    Board(std::string s) {
        FILE *fp = fopen(s.c_str(), "r");
        fscanf(fp, "%d", &size);

        board = new int*[size];
        for (int i = 0; i < size; i++) {
            board[i] = new int[size];
            int row = i*size;
            for (int j = 0; j < size; j++) {
                fscanf(fp, "%d", &board[i][j]);
                if (board[i][j] == 0) {
                    emptyRow = i;
                    emptyCol = j;
                }
            }
        }
        fclose(fp);

        initNew(0);
    }

    std::vector<State*> getNeighbors() {
        std::vector<State*> neighbors;
        if (emptyRow > 0) {
            Board* nbr = new Board(this, -1, 0);
            neighbors.push_back((State*)nbr);
        }
        if (emptyRow < size-1) {
            Board* nbr = new Board(this, 1, 0);
            neighbors.push_back((State*)nbr);
        }
        if (emptyCol > 0) {
            Board* nbr = new Board(this, 0, -1);
            neighbors.push_back((State*)nbr);
        }
        if (emptyCol < size-1) {
            Board* nbr = new Board(this, 0, 1);
            neighbors.push_back((State*)nbr);
        }
        return neighbors;
    }

    bool operator==(const State& other) {
        Board* b = (Board*)(&other);
        if (size != b->size) return false;
        for (int i = 0; i < size; i++) {
            for (int j = 0; j < size; j++) {
                if (board[i][j] != b->board[i][j]) {
                    return false;
                }
            }
        }
        return true;
    }

    std::string toString() {
        std::string s;
        
        // s += "Board of size " + std::to_string(size) + ":\n";
        // s += "f=" + std::to_string(estimatedCost) + ", ";
        // s += "g=" + std::to_string(lengthFromRoot) + ", ";
        // s += "h=" + std::to_string(heuristicCost) + "\n";
        // s += "inOpen=" + std::to_string(inOpen) + "\n";
        // s += "hashval=" + std::to_string(hashval) + "\n";
        
        s += "Blank tile (" + std::to_string(emptyRow) + ", " + std::to_string(emptyCol) + ")\n";
        for (int i = 0; i < size; i++) {
            for (int j = 0; j < size; j++) {
                s += std::to_string(board[i][j]);
                s += " ";
            }
            s += "\n";
        }
        return s;
    }

private:

    // copy board and make a move
    Board(Board* b, int dx, int dy) {
        size = b->size;
        emptyRow = b->emptyRow+dx;
        emptyCol = b->emptyCol+dy;

        board = new int*[size];
        for (int i = 0; i < size; i++) {
            if (i == emptyRow || i == b->emptyRow) {
                board[i] = new int[size];
                for (int j = 0; j < size; j++) {
                    board[i][j] = b->board[i][j];
                }
            } else {
                board[i] = b->board[i];
            }
        }

        board[emptyRow][emptyCol] = b->board[b->emptyRow][b->emptyCol];
        board[b->emptyRow][b->emptyCol] = b->board[emptyRow][emptyCol];

        initNew(INT_MAX);
    }

    int computeHeuristic() {
        int total = 0;
        for (int i = 0; i < size; i++) {
            for (int j = 0; j < size; j++) {
                int element = board[i][j];
                if (i == emptyRow && j == emptyCol) continue;
                int row = (element-1)/size;
                int col = (element-1)%size;
                int distance = abs(row-i) + abs(col-j);
                total += distance;
            }
        }
        return total;
    }

    size_t computeHash() {
        size_t h = 0;
        // int size2 = size * size;
        size_t mod = (1LL<<56)-5;
        for (int i = 0; i < size; i++) {
            for (int j = 0; j < size; j++) {
                h = (h * size + board[i][j]) % mod;
            }
        }

        size_t v = h;
        v = v ^ (v >> 21);
        v = v ^ (v << 37);
        v = v ^ (v >> 4);
        v = v * 4768777513237032717;
        v = v ^ (v << 20);
        v = v ^ (v >> 41);
        v = v ^ (v <<  5);
        return v;
    }

};
