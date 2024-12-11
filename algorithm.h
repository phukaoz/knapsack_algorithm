#pragma once

#include <vector>

class algorithm
{
public:
    virtual ~algorithm() = default;

    virtual void solve(int n, const std::vector<int>& weights, const std::vector<int>& costs, int capacity) = 0;

    int                      getr_best()     const { return m_result.best; }
    float                    getr_time()     const { return m_result.time; }
    const std::vector<bool>& getr_selected() const { return m_result.selected; }

    virtual const char* get_name() const { return "unknown algorithm"; }

protected:
    struct result
    {
        int best;
        float time;
        std::vector<bool> selected;
    };
    result m_result;
};