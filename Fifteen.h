//
// Created by d0niek on 23.05.16.
//

#include <iostream>
#include <vector>
using namespace std;

#ifndef PIENTNASTKA_FIFTEEN_H
#define PIENTNASTKA_FIFTEEN_H

struct Fifteen
{
    __uint64_t stan;
    int emptyCell;
    char direction[4];

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
        this->direction[0] = 'l';
        this->direction[1] = 'u';
        this->direction[2] = 'r';
        this->direction[3] = 'd';
    }

    Fifteen(char* direction)
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

        for (int i = 0; i < 4; i++) {
            this->direction[i] = direction[i];
        }
    }

    Fifteen(const Fifteen &f)
    {
        this->stan = f.stan;
        this->emptyCell = f.emptyCell;
        for (int i = 0; i < 4; i++) {
            this->direction[i] = f.direction[i];
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
