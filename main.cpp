#include <iostream>
#include <vector>
#include <algorithm>
#include <ctime>
#include <queue>
#include <map>
using namespace std;

#define PB(x) push_back(x)
#define _SIZE_ 3

#include "Fifteen.h"

ostream &operator<<(ostream &os, const Fifteen &f)
{
    int fifteenSize = _SIZE_ * _SIZE_;

    for (int i = 0; i < fifteenSize; i++) {
        os << f[i] << "\t";

        if (i % _SIZE_ == (_SIZE_ - 1)) {
            os << "\n";
        }
    }

    return os;
}

bool bfs(Fifteen fifteen, map<__uint64_t, Fifteen> &visited);
bool dfsR(Fifteen fifteen, map<__uint64_t, Fifteen> &visited);
bool dfsI(Fifteen fifteen, map<__uint64_t, Fifteen> &visited);

int main()
{
    srand(time(NULL));

    Fifteen fifteen;
    Fifteen f = fifteen;

    vector<Fifteen> stans;
    for (int i = 0; i < 5; i++) {
        stans = f.getPossibleStates();

        fifteen = f;
        f = stans[rand() % stans.size()];
    }

    map<__uint64_t, Fifteen> visited;
    visited[f.stan] = fifteen;

//    if (!bfs(f, visited)) {
//    if (!dfsR(f, visited)) {
    if (!dfsI(f, visited)) {
        cout << "Fail\n";
        return 0;
    }

    cout << Fifteen() << "\n";

    fifteen = visited[Fifteen().stan];

    while (fifteen != f) {
        cout << fifteen << "\n";

        fifteen = visited[fifteen.stan];
    }

    cout << f << "\n";

    return 0;
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

                if (*it == Fifteen()) {
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
    if (fifteen == Fifteen()) {
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

                if (*it == Fifteen()) {
                    return true;
                }

                q.PB(*it);
            }
        }
    }

    return false;
}
