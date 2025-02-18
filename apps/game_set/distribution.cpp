#include "distribution.h"

namespace ndn::game_set
{

void Distribution::Init(std::vector<int> elementsCounts)
{
    m_originalSize = elementsCounts.size();
    size_t pow2 = 1;
    while(pow2 < elementsCounts.size()) pow2 *= 2;
    m_tree.resize(pow2 * 2 - 1, 0);
    for(size_t idx = 0; idx < elementsCounts.size(); idx++)
    {
        AddByElementIndex(idx, elementsCounts[idx]);
    }

    m_initialBitmask = pow2;
}

int Distribution::ElementsCount() const
{
    return m_tree[0];
}

void Distribution::TakeByElementNumber(int elementNumber, int count)
{
    return AddByElementNumber(elementNumber, -count);
}

void Distribution::AddByElementNumber(int elementNumber, int count)
{
    size_t step = m_initialBitmask;
    size_t idx = 0;
    int sum = 0;

    if (step >= 1)
    {
        m_tree[idx] += count;
    }
    while(step > 1)
    {
        if (m_tree[idx + 1] + sum >= elementNumber)
        {
            // left branch
            idx++;
        }
        else
        {
            // right branch
            sum += m_tree[idx + 1];
            idx += step;
        }
        m_tree[idx] += count;
        step /= 2;
    }
}

void Distribution::TakeByElementIndex(size_t elementIdx, int count)
{
    AddByElementIndex(elementIdx, -count);
}

void Distribution::AddByElementIndex(size_t elementIdx, int count)
{
    size_t step = m_initialBitmask;
    size_t idx = 0;
    while(step >= 1)
    {
        m_tree[idx] += count;
        idx += (elementIdx & step) ? step : 1;
        step /= 2;
    }
}

static void DumpImpl(const std::vector<int>& tree, std::vector<int>& result, size_t front, size_t sz, size_t max)
{
    if (sz == 1 && result.size() < max)
    {
        result.push_back(tree[front]);
        return;
    }
    sz = (sz - 1) / 2;
    DumpImpl(tree, result, front + 1, sz, max);
    if (result.size() < max)
    {
        DumpImpl(tree, result, front + 1 + sz, sz, max);
    }
};

std::vector<int> Distribution::Dump() const
{
    std::vector<int> result;
    if (m_originalSize > 0)
    {
        DumpImpl(m_tree, result, 0, m_tree.size(), m_originalSize);
    }
    return result;
}

}
