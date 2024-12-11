#include "genetic_algorithm.h"

#include "../timer.h"

#include <random>

void 
genetic_algorithm::solve(int n, const std::vector<int> &weights, const std::vector<int> &costs, int capacity)
{
    timer clock;

    std::vector<std::vector<bool>> population = initialize_population(n);

    for (int generation = 0; generation < m_num_generations; ++generation) 
    {
        std::vector<int> fitness(m_population_size);
        for (int i = 0; i < m_population_size; ++i) 
        {
            fitness[i] = calculate_fitness(population[i], weights, costs, capacity);
        }

        std::vector<std::vector<bool>> newPopulation;
        while (newPopulation.size() < m_population_size) 
        {
            auto parent1 = select_parent(population, fitness);
            auto parent2 = select_parent(population, fitness);

            auto child = crossover(parent1, parent2);

            mutate(child);

            if (is_valid(child, weights, capacity)) 
            {
                newPopulation.push_back(child);
            }
        }

        population = std::move(newPopulation);
    }

    int best_fitness = 0;
    std::vector<bool> best_individual;

    for (const auto& individual : population) 
    {
        int fit = calculate_fitness(individual, weights, costs, capacity);
        if (fit > best_fitness) 
        {
            best_fitness = fit;
            best_individual = individual;
        }
    }

    m_result.best     = best_fitness;
    m_result.time     = clock.elapsed_millis();
    m_result.selected = best_individual;
}

std::vector<std::vector<bool>>
genetic_algorithm::initialize_population(int n)
{
    std::vector<std::vector<bool>> population(m_population_size, std::vector<bool>(n));
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(0, 1);

    for (size_t i = 0; i < population.size(); ++i) 
    {
        for (size_t j = 0; j < population[i].size(); ++j) 
        {
            population[i][j] = dist(gen);
        }
    }
    return population;
}

int 
genetic_algorithm::calculate_fitness(const std::vector<bool> &individual, const std::vector<int> &weights, const std::vector<int> &costs, int capacity)
{
    int total_weight = 0, total_value = 0;
    for (size_t i = 0; i < individual.size(); ++i) 
    {
        if (individual[i]) 
        {
            total_weight += weights[i];
            total_value += costs[i];
        }
    }
    return (total_weight <= capacity) ? total_value : 0;
}

std::vector<bool> 
genetic_algorithm::select_parent(const std::vector<std::vector<bool>> &population, const std::vector<int> &fitness)
{
    int tournament_size = 5;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(0, population.size() - 1);

    int best_index = dist(gen);
    for (int i = 1; i < tournament_size; ++i) 
    {
        int contender = dist(gen);
        if (fitness[contender] > fitness[best_index]) 
        {
            best_index = contender;
        }
    }
    return population[best_index];
}

std::vector<bool> 
genetic_algorithm::crossover(const std::vector<bool> &parent1, const std::vector<bool> &parent2)
{
    int n = parent1.size();
    std::vector<bool> child(n);

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(0, n - 1);

    int crossover_point = dist(gen);
    for (int i = 0; i < n; ++i) 
    {
        child[i] = (i < crossover_point) ? parent1[i] : parent2[i];
    }
    return child;
}

void 
genetic_algorithm::mutate(std::vector<bool> &individual)
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dist(0.0, 1.0);

    for (size_t i = 0; i < individual.size(); ++i) 
    {
        if(dist(gen) < m_mutation_rate)
        {
            individual[i] = !individual[i];
        }
    }
}

bool 
genetic_algorithm::is_valid(const std::vector<bool> &individual, const std::vector<int> &weights, int capacity)
{
    int total_weight = 0;
    for (size_t i = 0; i < individual.size(); ++i) 
    {
        if (individual[i]) 
        {
            total_weight += weights[i];
        }
    }
    return total_weight <= capacity;
}
