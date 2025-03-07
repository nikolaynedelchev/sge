#include <cinttypes>
#include <fmt/format.h>
#include <string>
#include <tools/Stopwatch.h>

// https://dmoj.ca/problem/sudoku/rank/

namespace ndn::sudoku
{

using uint = unsigned;

#define M_TO_Q(r, c) (((r)/3)*3 + (c)/3)

#define BIT(b) (1 << (b))
#define SET_BIT(n, b) n |= BIT(b)
#define CLR_BIT(n, b) n &= ~(BIT(b))


uint8_t emptyMatrix[9][9] = // best 13, worst 30, av 18
{
    {0,0,0,  0,0,0,  0,0,0},
    {0,0,0,  0,0,0,  0,0,0},
    {0,0,0,  0,0,0,  0,0,0},
//
    {0,0,0,  0,0,0,  0,0,0},
    {0,0,0,  0,0,0,  0,0,0},
    {0,0,0,  0,0,0,  0,0,0},
//
    {0,0,0,  0,0,0,  0,0,0},
    {0,0,0,  0,0,0,  0,0,0},
    {0,0,0,  0,0,0,  0,0,0}
};

uint8_t matrix_3[9][9] = // best 27 worst 75 av 40
{
        {0,0,8,  3,0,0,  0,6,0},
        {4,7,0,  0,0,0,  0,0,0},
        {3,0,0,  0,0,7,  0,5,0},
        //
        {0,0,9,  8,0,0,  0,1,0},
        {0,0,0,  0,0,0,  0,0,0},
        {0,0,3,  0,2,0,  5,0,0},
        //
        {0,0,0,  7,0,0,  4,0,0},
        {0,9,4,  0,0,8,  2,0,0},
        {0,0,5,  1,0,0,  9,0,0}
};

uint8_t matrix_HARDEST[9][9] =  // best 1500, worst 2500, av 2000
    {
        {8,0,0,  0,0,0,  0,0,0},
        {0,0,3,  6,0,0,  0,0,0},
        {0,7,0,  0,9,0,  2,0,0},
        //
        {0,5,0,  0,0,7,  0,0,0},
        {0,0,0,  0,4,5,  7,0,0},
        {0,0,0,  1,0,0,  0,3,0},
        //
        {0,0,1,  0,0,0,  0,6,8},
        {0,0,8,  5,0,0,  0,1,0},
        {0,9,0,  0,0,0,  4,0,0}
};

uint8_t matrix_HARDEST_2[9][9] = // best 13, worst 30, av 18
    {
        {0,0,0,  0,0,0,  0,0,0},
        {0,0,0,  0,0,3,  0,8,5},
        {0,0,1,  0,2,0,  0,0,0},
        //
        {0,0,0,  5,0,7,  0,0,0},
        {0,0,4,  0,0,0,  1,0,0},
        {0,9,0,  0,0,0,  0,0,0},
        //
        {5,0,0,  0,0,0,  0,7,3},
        {0,0,2,  0,1,0,  0,0,0},
        {0,0,0,  0,4,0,  0,0,9}
};

uint8_t matrix_onlineJudge[9][9] = // best 13, worst 30, av 18
    {
        {0,7,0,1,8,0,4,0,0},
        {9,0,0,0,0,3,0,0,6},
        {0,0,1,9,0,0,0,0,0},
        {0,0,4,0,0,8,9,0,2},
        {1,0,0,0,6,0,0,0,3},
        {2,0,7,4,0,0,6,0,0},
        {0,0,0,0,0,1,2,0,0},
        {7,0,0,8,0,0,0,0,4},
        {0,0,9,0,5,2,0,3,0}
};

auto matrix = matrix_onlineJudge;

struct
{
    uint16_t rows[9];
    uint16_t columns[9];
    uint16_t quadrants[9];
}ctx;

void PrintMatrix(uint8_t sudokuMatrix[9][9])
{
    // ╔ ═ ╗ ║ ╚ ╝ ╬ ╦ ╩ ╣ ╠ ╤
    // ┌ ─ ┐ │ └ ┘ ┼ ┬ ┴ ┤ ├
    // ╤ ╧ ╪ ┼ ╒ ╕ ╞ ╡ ╓ ╖ ╙ ╜ ╟ ╢ ╫

    static std::string s_mat = R"(
    ╔═══╤═══╤═══╦═══╤═══╤═══╦═══╤═══╤═══╗
    ║ x │ x │ x ║ x │ x │ x ║ x │ x │ x ║
    ╟───┼───┼───╫───┼───┼───╫───┼───┼───╢
    ║ x │ x │ x ║ x │ x │ x ║ x │ x │ x ║
    ╟───┼───┼───╫───┼───┼───╫───┼───┼───╢
    ║ x │ x │ x ║ x │ x │ x ║ x │ x │ x ║
    ╠═══╪═══╪═══╬═══╪═══╪═══╬═══╪═══╪═══╣
    ║ x │ x │ x ║ x │ x │ x ║ x │ x │ x ║
    ╟───┼───┼───╫───┼───┼───╫───┼───┼───╢
    ║ x │ x │ x ║ x │ x │ x ║ x │ x │ x ║
    ╟───┼───┼───╫───┼───┼───╫───┼───┼───╢
    ║ x │ x │ x ║ x │ x │ x ║ x │ x │ x ║
    ╠═══╪═══╪═══╬═══╪═══╪═══╬═══╪═══╪═══╣
    ║ x │ x │ x ║ x │ x │ x ║ x │ x │ x ║
    ╟───┼───┼───╫───┼───┼───╫───┼───┼───╢
    ║ x │ x │ x ║ x │ x │ x ║ x │ x │ x ║
    ╟───┼───┼───╫───┼───┼───╫───┼───┼───╢
    ║ x │ x │ x ║ x │ x │ x ║ x │ x │ x ║
    ╚═══╧═══╧═══╩═══╧═══╧═══╩═══╧═══╧═══╝
    )";
    std::string s = s_mat;
    int c = 0, r = 0;
    for(auto& l : s)
    {
        if (l != 'x') continue;
        l = '0' + sudokuMatrix[r][c];
        if (l == '0') l = ' ';
        c++;
        if(c > 8)
        {
            r++;
            c = 0;
        }
    }

