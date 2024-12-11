#pragma once

#include "../algorithm.h"

class brute_force : public algorithm
{
public:
    void solve(int n, const std::vector<int>& weights, const std::vector<int>& costs, int capacity) override;

    const char* get_name() const override { return "Brute Force (recurrion)"; }

private:
    int recursion(int n, const std::vector<int>& weights, const std::vector<int>& costs, int capacity);
};