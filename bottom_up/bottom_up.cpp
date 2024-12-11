#include "bottom_up.h"

#include "../timer.h"

#include <algorithm>

void 
bottom_up::solve(int n, const std::vector<int> &weights, const std::vector<int> &costs, int capacity)
{
    timer clock;

    int i, w;
    std::vector<std::vector<int>> dp(n + 1, std::vector<int>(capacity + 1, 0));

    for (i = 1; i <= n; i++)
    {
        for (w = 1; w <= capacity; w++)
        {
            if (weights[i - 1] <= w)
            {
                dp[i][w] = std::max(dp[i - 1][w], dp[i - 1][w - weights[i - 1]] + costs[i - 1]);
            }
            else
            {
                dp[i][w] = dp[i - 1][w];
            }
        }
    }

    m_result.best = dp[n][capacity];
    m_result.time = clock.elapsed_millis();
}
