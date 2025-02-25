#pragma once
#include <cstdint>
#include <chrono>

namespace ndn::tools
{

class Stopwatch
{
public:
    int64_t Measure() const;
    int64_t MeasureMicros() const;

    void Pause();
    void Resume();
    int64_t Reset(); // returns milliseconds

    static void SleepForMs(uint32_t ms);

    static const Stopwatch& GetGlobalTimer();

private:
    std::chrono::steady_clock::duration _measureDuration() const;
    bool m_isPaused = false;
    std::chrono::steady_clock::time_point m_startTime = std::chrono::steady_clock::now();
    std::chrono::steady_clock::time_point m_pauseStartTime;
    std::chrono::steady_clock::duration m_pauseDuration = {};
};

}
