#pragma once
#include <deque>
#include <numeric>

class NumberSmoothing
{
public:
    NumberSmoothing(size_t size): m_averageQueue(size, 0.0) {}

    double push(double value)
    {
        m_averageQueue.pop_front();
        m_averageQueue.push_back(value);
        return std::accumulate(m_averageQueue.begin(), m_averageQueue.end(), 0.0) /
               static_cast<double>(m_averageQueue.size());
    }

private:
    std::deque<double> m_averageQueue;
};
