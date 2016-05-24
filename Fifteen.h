//
// Created by d0niek on 23.05.16.
//

#ifndef PIENTNASTKA_FIFTEEN_H
#define PIENTNASTKA_FIFTEEN_H

#include <iostream>
#include <vector>
using namespace std;

struct Fifteen
{
    int r, c;
    __uint64_t stan;
    int emptyCell;
    char direction[4];

    Fifteen() : Fifteen(w, k)
    {
    }

    Fifteen(int r, int c) : Fifteen(r, c, "lurd")
    {
    }

    Fifteen(int r, int c, const char *direction)
    {
        this->r = r;
        this->c = c;
        this->stan = 0;
        this->emptyCell = r * c - 1;

        for (int i = 0; i < this->emptyCell; i++) {
            this->set(i, (__uint64_t)(i + 1));
        }

        for (int i = 0; i < 4; i++) {
            this->direction[i] = direction[i];
        }
    }

    Fifteen(const Fifteen &f)
    {
        this->r = f.r;
        this->c = f.c;
        this->stan = f.stan;
        this->emptyCell = f.emptyCell;

        for (int i = 0; i < 4; i++) {
            this->direction[i] = f.direction[i];
        }
    }

    void set(int index, __uint64_t value)
    {
        this->stan |= value << (4 * (15 - index));

        if (value == 0) {
            this->emptyCell = index;
        }
    }

    vector<Fifteen> getPossibleStates()
    {
        vector<Fifteen> stans;

        for (int i = 0; i < 4; i++) {
            Fifteen f = this->slide(this->direction[i]);

            if (f != *this) {
                stans.PB(f);
            }
        }

        return stans;
    }

    char different(Fifteen f)
    {
        char direction = '?';

        if (this->emptyCell - 1 == f.emptyCell) {
            direction = 'l';
        } else if (this->emptyCell - this->c == f.emptyCell) {
            direction = 'u';
        } else if (this->emptyCell + 1 == f.emptyCell) {
            direction = 'r';
        } else if (this->emptyCell + this->c == f.emptyCell) {
            direction = 'd';
        }

        return direction;
    }

    Fifteen slide(char direction)
    {
        Fifteen f = *this;

        switch (direction) {
            case 'l': {
                if (f.emptyCell % this->c != 0) {
                    f.swapCell(f.emptyCell, f.emptyCell - 1);
                    f.emptyCell -= 1;
                }
            } break;

            case 'u': {
                if (f.emptyCell / this->r != 0) {
                    f.swapCell(f.emptyCell, f.emptyCell - this->c);
                    f.emptyCell -= this->c;
                }
            } break;

            case 'r': {
                if (f.emptyCell % this->c != (this->c - 1)) {
                    f.swapCell(f.emptyCell, f.emptyCell + 1);
                    f.emptyCell += 1;
                }
            } break;

            case 'd': {
                if (f.emptyCell / this->r != (this->r - 1)) {
                    f.swapCell(f.emptyCell, f.emptyCell + this->c);
                    f.emptyCell += this->c;
                }
            } break;

            default:
                break;
        }

        return f;
    }

    void reset()
    {
        this->stan = 0;
        this->emptyCell = -1;
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
        this->r = f.r;
        this->c = f.c;
        this->stan = f.stan;
        this->emptyCell = f.emptyCell;

        for (int i = 0; i < 4; i++) {
            this->direction[i] = f.direction[i];
        }

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

    const __uint64_t operator[](int c) const
    {
        return (this->stan >> (4 * (15 - c))) & 15;
    }
};

#endif //PIENTNASTKA_FIFTEEN_H