    fmt::println("{}", s);
}

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
    matrix[r][c] = n;
}

static inline void RemoveNumber(uint r, uint c)
{
    auto q = M_TO_Q(r, c);
    auto n = matrix[r][c];
    CLR_BIT(ctx.rows[r], n);
    CLR_BIT(ctx.columns[c], n);
    CLR_BIT(ctx.quadrants[q], n);
    matrix[r][c] = 0;
}

static void InitCtx()
{
    for(uint r = 0; r < 9; r++)
    {
        for(uint c = 0; c < 9; c++)
        {
            if (auto n = matrix[r][c])
            {
                PutNumber(r, c, n);
            }
            else
            {
                RemoveNumber(r, c);
            }
        }
    }
}

static inline uint16_t TakenMask(uint r, uint c)
{
    auto q = M_TO_Q(r, c);
    uint16_t mask = ctx.rows[r] | ctx.columns[c] | ctx.quadrants[q];
    return mask;
}

static inline uint CountAvailable(uint r, uint c)
{
    auto mask = TakenMask(r, c);
    return 9 - BitsCount(mask);
}

static int s_fail = 0;
static bool Find()
{
    int bestC = -1;
    int bestR = -1;
    int bestScore = 10;
    for(uint r = 0; r < 9; r++)
    {
        uint8_t* row = matrix[r];
        for(uint c = 0; c < 9; c++)
        {
            auto digit = row[c];
            if (digit == 0)
            {
                auto score = CountAvailable(r, c);
                if (score < bestScore)
                {
                    if (score == 0)
                    {
                        s_fail++;
                        return false;
                    }
                    bestScore = score;
                    bestR = r;
                    bestC = c;
                }
            }
        }
    }
    if (bestC < 0)
    {
        return true;
    }
    uint16_t usedNumbers = TakenMask(bestR, bestC);
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

static bool CheckRow(int r)
{
    uint8_t rowMap[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    for(int c = 0; c < 9; c++)
    {
        rowMap[ matrix[r][c] ] = 1;
    }
    for(int c = 1; c < 10; c++)
    {
        if (rowMap[c] == 0)
            return false;
    }
    return true;
}

static bool CheckColumn(int c)
{
    uint8_t colMap[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    for(int r = 0; r < 9; r++)
    {
        colMap[ matrix[r][c] ] = 1;
    }
    for(int r = 1; r < 10; r++)
    {
        if (colMap[r] == 0)
            return false;
    }
    return true;
}

static bool CheckQuadrant(int qr, int qc)
{
    uint8_t qMap[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    for(int r = qr * 3; r < qr*3 + 3; r++)
    {
        for(int c = qc * 3; c < qc*3 + 3; c++)
        {
            qMap[ matrix[r][c] ] = 1;
        }
    }
    for(int r = 1; r < 10; r++)
    {
        if (qMap[r] == 0)
            return false;
    }
    return true;
}

void Solve()
{
    s_fail = 0;
    PrintMatrix(matrix);

    tools::Stopwatch sw;

    InitCtx();
    Find();

    auto duration = sw.MeasureMicros();

    bool ok = true;
    for(int i = 0; i < 9; i++)
    {
        if (CheckColumn(i) == false ||
            CheckRow(i) == false)
        {
            ok = false;
            break;
        }
    }
    for(int qr = 0; qr < 3; qr++)
    {
        for (int qc = 0; qc < 3; qc++)
        {
            if (CheckQuadrant(qr, qc) == false)
            {
                ok = false;
                break;
            }
        }
    }

    if (!ok)
    {
        fmt::println("Sudoku ERROR !");
    }
    else
    {
        fmt::println("Sudoku solved: {} micro seconds, fails: {}", duration, s_fail);
    }
    PrintMatrix(matrix);
}


}
