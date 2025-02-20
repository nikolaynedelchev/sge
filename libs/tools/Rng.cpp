#include "Rng.h"
#include <random>
#include <stdexcept>
namespace ndn::tools
{

namespace
{
template<typename T = std::mt19937>
auto GetEngine() -> T&
{
    static auto engine = []()
    {
        std::random_device randomDevice;
        T engine{randomDevice()};
        return engine;
    }();
    return engine;
}
}

uint64_t Rng::GetRandom(uint64_t maxIncluding, uint64_t minIncluding)
{
    auto& engine = GetEngine<std::mt19937>();

    std::uniform_int_distribution<uint64_t> randomDistribution(minIncluding, maxIncluding); // including min and max

    return randomDistribution(engine);
}

uint64_t Rng::MergeUpperBounds(const std::vector<uint64_t> &upperBoundsIncluding)
{
    if (upperBoundsIncluding.empty())
    {
        throw std::logic_error("merging empty upper bounds array");
    }
    uint64_t mergedBound = 1;
    for(uint64_t bound : upperBoundsIncluding)
    {
        uint64_t size = bound;
        size++;
        uint64_t tmp = mergedBound * size;
        if (tmp / size != mergedBound)
        {
            throw std::logic_error("merging upper bounds overflow");
        }
        mergedBound = tmp;
    }
    return mergedBound - 1;
}

uint64_t Rng::GenerateBigNumber(const std::vector<uint64_t> &upperBoundsIncluding, const std::vector<uint64_t> &smallRandomNubers)
{
    if (upperBoundsIncluding.size() != smallRandomNubers.size())
    {
        throw std::logic_error("different bounds and numbers sizes");
    }
    uint64_t bigRandomNumber = 0;
    uint64_t product = 1;
    for (std::size_t idx = 0; idx < upperBoundsIncluding.size(); idx++)
    {
        uint64_t number = smallRandomNubers[idx];
        uint64_t bound = upperBoundsIncluding[idx];
        uint64_t size = bound + 1;
        bigRandomNumber += product * number;
        product *= size;
    }
    return bigRandomNumber;
}

std::vector<uint32_t> Rng::SplitBigNumber(const std::vector<uint64_t> &upperBoundsIncluding,
                                          uint64_t bigRandomNumber,
                                          uint64_t bigRandomUpperBoundIncluding)
{
    uint64_t targetUpperBound = MergeUpperBounds(upperBoundsIncluding);
    uint64_t targetDistributionSize = targetUpperBound + 1;

    // prevent overflow for
    // t = (bigRandomUpperBoundIncluding + 1) / (targetUpperBound + 1);
    // in this way bigRandomUpperBoundIncluding can be 2^64
    uint64_t t = bigRandomUpperBoundIncluding / targetDistributionSize;
    {
        // we add 1 to the remainder to take the remainder as if we devide (bigRandomUpperBoundIncluding + 1) / (targetUpperBound + 1)
        uint64_t r = bigRandomUpperBoundIncluding % targetDistributionSize + 1;
        if (r == targetDistributionSize)
        {
            // devision with no remainder (r = 0)
            t++;
        }
    }

    // prevent overflow for
    // lastValid = t * targetDistributionSize - 1
    uint64_t lastValid = (t-1) * targetDistributionSize;
    lastValid -= 1;
    lastValid += targetDistributionSize;
    if (bigRandomNumber > lastValid)
    {
        return {};
    }
    uint64_t randomNumber = bigRandomNumber % targetDistributionSize;
    uint64_t product = 1;
    std::vector<uint32_t> result;
    result.reserve(upperBoundsIncluding.size());
    for (uint64_t bound : upperBoundsIncluding)
    {
        if (bound > std::numeric_limits<uint32_t>::max())
        {
            throw std::logic_error("splitting big number to invalid bound");
        }
        uint64_t size = bound + 1;
        uint64_t number = uint64_t((randomNumber / product) % size);
        result.push_back( uint32_t(number) );
        product *= size;
    }
    return result;
}

std::vector<uint32_t> Rng::TransformNumbers(const std::vector<uint64_t> &sourceUpperBoundsIncluding, const std::vector<uint64_t> &randomNumbers, const std::vector<uint64_t> &targetBoundsIncluding)
{
    auto bigNumber = GenerateBigNumber(sourceUpperBoundsIncluding, randomNumbers);
    return SplitBigNumber(targetBoundsIncluding, bigNumber);
}

std::map<uint32_t, std::size_t> Rng::CreateDistribution(const std::vector<double> &dist)
{
    std::map<uint32_t, std::size_t> res;
    double sum = 0.0;
    for(auto v : dist) sum += v;
    uint32_t val = 0;

    for(std::size_t idx = 0; idx < dist.size(); idx++)
    {
        uint32_t change = uint32_t((dist[idx] / sum) * double(~uint32_t(0)));
        if (0 == change)
        {
            throw std::logic_error("too small differences in the distribution");
        }
        val += change;
        if (idx == dist.size() - 1)
        {
            val = ~uint32_t(0);
        }
        res[val] = idx;
    }
    return res;
}

size_t Rng::GetRandomElementFromDistribution(const std::map<uint32_t, std::size_t> &distribution)
{
    auto it = distribution.lower_bound( static_cast<uint32_t>(GetRandom(~uint32_t(0))) );
    if (it == distribution.end())
    {
        return distribution.begin()->second;
    }
    return it->second;
}

}

