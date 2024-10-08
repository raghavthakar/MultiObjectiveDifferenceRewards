#ifndef MOD_H
#define MOD_H

#include "environment.h"
#include "team.h"
#include "evolutionary_utils.h"
#include <vector>
#include <string>
#include <utility>
#include <cmath>
#include <unordered_set>
#include <functional>

class MOD;

class MOD {
    int numberOfGenerations;
    int numberOfEpisodes;
    int populationSize;
    unsigned int teamIDCounter; // Tracks the latest team ID
    std::vector<Environment> generateTestEnvironments
        (const std::string& filename);
public:
    std::vector<Individual> population;
    MOD(const std::string& filename);
    void evolve(const std::string& filename, const std::string& data_filename);
};
#endif // MOD_H
