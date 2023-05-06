#ifndef SIMULATOR_H
#define SIMULATOR_H

#include "BaseStrategy.hpp"
#include <vector>
#include <string>

class Simulator {
    private:
        std::vector<BaseStrategy*> strategies;
        std::vector<std::string> sim_names;
        std::vector<std::string> timestamps;
        std::unordered_map<std::string, std::vector<std::vector<double>>> positions_map;
        std::string stime;
        std::string etime;
        bool use_multiple_threads;
        int bar_freq;
    public:
        Simulator(std::unordered_map<std::string, std::string> global_params, std::vector<std::string> sim_names, std::vector<BaseStrategy*> strategies);
        void start();
        void run(int strat_id);
        void dumpPositions(int strat_id);
        void startSingleThread();
        void startMultipleThreads();

};
#endif