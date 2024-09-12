#include "environment.h"
#include "MOREPDomain.h"
#include "MOBPDomain.h"
#include <iostream>
#include <fstream>
#include <limits>
#include <yaml-cpp/yaml.h>

void Environment::loadConfig(const std::string& filename) {
    this->rover_env.loadConfig(filename);
}

// compute the rewards generated by the provided agents configuration
std::vector<double> Environment::getRewards(std::vector<std::vector<double>> agentPositions,
                                        int stepNumber) {
                                            // Convert std::vector<std::vector<double>> to std::vector<std::pair<double, double>>
    std::vector<std::pair<double, double>> convertedPositions;
    convertedPositions.reserve(agentPositions.size());

    for (const auto& position : agentPositions) {
        // Assuming each inner vector has exactly 2 elements
        if (position.size() == 2) {
            convertedPositions.emplace_back(position[0], position[1]);
        } else {
            throw std::runtime_error("Each position vector must have exactly 2 elements.");
        }
    }

    auto episodeReward = rover_env.getRewards(convertedPositions, stepNumber);
    // Initialize the vector of doubles with the same size as the int vector
    std::vector<double> convertedEpisodeReward(episodeReward.size());

    // Manually copy and convert each element
    for (size_t i = 0; i < episodeReward.size(); ++i) {
        convertedEpisodeReward[i] = static_cast<double>(episodeReward[i]);
    }

    return convertedEpisodeReward;
}

// take in an agent's position and return new position based on environmental limits
std::pair<double, double> Environment::moveAgent(std::pair<double, double> currentPos, std::pair<double, double> delta, double maxStepSize) {
    return this->rover_env.moveAgent(currentPos,delta, maxStepSize);
}

// observations of an agent
std::vector<double> Environment::getAgentObservations(std::pair<double, double> agentPos, int numberOfSensors, double observationRadius, std::vector<std::pair<double, double>> agentPositions) {
    return this->rover_env.getAgentObservations(agentPos, numberOfSensors, observationRadius, agentPositions);
}

// generate a counterfactual trajectory 
std::vector<std::vector<double>> Environment::generateCounterfactualTrajectory(const std::string& config_filename, const std::string& counterfactualType, int trajectoryLength) {
    return this->rover_env.generateCounterfactualTrajectory(config_filename, counterfactualType, trajectoryLength);
}

// initialise zero rewards for an episode // intiialise zero reward for an episode
std::vector<double> Environment::initialiseEpisodeReward(const std::string& config_filename) {
    return this->rover_env.initialiseEpisodeReward(config_filename);
}

void Environment::reset() {
    this->rover_env.reset();
}
