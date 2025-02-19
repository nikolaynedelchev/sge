#pragma once
#include <cinttypes>
#include <vector>
#include "distribution.h"
#include <mutex>
#include <random>

namespace ndn::game_set
{

struct Game
{
    bool isValid;
    uint64_t snapshotId;
    int win;
};

class GameSet
{
public:
    uint64_t InitAndGetSnapshotId();
    void ResetGameSet(const std::vector<int>& gameset);
    void RecoverGames(const std::vector<Game>& games);
    Game DrawGame();
    size_t GetGameSetSize() const;

private:
    std::mt19937 m_rng;
    Distribution m_dist;
    int m_gamesPlayed = 0;
    uint64_t m_snapshotId = 0;
    mutable std::mutex m_locker;
};

}
