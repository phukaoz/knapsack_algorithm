#pragma once

#include "../algorithm.h"

class bottom_up : public algorithm
{
public:
    void solve(int n, const std::vector<int>& weights, const std::vector<int>& costs, int capacity) override;

    const char* get_name() const override { return "Dynamic Programming (bottom up)"; }
};