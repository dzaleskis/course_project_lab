#pragma once
#include <array>
#include <iostream>
#include <fstream>
#include "openGA.hpp"
#include "json.hpp"
#include "array_utils.hpp"
#include "iterator_utils.hpp"
#include "algorithms.hpp"
#include "benchmarks.hpp"
#include "solution.hpp"

namespace mobj_ga_shellsort {
	
	const int GAP_COUNT = 7;
	const int EVAL_ARRAY_SIZE = 1000;
	const int MIN_GAP_VALUE = 1;
	const int MAX_GAP_VALUE = EVAL_ARRAY_SIZE;
	const char* result_filename = "mobj_ga_shellsort.json";

	using json = nlohmann::json;
	using solution = ga_solution::solution_base<GAP_COUNT, MIN_GAP_VALUE, MAX_GAP_VALUE>;

	struct middle_cost {
		int comparison_count;
		int assignment_count;
	};

	struct optimization_result {
		middle_cost middle_cost;
		solution solution;
	};

	void to_json(json& j, const middle_cost& m_cost) {
		j = json{
			{"comparison_count", m_cost.comparison_count},
			{"assignment_count", m_cost.assignment_count},
		};
	}

	void from_json(const json& j, middle_cost& m_cost) {
		j.at("comparison_count").get_to(m_cost.comparison_count);
		j.at("assignment_count").get_to(m_cost.assignment_count);
    }

	void to_json(json& j, const optimization_result& res) {
        j = json{{"middle_cost", res.middle_cost}, {"solution", res.solution}};
    }

    void from_json(const json& j, optimization_result& res) {
        j.at("middle_cost").get_to(res.middle_cost);
        j.at("solution").get_to(res.solution);
    }

	typedef EA::Genetic<solution, middle_cost> GA_Type;
	typedef EA::GenerationType<solution, middle_cost> Generation_Type;

	void init_genes(solution& p, const std::function<double(void)> &rnd01) {
		p.init();
	}

	bool eval_solution(const solution& p, middle_cost &c) {
		auto ops = benchmarks::measure_ops_int<EVAL_ARRAY_SIZE>(p.gaps);

		c.assignment_count = ops.classic_assignments;
		c.comparison_count = ops.classic_comparisons;

		return true; // genes are accepted
	}

	solution mutate(
	const solution& X_base,
	const std::function<double(void)> &rnd01,
	double shrink_scale)
	{
		return X_base.mutate(rnd01, shrink_scale);
	}

	solution crossover(
		const solution& X1,
		const solution& X2,
		const std::function<double(void)> &rnd01)
	{
		return X1.crossover(X2, rnd01);
	}

	std::vector<double> calculate_MO_objectives(const GA_Type::thisChromosomeType &X)
	{
		const int assignments = X.middle_costs.assignment_count;
		const int comparisons = X.middle_costs.comparison_count;

		return {
			(double)(assignments),
			(double)(comparisons),
		};
	}

		
	void MO_report_generation(
		int generation_number,
		const EA::GenerationType<solution, middle_cost> &last_generation,
		const std::vector<unsigned int>& pareto_front)
	{
		(void) last_generation;
		json j = {
			{"generation_number", generation_number},
			{"pareto_front", pareto_front},
		};

		std::cout << j << std::endl;
	}

	void save_results(const GA_Type &ga_obj)
	{
		std::ofstream output_file;
		output_file.open(result_filename);
		std::vector<unsigned int> paretofront_indices=ga_obj.last_generation.fronts[0];
		std::vector<optimization_result> results;
		results.resize(paretofront_indices.size());
		
		for(unsigned int i: paretofront_indices)
		{
			const auto &X = ga_obj.last_generation.chromosomes[i];
			results[i] = {X.middle_costs, X.genes};
		}

		json j = {
			{"results", results}
		};

		output_file << j << std::endl;
		output_file.close();
	}
}