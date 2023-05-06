#include "BaseStrategy.hpp"
#include "Algo1.hpp"
#include "Algo2.hpp"
#include "Simulator.hpp"
#include "Utils.hpp"
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <unordered_map>

struct AlgoParams {
    std::string simulation_name;
    std::string algo_name;
    std::unordered_map<std::string, std::string> params;
};

void parseSimConfig(
        std::string path, 
        std::unordered_map<std::string, std::string>& globalParams,
        std::unordered_map<std::string, AlgoParams>& algoParams
        ) {
    std::ifstream infile(path);
    std::string line;
    if (infile.is_open()) {
        std::getline(infile, line);
        auto tokens = Utils::splitStringHelper(line);
        //Reading global simulation parameters
        for (int i = 0; i < tokens.size();) {
            globalParams[tokens[i]] = tokens[i+1];
            i += 2;
        }
        while(std::getline(infile, line)) {
            std::size_t pos = line.find_first_of(',');
            std::string simulation_name = line.substr(0, pos);
            std::size_t pos1= line.substr(pos+1).find_first_of(',');
            std::string algo_name = line.substr(pos+1, pos1);
            std::string rest = line.substr(pos + pos1 + 2);
 
            //Reading simulation_name and algo_name
            AlgoParams aparams;
            aparams.simulation_name = simulation_name;
            aparams.algo_name = algo_name;
            if (simulation_name == "") {continue;}
    
            //Reading algo parameters
            std::vector<std::string> tokens = Utils::splitStringHelper(rest);
            for (int i = 0; i < tokens.size();) {
                aparams.params[tokens[i]] = tokens[i+1];
                i += 2;
            }
            algoParams[simulation_name] = aparams;
        }
    }
}

int main(int argc, char *argv[]) {
    std::string simconfigpath = "Simconfig.csv"; 
    if (argc == 2) {
        simconfigpath = std::string(argv[1]);
        std::cout << "Read simconfig path from cmd arg: " << simconfigpath << std::endl;
    }
    std::unordered_map<std::string, AlgoParams> algoParams;
    std::unordered_map<std::string, std::string> globalParams;
    parseSimConfig(simconfigpath, globalParams, algoParams);

    //Using BaseStrategy pointer with ActualStrat object,
    //Allowing dynamic polymorphism of methods in BaseStrat
    std::vector<BaseStrategy*> strategies;
    std::vector<std::string> sim_names;

    //Creating dynamic instances of algos
    for (const auto& itr: algoParams) {
        std::string algo_name = itr.second.algo_name;
        sim_names.push_back(itr.first);
        auto params = itr.second.params;
        if (algo_name == "Algo1") {
            strategies.push_back(new Algo1(params));
        } else if (algo_name == "Algo2") {
            strategies.push_back(new Algo2(params));
        } else {
            std::cout << "Algo not known: " << algo_name << std::endl;
            exit(0);
        }
    }
    
    Simulator sim_engine(globalParams, sim_names, strategies);
    std::cout << "Simulation started" << std::endl;
    //Simulation starts
    sim_engine.start();
    std::cout << "Simulation ends" << std::endl;
    return 0;
}