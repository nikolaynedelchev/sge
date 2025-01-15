#include <cinttypes>
#include <string>

#define TEST_HOME

#ifdef TEST_HOME
#include "sudoku.h"
#endif

namespace ndn
{
namespace sudoku
{
namespace alg
{

using uint = unsigned;

#define M_TO_Q(r, c) (((r)/3)*3 + (c)/3)

#define BIT(b) (1 << (b))
#define SET_BIT(n, b) n |= BIT(b)
#define CLR_BIT(n, b) n &= ~(BIT(b))

uint8_t matrix[9*9] = // best 13, worst 30, av 18
{
    0,0,0,  0,0,0,  0,0,0,
    0,0,0,  0,0,0,  0,0,0,
    0,0,0,  0,0,0,  0,0,0,
//
    0,0,0,  0,0,0,  0,0,0,
    0,0,0,  0,0,0,  0,0,0,
    0,0,0,  0,0,0,  0,0,0,
//
    0,0,0,  0,0,0,  0,0,0,
    0,0,0,  0,0,0,  0,0,0,
    0,0,0,  0,0,0,  0,0,0
};

struct
{
    uint16_t rows[9];
    uint16_t columns[9];
    uint16_t quadrants[9];
}ctx;

static inline uint BitsCount(uint16_t n) {
    n = n - ((n >> 1) & 0x5555);
    n = (n & 0x3333) + ((n >> 2) & 0x3333);
    n = (n + (n >> 4)) & 0x0F0F;
    n = n + (n >> 8);
    return n & 0x001F;
}

static inline void PutNumber(uint r, uint c, uint8_t n)
{
    auto q = M_TO_Q(r, c);
    SET_BIT(ctx.rows[r], n);
    SET_BIT(ctx.columns[c], n);
    SET_BIT(ctx.quadrants[q], n);
    matrix[r*9+c] = n;
}

static inline void RemoveNumber(uint r, uint c)
{
    auto q = M_TO_Q(r, c);
    auto idx = r*9+c;
    auto n = matrix[idx];
    CLR_BIT(ctx.rows[r], n);
    CLR_BIT(ctx.columns[c], n);
    CLR_BIT(ctx.quadrants[q], n);
    matrix[idx] = 0;
}

static void InitCtx()
{
    int idx = 0;
    for(uint r = 0; r < 9; r++)
    {
        for(uint c = 0; c < 9; c++)
        {
            int ch = 0;
            while (true)
            {
                ch = getchar();
                if (ch == '\n' || ch == '\a')
                    continue;
                break;
            }
            if (ch != '.')
            {
                matrix[idx] = ch - '0';
                PutNumber(r, c, matrix[idx]);
            }
            else
            {
                matrix[idx] = 0;
                RemoveNumber(r, c);
            }
            idx++;
        }
    }
}

static inline uint16_t TakenMask(uint r, uint c, uint8_t q)
{
    //auto q = M_TO_Q(r, c);
    uint16_t mask = ctx.rows[r] | ctx.columns[c] | ctx.quadrants[q];
    return mask;
}

static inline uint CountAvailable(uint r, uint c, uint8_t q)
{
    auto mask = TakenMask(r, c, q);
    return 9 - BitsCount(mask);
}

static uint8_t quadrantMap[] = {
    0, 0, 0, 1, 1, 1, 2, 2, 2,
    0, 0, 0, 1, 1, 1, 2, 2, 2,
    0, 0, 0, 1, 1, 1, 2, 2, 2,
    3, 3, 3, 4, 4, 4, 5, 5, 5,
    3, 3, 3, 4, 4, 4, 5, 5, 5,
    3, 3, 3, 4, 4, 4, 5, 5, 5,
    6, 6, 6, 7, 7, 7, 8, 8, 8,
    6, 6, 6, 7, 7, 7, 8, 8, 8,
    6, 6, 6, 7, 7, 7, 8, 8, 8
};

static bool Find()
{
    int bestC = -1;
    int bestR = -1;
    uint8_t* bestQ = nullptr;
    int bestScore = 10;
    uint8_t* qPtr = &(quadrantMap[0]);
    int idx = 0;
    for(uint r = 0; r < 9; r++)
    {
        for(uint c = 0; c < 9; c++)
        {
            auto digit = matrix[idx];
            if (digit == 0)
            {
                auto score = CountAvailable(r, c, *qPtr);
                if (score < bestScore)
                {
                    if (score == 0)
                    {
                        return false;
                    }
                    bestScore = score;
                    bestR = r;
                    bestC = c;
                    bestQ = qPtr;
                }
            }
            qPtr++;
            idx++;
        }
    }
    if (bestC < 0)
    {
        return true;
    }
    uint16_t usedNumbers = TakenMask(bestR, bestC, *bestQ);
    uint16_t numMask = BIT(0);
    for(uint n = 1; n < 10; n++)
    {
        numMask <<= 1;
        if ((usedNumbers & numMask) == 0)
        {
            PutNumber(bestR, bestC, n);
            if (Find())
            {
                return true;
            }
            RemoveNumber(bestR, bestC);
        }
    }
    return false;
}

void PrintMatrix()
{
    int idx = 0;
    for(uint r = 0; r < 9; r++)
    {
        for(uint c = 0; c < 9; c++)
        {
            putchar(matrix[idx] + '0');
            //printf("%d", matrix[r][c]);
            idx++;
        }
        putchar('\n');
    }
}

void Solve()
{
    InitCtx();

    //PrintMatrix();

    Find();

    PrintMatrix();
}

}
}
}

#ifdef TEST_HOME
#else

int main()
{
    ndn::sudoku::alg::Solve();
}

#endif
