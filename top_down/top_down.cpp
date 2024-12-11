#include "top_down.h"

#include "../timer.h"

#include <algorithm>

int 
top_down::recursion(int n, const std::vector<int>& weights, const std::vector<int>& costs, int capacity, std::vector<std::vector<int>>& memo)
{
    if (n == 0 || capacity == 0)
        return 0;

    if (memo[n][capacity] != -1)
        return memo[n][capacity];

    if (weights[n - 1] <= capacity)
    {
        memo[n][capacity] = std::max(
            recursion(n - 1, weights, costs, capacity, memo),
            recursion(n - 1, weights, costs, capacity - weights[n - 1], memo) + costs[n - 1]);
    }
    else
    {
        memo[n][capacity] = recursion(n - 1, weights, costs, capacity, memo);
    }

    return memo[n][capacity];
}

void 
top_down::solve(int n, const std::vector<int>& weights, const std::vector<int>& costs, int capacity)
{
    timer clock;
    std::vector<std::vector<int>> memo(n + 1, std::vector<int>(capacity + 1, -1));
    m_result.best = recursion(n, weights, costs, capacity, memo);
    m_result.time = clock.elapsed_millis();
}