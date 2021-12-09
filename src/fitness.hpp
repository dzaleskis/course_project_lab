#pragma once
#include <cmath>
#include "element.hpp"
#include "iterator_utils.hpp"
#include "sorting_algorithm.hpp"

struct SortingStats {
    double avg_inversions;
    double avg_comparisons;
    double avg_assignments;
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(SortingStats, avg_inversions, avg_assignments, avg_comparisons)

SortingStats get_sorting_stats(const AlgorithmBlueprint& algorithmBlueprint, int size, int runs) {
    auto elements = std::vector<Element<int>>(size);
    auto algorithm = ConcreteAlgorithmFactory::getConcreteAlgorithm<typeof elements>(algorithmBlueprint);

    double total_inversions = 0;
    double total_comparisons = 0;
    double total_assignments = 0;

    for (int i = 0; i < runs; i++) {
        utils::fill_random(elements.begin(), elements.end());

        auto report = global_measure.withReport([&]() {
            algorithm->sort(elements);
        });

        total_inversions += utils::count_inversions(elements.begin(), elements.end());
        total_comparisons += report.comparisons;
        total_assignments += report.assignments;
    }

    auto stats = SortingStats { total_inversions / runs, total_comparisons / runs, total_assignments / runs };

    return stats;
}

// lower is better
double evaluate_fitness(const SortingStats& avgSortingStats) {
    auto inversions = avgSortingStats.avg_inversions;
    auto comparisons = avgSortingStats.avg_comparisons;
    auto assignments = avgSortingStats.avg_assignments;

    auto efficiency = std::pow(inversions, 2.0) + ((comparisons / assignments) * (comparisons + assignments));

    return efficiency;
}