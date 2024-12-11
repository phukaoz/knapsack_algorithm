#pragma once

#include "../algorithm.h"

class genetic_algorithm : public algorithm
{
public:
    void solve(int n, const std::vector<int>& weights, const std::vector<int>& costs, int capacity) override;

    const char* get_name() const override { return "Genetic Algorithm"; }

private:
    std::vector<std::vector<bool>> initialize_population(int n);
    int                            calculate_fitness    (const std::vector<bool>& individual, const std::vector<int>& weights, const std::vector<int>& costs, int capacity);
    std::vector<bool>              select_parent        (const std::vector<std::vector<bool>>& population, const std::vector<int>& fitness);
    std::vector<bool>              crossover            (const std::vector<bool>& parent1, const std::vector<bool>& parent2);
    void                           mutate               (std::vector<bool>& individual);
    bool                           is_valid             (const std::vector<bool>& individual, const std::vector<int>& weights, int capacity);

private:
    int m_population_size = 100;
    int m_num_generations = 200; 
    double m_mutation_rate = 0.025;
};