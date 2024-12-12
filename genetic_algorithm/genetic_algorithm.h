#pragma once

#include "../algorithm.h"

#include <random>

class genetic_algorithm : public algorithm
{
public:
    genetic_algorithm() = default;
    genetic_algorithm(int population_size, int num_generation, double mutation_rate);

    void solve(int n, const std::vector<int>& weights, const std::vector<int>& costs, int capacity) override;

    const char* get_name() const override { return "Genetic Algorithm"; }

private:
    std::vector<std::vector<bool>> initialize_population(int n, const std::vector<int>& weights, const std::vector<int>& costs, int capacity, const std::vector<std::pair<double, int>>& value_density, std::mt19937& gen);
    void                           preserve_elite       (const std::vector<std::vector<bool>>& population, const std::vector<int>& fitness, std::vector<std::vector<bool>>& new_population, int elite_size);
    int                            calculate_fitness    (const std::vector<bool>& individual, const std::vector<int>& weights, const std::vector<int>& costs, int capacity);
    std::vector<bool>              select_parent        (const std::vector<std::vector<bool>>& population, const std::vector<int>& fitness, std::mt19937 &gen);
    std::vector<bool>              crossover            (const std::vector<bool>& parent1, const std::vector<bool>& parent2, const std::vector<int>& fitness, std::mt19937 &gen);
    void                           mutate               (std::vector<bool>& individual, std::mt19937 &gen, int current_generation);
    void                           optimize             (std::vector<bool>& individual, const std::vector<int>& weights, const std::vector<int>& costs, int capacity);
    bool                           is_valid             (const std::vector<bool>& individual, const std::vector<int>& weights, int capacity);

private:
    int m_population_size = 300;
    int m_num_generations = 1200; 
    double m_mutation_rate = 0.005;
};