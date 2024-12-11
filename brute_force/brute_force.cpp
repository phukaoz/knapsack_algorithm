#include "brute_force.h"

#include "../timer.h"

#include <algorithm>

int 
brute_force::recursion(int n, const std::vector<int>& weights, const std::vector<int>& costs, int capacity)
{
    if (n == 0 || capacity == 0)
        return 0;

    int exclude = recursion(n - 1, weights, costs, capacity);

    int include = 0;
    if (weights[n - 1] <= capacity)
    {
        include = costs[n - 1] + recursion(n - 1, weights, costs, capacity - weights[n - 1]);
    }

    return std::max(exclude, include);
}

void 
brute_force::solve(int n, const std::vector<int>& weights, const std::vector<int>& costs, int capacity)
{
    timer clock;
    m_result.best = recursion(n, weights, costs, capacity);
    m_result.time = clock.elapsed_millis();
}