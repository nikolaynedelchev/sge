#include "game_set.h"
#include <thread>
#include <random>

#define LOCK_FUNCTION() std::lock_guard<std::mutex> guard(m_locker);
namespace ndn::game_set
{

uint64_t GameSet::InitAndGetSnapshotId()
{
    m_rng.seed(1);
}

void GameSet::ResetGameSet(const std::vector<int> &gameset)
{

}

void GameSet::RecoverGames(const std::vector<Game> &games)
{

}

Game GameSet::DrawGame()
{
    LOCK_FUNCTION();
    Game game;
    auto count = m_dist.ElementsCount();
    if (count == 0)
    {
        game.isValid = false;
        return game;
    }
    std::normal_distribution<size_t> normalDist(0, count - 1);
    normalDist(m_rng);
}

size_t GameSet::GetGameSetSize() const
{
    LOCK_FUNCTION();
    return m_dist.ElementsCount();
}

}
