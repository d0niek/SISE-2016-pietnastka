#include <iostream>
#include <vector>
#include <algorithm>
#include <ctime>
#include <queue>
#include <map>
using namespace std;

#define PB(x) push_back(x)
#define _SIZE_ 4

struct Fifteen;

ostream &operator<<(ostream &os, const Fifteen &f);

struct Fifteen
{
    __uint64_t stan;
    int emptyCell;

    Fifteen()
    {
#if _SIZE_ == 4
        this->stan = 0x123456789abcdef0;
        this->emptyCell = 15;
#elif _SIZE_ == 3
        this->stan = 0x1234567800000000;
        this->emptyCell = 8;
#elif _SIZE_ == 2
        this->stan = 0x1230000000000000;
        this->emptyCell = 3;
#endif
    }

    Fifteen(const Fifteen &f)
    {
        this->stan = f.stan;
        this->emptyCell = f.emptyCell;
    }

    vector<Fifteen> getPossibleStates()
    {
        vector<Fifteen> stans;
        char direction[] = {'l', 'u', 'r', 'd'};

        for (int i = 0; i < 4; i++) {
            Fifteen f = this->slide(direction[i]);

            if (f != *this) {
                stans.PB(f);
            }
        }

        return stans;
    }

    Fifteen slide(char direction)
    {
        Fifteen f = *this;

        switch (direction) {
            case 'l': {
                if (f.emptyCell % _SIZE_ != 0) {
                    f.swapCell(f.emptyCell, f.emptyCell - 1);
                    f.emptyCell -= 1;
                }
            } break;

            case 'u': {
                if (f.emptyCell / _SIZE_ != 0) {
                    f.swapCell(f.emptyCell, f.emptyCell - _SIZE_);
                    f.emptyCell -= _SIZE_;
                }
            } break;

            case 'r': {
                if (f.emptyCell % _SIZE_ != (_SIZE_ - 1)) {
                    f.swapCell(f.emptyCell, f.emptyCell + 1);
                    f.emptyCell += 1;
                }
            } break;

            case 'd': {
                if (f.emptyCell / _SIZE_ != (_SIZE_ - 1)) {
                    f.swapCell(f.emptyCell, f.emptyCell + _SIZE_);
                    f.emptyCell += _SIZE_;
                }
            } break;

            default:
                break;
        }

        return f;
    }

    void swapCell(int x, int y)
    {
        // Gets cells and move them to the beginning
        __uint64_t a = (this->stan & ((__uint64_t) 15 << (4 * (15 - x)))) >> (4 * (15 - x));
        __uint64_t b = (this->stan & ((__uint64_t) 15 << (4 * (15 - y)))) >> (4 * (15 - y));

        // Puts zero to cells
        this->stan &= ~((__uint64_t) 15 << (4 * (15 - x)));
        this->stan &= ~((__uint64_t) 15 << (4 * (15 - y)));

        // Swap cells
        this->stan |= (a << (4 * (15 - y)));
        this->stan |= (b << (4 * (15 - x)));
    }

    Fifteen &operator=(const Fifteen &f)
    {
        this->stan = f.stan;
        this->emptyCell = f.emptyCell;

        return *this;
    }

    bool operator==(const Fifteen &f) const
    {
        return this->stan == f.stan;
    }

    bool operator!=(const Fifteen &f) const
    {
        return this->stan != f.stan;
    }

    bool operator<(const Fifteen &f) const
    {
        return this->stan < f.stan;
    }

    __uint64_t operator[](int c) const
    {
        return (this->stan >> (4 * (15 - c))) & 15;
    }
};

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
