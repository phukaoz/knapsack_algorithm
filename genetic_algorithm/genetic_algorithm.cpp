#include "genetic_algorithm.h"

#include "../timer.h"

#include <algorithm>
#include <execution>
#include <numeric>

genetic_algorithm::genetic_algorithm(int population_size, int num_generation, double mutation_rate)
    : m_population_size(population_size), m_num_generations(num_generation), m_mutation_rate(mutation_rate)
{
}

void 
genetic_algorithm::solve(int n, const std::vector<int> &weights, const std::vector<int> &costs, int capacity)
{
    timer clock;

    std::vector<std::pair<double, int>> value_density(n);
    for (int i = 0; i < n; ++i) 
    {
        value_density[i] = {static_cast<double>(costs[i]) / weights[i], i};
    }
    std::sort(value_density.begin(), value_density.end(), std::greater<>());

    thread_local std::mt19937 gen(std::random_device{}());

    auto population = initialize_population(n, weights, costs, capacity, value_density, gen);

    std::vector<int> fitness(m_population_size);
    std::vector<std::vector<bool>> new_population;
    new_population.reserve(m_population_size);

    int g_best_fitness = 0;
    std::vector<bool> g_best_individual;
    int generations_without_improvement = 0;

    for (int generation = 0; generation < m_num_generations; ++generation) 
    {
        #pragma omp parallel for
        for (int i = 0; i < m_population_size; ++i) 
        {
            fitness[i] = calculate_fitness(population[i], weights, costs, capacity);
        }

        int elite_size = m_population_size / 10;
        preserve_elite(population, fitness, new_population, elite_size);    

        while (new_population.size() < m_population_size) 
        {
            int tournament_size = std::max(3, m_population_size / 20);
            auto parent1 = select_parent(population, fitness, gen);
            auto parent2 = select_parent(population, fitness, gen);

            auto child = crossover(parent1, parent2, fitness, gen);

            mutate(child, gen, generation);

            optimize(child, weights, costs, capacity);

            if (is_valid(child, weights, capacity)) 
            {
                new_population.push_back(std::move(child));
            }
        }

        population = std::move(new_population);
        new_population.clear();
        new_population.reserve(m_population_size);

        int current_best = *std::max_element(fitness.begin(), fitness.end());
        if (current_best > g_best_fitness) 
        {
            g_best_fitness = current_best;
            auto best_idx = std::max_element(fitness.begin(), fitness.end()) - fitness.begin();
            g_best_individual = population[best_idx];
            generations_without_improvement = 0;
        } 
        else 
        {
            generations_without_improvement++;
        }

        if (generations_without_improvement > 20) 
        {
            break;
        }
    }

    m_result.best     = g_best_fitness;
    m_result.time     = clock.elapsed_millis();
    m_result.selected = std::move(g_best_individual);;
}

std::vector<std::vector<bool>>
genetic_algorithm::initialize_population(int n, const std::vector<int>& weights, const std::vector<int>& costs, int capacity, const std::vector<std::pair<double, int>>& value_density, std::mt19937& gen)
{
    std::vector<std::vector<bool>> population(m_population_size, std::vector<bool>(n));
    std::uniform_int_distribution<> dist(0.0, 1.0);

    int greedy_count = m_population_size / 5;
    for (int i = 0; i < greedy_count; ++i) 
    {
        auto& individual = population[i];
        int current_weight = 0;
        
        for (const auto& [ratio, idx] : value_density) 
        {
            if (current_weight + weights[idx] <= capacity) 
            {
                individual[idx] = true;
                current_weight += weights[idx];
            }
        }
    }
    
    for (int i = greedy_count; i < m_population_size; ++i) 
    {
        for (int j = 0; j < n; ++j) 
        {
            population[i][j] = dist(gen) < 0.5;
        }
    }
    
    return population;
}

void 
genetic_algorithm::preserve_elite(const std::vector<std::vector<bool>> &population, const std::vector<int> &fitness, std::vector<std::vector<bool>> &new_population, int elite_size)
{
    std::vector<int> indices(population.size());
    std::iota(indices.begin(), indices.end(), 0);
    
    std::partial_sort(indices.begin(), indices.begin() + elite_size, indices.end(), [&fitness](int a, int b) { return fitness[a] > fitness[b]; });
    
    for (int i = 0; i < elite_size; ++i) 
    {
        new_population.push_back(population[indices[i]]);
    }
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
genetic_algorithm::select_parent(const std::vector<std::vector<bool>> &population, const std::vector<int> &fitness, std::mt19937 &gen)
{
    int tournament_size = 5;
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
genetic_algorithm::crossover(const std::vector<bool> &parent1, const std::vector<bool> &parent2, const std::vector<int>& fitness, std::mt19937 &gen)
{
    int n = parent1.size();
    std::vector<bool> child(n);
    
    std::uniform_int_distribution<> dist(0, n - 1);
    int point1 = dist(gen);
    int point2 = dist(gen);
    if (point1 > point2) std::swap(point1, point2);
    
    for (int i = 0; i < n; ++i) 
    {
        if (i < point1 || i > point2) 
        {
            child[i] = parent1[i];
        } 
        else 
        {
            child[i] = parent2[i];
        }
    }
    
    return child;
}

void 
genetic_algorithm::mutate(std::vector<bool> &individual, std::mt19937 &gen, int current_generation)
{
    double adaptive_rate = m_mutation_rate * (1.0 - current_generation / static_cast<double>(m_num_generations));
    
    std::uniform_real_distribution<> dist(0.0, 1.0);
    for (size_t i = 0; i < individual.size(); ++i) 
    {
        if (dist(gen) < adaptive_rate) 
        {
            individual[i] = !individual[i];
        }
    }
}

void
genetic_algorithm::optimize(std::vector<bool>& individual, const std::vector<int>& weights, const std::vector<int>& costs, int capacity)
{
    bool improved;
    do 
    {
        improved = false;
        int current_fitness = calculate_fitness(individual, weights, costs, capacity);
        
        for (size_t i = 0; i < individual.size(); ++i) 
        {
            individual[i] = !individual[i];
            int new_fitness = calculate_fitness(individual, weights, costs, capacity);
            
            if (new_fitness <= current_fitness) 
            {
                individual[i] = !individual[i];
            } 
            else 
            {
                improved = true;
                current_fitness = new_fitness;
            }
        }
    } while (improved);
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
