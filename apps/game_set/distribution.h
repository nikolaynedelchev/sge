#pragma once
#include <cinttypes>
#include <vector>
#include <cstddef>
namespace ndn::game_set
{

class Distribution
{
public:

    void Init(std::vector<int> elementsCounts); //{el_0_count, el_1_count, el_2_count, el_3_count, ...}
    void TakeByElementIndex(size_t elementIdx, int count = 1);
    void AddByElementIndex(size_t elementIdx, int count = 1);

    int ElementsCount() const;
    size_t TakeByElementNumber(int elementNumber, int count);

    std::vector<int> Dump() const;

private:

    std::vector<int> m_tree;
    size_t m_originalSize = 0;
    size_t m_initialBitmask = 0;
};

}
