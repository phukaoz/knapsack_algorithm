#pragma once

#include "../algorithm.h"

class top_down : public algorithm
{
public:
    void solve(int n, const std::vector<int>& weights, const std::vector<int>& costs, int capacity) override;
    
    const char* get_name() const override { return "Dynamic Programming (top down)"; }

private:
    int recursion(int n, const std::vector<int>& weights, const std::vector<int>& costs, int capacity, std::vector<std::vector<int>>& memo);
};