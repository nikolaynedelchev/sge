#pragma once
#include <cinttypes>
#include <vector>
#include <map>
#include <limits>
#include <cstddef>

namespace ndn::tools
{

struct Rng
{
    static uint64_t GetRandom(uint64_t maxIncluding, uint64_t minIncluding = 0);

    static uint64_t MergeUpperBounds(const std::vector<uint64_t>& upperBoundsIncluding);

    static uint64_t GenerateBigNumber(const std::vector<uint64_t>& upperBoundsIncluding,
                                      const std::vector<uint64_t>& smallRandomNubers);

    static std::vector<uint32_t> SplitBigNumber(const std::vector<uint64_t>& upperBoundsIncluding,
                                                uint64_t bigRandomNumber,
                                                uint64_t bigRandomUpperBoundIncluding = std::numeric_limits<uint64_t>::max());

    static std::vector<uint32_t> TransformNumbers(const std::vector<uint64_t>& sourceUpperBoundsIncluding,
                                                  const std::vector<uint64_t>& randomNumbers,
                                                  const std::vector<uint64_t>& targetBoundsIncluding);

    template<typename Type>
    static Type GetRandomElement(const std::vector<Type>& c)
    {
        return c[GetRandom(c.size() - 1)];
    }

    static std::map<uint32_t, std::size_t> CreateDistribution(const std::vector<double>&);
    static std::size_t GetRandomElementFromDistribution(const std::map<uint32_t, std::size_t>& distribution);
};

}
