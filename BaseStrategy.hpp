#ifndef BASESTRATEGY_H
#define BASESTRATEGY_H

#include "DataManager.hpp"
#include <unordered_map>
#include <vector>

class BaseStrategy {
    protected:
        std::vector<double> position;
        std::vector<std::string> instruments;

        std::string positions_path;
        std::string simulation_name;
        std::string algo_name;
    public:
        BaseStrategy();
        BaseStrategy(std::unordered_map<std::string, std::string> params);
        virtual void run(std::string ts);
        std::vector<std::string> getInstruments();
        std::vector<double> getPosition();

};
#endif
