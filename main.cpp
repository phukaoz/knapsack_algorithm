#include "bottom_up/bottom_up.h"
#include "top_down/top_down.h"
#include "genetic_algorithm/genetic_algorithm.h"
#include "brute_force/brute_force.h"

#include <iostream>
#include <memory>

void print_result(std::shared_ptr<algorithm> algo)
{
    std::cout << algo->get_name() << "'s result: \n";
    std::cout << "solution: " << algo->getr_best() << "\n";
    std::cout << "time    : " << algo->getr_time() << "ms\n";
}

int main(int argc, char* argv[])
{
    int n, capacity;
    std::vector<int> weights, costs;
    
    std::cin >> n;
    weights.resize(n);  
    costs.resize(n);
    for(int i = 0; i < n; ++i)
    {
        std::cin >> weights[i] >> costs[i];
    }
    std::cin >> capacity;

    std::vector<std::shared_ptr<algorithm>> algorithms = {
        std::make_shared<top_down>(),
        std::make_shared<genetic_algorithm>(),
        std::make_shared<bottom_up>(),
        std::make_shared<brute_force>()
    };

    for(auto algo : algorithms)
    {
        algo->solve(n, weights, costs, capacity);
        print_result(algo);
    }
}