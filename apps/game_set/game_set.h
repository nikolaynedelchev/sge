#pragma once
#include <cinttypes>
#include <vector>
#include "distribution.h"
namespace ndn::game_set
{

struct Game
{
    uint64_t snapshotId;
    int win;
};

class GameSet
{
public:
    uint64_t InitAndGetSnapshotId();
    void ResetGameSet(const std::vector<int>& gameset);
    void RecoverGames(const std::vector<size_t>& games);
    Game DrawGame();

private:
    Distribution m_dist;
};

}
