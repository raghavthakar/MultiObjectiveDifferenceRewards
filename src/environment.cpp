#include "environment.h"
#include "MOREPDomain.h"
#include "MOBPDomain.h"
#include <iostream>
#include <fstream>
#include <limits>
#include <yaml-cpp/yaml.h>

void Environment::loadConfig(const std::string& filename) {
    YAML::Node config = YAML::LoadFile(filename); // Parse YAML from file

    this->whichDomain = config["experiment"]["domain"].as<std::string>();

    if (this->whichDomain == this->rover_env.whichDomain)
        this->rover_env.loadConfig(filename);
    else if (this->whichDomain == this->beach_env.whichDomain)
        this->beach_env = MOBPDomain(filename);
    else {
        throw std::runtime_error("Domain is unrecognized!");
    }
}

// compute the rewards generated by the provided agents configuration
std::vector<double> Environment::getRewards(std::vector<std::vector<double>> agentPositions, int stepNumber) {

    if (this->whichDomain == rover_env.whichDomain) {
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
    else {
        throw std::runtime_error("Domain is unrecognized!");
    }
    
}

std::vector<double> Environment::getRewards(std::vector<std::vector<double>> agentPositions, std::vector<unsigned short int> agentTypes, int stepNumber) {
    if (this->whichDomain == this->beach_env.whichDomain) {
        std::vector<unsigned short int> positionVector;

        // convert vector of vector of doubles to vector of unsigned short ints
        for (const auto& innerVector : agentPositions) {
            if (innerVector.size() != 1) {
                std::cerr << "Error: Each inner vector must contain exactly one element." << std::endl;
                exit(1);
            }
            // Convert double to unsigned short int
            if (innerVector[0] < 0 || innerVector[0] > std::numeric_limits<unsigned short int>::max()) {
                std::cerr << "Error: Value out of range for unsigned short int." << std::endl;
                exit(1);
            }
            positionVector.push_back(static_cast<unsigned short int>(innerVector[0]));
        }

        // get the reward for this position and type configuration of agents
        return (this->beach_env.getRewards(positionVector, agentTypes));
    }
    else {
        throw std::runtime_error("Domain is unrecognized!");
    }
}

// take in an agent's position and return new position based on environmental limits
std::pair<double, double> Environment::moveAgent(std::pair<double, double> currentPos, std::pair<double, double> delta, double maxStepSize) {
    if (this->whichDomain == this->rover_env.whichDomain) {
        return this->rover_env.moveAgent(currentPos,delta, maxStepSize);
    }
    else {
        throw std::runtime_error("Domain is unrecognized!");
    }
}

// take in an agent's position and return new position based on environmental limits
// overloaded for MOBP
unsigned short int Environment::moveAgent(unsigned short int currentPos, short int delta) {
    if (this->whichDomain == this->beach_env.whichDomain) {
        return this->beach_env.moveAgent(currentPos, delta);
    }
    else {
        throw std::runtime_error("Domain is unrecognized!");
    }
}

// observations of an agent
std::vector<double> Environment::getAgentObservations(std::pair<double, double> agentPos, int numberOfSensors, double observationRadius, std::vector<std::pair<double, double>> agentPositions) {
    return this->rover_env.getAgentObservations(agentPos, numberOfSensors, observationRadius, agentPositions);
}

// generate a counterfactual trajectory for rovers
std::vector<std::vector<double>> Environment::generateCounterfactualTrajectory(const std::string& config_filename, const std::string& counterfactualType, int trajectoryLength) {
    if (this->whichDomain == this->rover_env.whichDomain) {
        return this->rover_env.generateCounterfactualTrajectory(config_filename, counterfactualType, trajectoryLength);
    }
    else if (this->whichDomain == this->beach_env.whichDomain) {
        return this->rover_env.generateCounterfactualTrajectory(config_filename, counterfactualType, trajectoryLength);
    }
    else {
        throw std::runtime_error("Domain is unrecognized!");
    }
}

// generate a counterfactual trajectory fopr beach agent
std::vector<std::vector<double>> Environment::generateCounterfactualTrajectory(const std::string& config_filename, const std::string& counterfactualType, int trajectoryLength, unsigned short int startingPos) {
    if (this->whichDomain == this->beach_env.whichDomain) {
        std::vector<std::vector<unsigned short int>> input =  this->beach_env.generateCounterfactualTrajectory(config_filename, counterfactualType, trajectoryLength, startingPos);

        // convert dvector of vector of unsigned short ints to a vector of vector of doubles
        std::vector<std::vector<double>> result;
        result.reserve(input.size()); // Reserve space to avoid reallocations

        for (const auto& innerVec : input) {
            std::vector<double> doubleInnerVec;
            doubleInnerVec.reserve(innerVec.size()); // Reserve space for inner vector
            for (unsigned short int val : innerVec) {
                doubleInnerVec.push_back(static_cast<double>(val)); // Convert to double
            }
            result.push_back(doubleInnerVec);
        }


        return result;
    }
    else {
        throw std::runtime_error("Domain is unrecognized!");
    }
}

// initialise zero rewards for an episode // intiialise zero reward for an episode
std::vector<double> Environment::initialiseEpisodeReward(const std::string& config_filename) {
    if (this->whichDomain == this->rover_env.whichDomain) {
        return this->rover_env.initialiseEpisodeReward(config_filename);
    }
    else if (this->whichDomain == this->beach_env.whichDomain) {
        return this->beach_env.initialiseEpisodeReward(config_filename);
    }
    else {
        throw std::runtime_error("Domain is unrecognized!");
    }
}

void Environment::reset() {
    this->rover_env.reset();
    this->beach_env.reset();
}

// return h prnatly of the enviroment
int Environment::getPenalty() {
    if (this->whichDomain == this->rover_env.whichDomain) {
        return this->rover_env.getPenalty();
    }
    else if (this->whichDomain == this->beach_env.whichDomain) {
        return 0;
    }
    else {
        throw std::runtime_error("Domain is unrecognized!");
    }
}
