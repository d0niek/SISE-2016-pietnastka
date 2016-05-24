#include <iostream>
#include <vector>
#include <algorithm>
#include <ctime>
#include <queue>
#include <map>
#include <cstring>
using namespace std;

#define PB(x) push_back(x)
#define _SIZE_ROWS_ 4
#define _SIZE_COLS_ 2

#include "Fifteen.h"

ostream &operator<<(ostream &os, const Fifteen &f)
{
    int fifteenSize = _SIZE_ROWS_ * _SIZE_COLS_;

    for (int i = 0; i < fifteenSize; i++) {
        os << f[i] << "\t";

        if (i % _SIZE_COLS_ == (_SIZE_COLS_ - 1)) {
            os << "\n";
        }
    }

    return os;
}

bool resolv(Fifteen fifteen, map<__uint64_t, Fifteen> &visited, char* option);
bool bfs(Fifteen fifteen, map<__uint64_t, Fifteen> &visited);
bool dfsR(Fifteen fifteen, map<__uint64_t, Fifteen> &visited);
bool dfsI(Fifteen fifteen, map<__uint64_t, Fifteen> &visited);

int main(int argc, char* argv[])
{
    if (argc < 3) {
        cout << "Brakuje parametrów\n";
        return 0;
    }

    srand(time(NULL));

    Fifteen fifteen(_SIZE_ROWS_, _SIZE_COLS_, argv[2]);
    Fifteen f = fifteen;

    vector<Fifteen> stans;
    for (int i = 0; i < 5; i++) {
        stans = f.getPossibleStates();

        fifteen = f;
        f = stans[rand() % stans.size()];
    }

    map<__uint64_t, Fifteen> visited;
    visited[f.stan] = fifteen;

    if (!resolv(f, visited, argv[1])) {
        cout << "Fail\n";
        return 0;
    }

    cout << Fifteen(_SIZE_ROWS_, _SIZE_COLS_) << "\n";

    fifteen = visited[Fifteen(_SIZE_ROWS_, _SIZE_COLS_).stan];

    while (fifteen != f) {
        cout << fifteen << "\n";

        fifteen = visited[fifteen.stan];
    }

    cout << f << "\n";

    return 0;
}

bool resolv(Fifteen fifteen, map<__uint64_t, Fifteen> &visited, char* option)
{
    bool result = false;

    if (strcmp(option, "-b") == 0 || strcmp(option, "--bfs") == 0) {
        result = bfs(fifteen, visited);
    } else if (strcmp(option, "-d") == 0 || strcmp(option, "--dfs") == 0) {
        result = dfsR(fifteen, visited);
    } else if (strcmp(option, "-i") == 0 || strcmp(option, "--idfs") == 0) {
        result = dfsI(fifteen, visited);
    }

    return result;
}

bool bfs(Fifteen fifteen, map<__uint64_t, Fifteen> &visited)
{
    queue<Fifteen> q;

    q.push(fifteen);

    while (!q.empty()) {
        fifteen= q.front();
        q.pop();

        vector<Fifteen> stans = fifteen.getPossibleStates();

        for (vector<Fifteen>::iterator it = stans.begin(); it != stans.end(); it++) {
            if (visited.find(it->stan) == visited.end()) {
                visited[it->stan] = fifteen;

                if (*it == Fifteen(_SIZE_ROWS_, _SIZE_COLS_)) {
                    return true;
                }

                q.push(*it);
            }
        }
    }

    return false;
}

bool dfsR(Fifteen fifteen, map<__uint64_t, Fifteen> &visited)
{
    if (fifteen == Fifteen(_SIZE_ROWS_, _SIZE_COLS_)) {
        return true;
    }

    vector<Fifteen> stans = fifteen.getPossibleStates();

    for (vector<Fifteen>::iterator it = stans.begin(); it != stans.end(); it++) {
        if (visited.find(it->stan) == visited.end()) {
            visited[it->stan] = fifteen;

            if (dfsR(*it, visited)) {
                return true;
            }
        }
    }

    return false;
}

bool dfsI(Fifteen fifteen, map<__uint64_t, Fifteen> &visited)
{
    vector<Fifteen> q;

    q.PB(fifteen);

    while (!q.empty()) {
        fifteen= q.back();
        q.pop_back();

        vector<Fifteen> stans = fifteen.getPossibleStates();

        for (vector<Fifteen>::iterator it = stans.begin(); it != stans.end(); it++) {
            if (visited.find(it->stan) == visited.end()) {
                visited[it->stan] = fifteen;

                if (*it == Fifteen(_SIZE_ROWS_, _SIZE_COLS_)) {
                    return true;
                }

                q.PB(*it);
            }
        }
    }

    return false;
}
