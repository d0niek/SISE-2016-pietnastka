#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>
#include <map>
#include <cstring>

using namespace std;

#define PB(x) push_back(x)

int w, k;

#include "Fifteen.h"

ostream &operator<<(ostream &os, const Fifteen &f)
{
    int fifteenSize = w * k;

    for (int i = 0; i < fifteenSize; i++) {
        os << f[i] << "\t";

        if (i % k == (k - 1)) {
            os << "\n";
        }
    }

    return os;
}

bool solve(Fifteen fifteen, map<__uint64_t, Fifteen> &visited, char *option);
bool bfs(Fifteen fifteen, map<__uint64_t, Fifteen> &visited);
bool dfsR(Fifteen fifteen, map<__uint64_t, Fifteen> &visited);
bool dfsI(Fifteen fifteen, map<__uint64_t, Fifteen> &visited);

int main(int argc, char *argv[])
{
    if (argc < 3) {
        cout << "Brakuje parametrów\n";
        return 0;
    }

    cin >> w >> k;

    Fifteen fifteen(w, k, argv[2]);
    fifteen.reset();

    for (int i = 0; i < w * k; i++) {
        __uint64_t v;
        cin >> v;
        fifteen.set(i, v);
    }

    map<__uint64_t, Fifteen> visited;
    visited[fifteen.stan] = fifteen;

    if (solve(fifteen, visited, argv[1])) {
        Fifteen s = Fifteen(w, k);
        Fifteen f;
        vector<char> solution;

        do {
            f = visited[s.stan];

            solution.PB(f.different(s));

            s = f;
        } while (f != fifteen);

        reverse(solution.begin(), solution.end());

        cout << solution.size() << "\n";

        for (unsigned int i = 0; i < solution.size(); i++) {
            cout << solution[i];
        }

        cout << "\n" << visited.size();

        return 0;
    }

    cout << -1;
    return 0;
}

bool solve(Fifteen fifteen, map<__uint64_t, Fifteen> &visited, char *option)
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
        fifteen = q.front();
        q.pop();

        vector<Fifteen> stans = fifteen.getPossibleStates();

        for (vector<Fifteen>::iterator it = stans.begin(); it != stans.end(); it++) {
            if (visited.find(it->stan) == visited.end()) {
                visited[it->stan] = fifteen;

                if (*it == Fifteen(w, k)) {
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
    if (fifteen == Fifteen(w, k)) {
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
        fifteen = q.back();
        q.pop_back();

        vector<Fifteen> stans = fifteen.getPossibleStates();

        reverse(stans.begin(), stans.end());

        for (vector<Fifteen>::iterator it = stans.begin(); it != stans.end(); it++) {
            if (visited.find(it->stan) == visited.end()) {
                visited[it->stan] = fifteen;

                if (*it == Fifteen(w, k)) {
                    return true;
                }

                q.PB(*it);
            }
        }
    }

    return false;
}
