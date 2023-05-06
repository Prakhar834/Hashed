#include "Simulator.hpp"
#include "BaseStrategy.hpp"
#include "Utils.hpp"
#include "fstream"
#include <thread>

Simulator::Simulator(std::unordered_map<std::string, std::string> global_params, std::vector<std::string> sim_names, std::vector<BaseStrategy*> strategies) {
    //bar_freq here is defaulted to 60 minutes -> 1 hr
    //Can easily use it, currently not used and assumes 60 minutes data only
    this->bar_freq = std::stoi(global_params["bar_freq"]);
    this->stime = global_params["stime"];
    this->etime = global_params["etime"];
    this->use_multiple_threads = bool(std::stoi(global_params["use_multiple_threads"]));
    this->strategies = strategies;
    this->sim_names = sim_names;
    this->timestamps = Utils::getTimeRange(bar_freq, stime, etime);
}

void Simulator::start() {
    if (use_multiple_threads) {
        startMultipleThreads();
    } else {
        startSingleThread();
    }
}

//................Multiple_Threaded...................
void Simulator::startMultipleThreads() {
    std::cout << "Running in multiple threads" << std::endl;
    std::vector<std::thread> threads;
    for (int i = 0; i < strategies.size(); i++) {
        threads.push_back(std::move(std::thread(&Simulator::run, this, i)));    
    }
    for (int i = 0; i < threads.size(); i++) {
        threads[i].join();
    }
}

void Simulator::run(int strat_id) {
    std::cout << "Simulation started for: " << sim_names[strat_id] << std::endl;
    auto& strat = strategies[strat_id];
    auto& sim_name = sim_names[strat_id];
    for (auto ti: timestamps) {
        strat->run(ti);
        positions_map[sim_name].push_back(strat->getPosition());
    }
    dumpPositions(strat_id); 
    std::cout << "Simulation ends for: " << sim_name << std::endl;
}

//..................Single_Threaded......................
void Simulator::startSingleThread() {
    std::cout << "Running in single thread" << std::endl;
    for (auto ti: timestamps) {
        for (int i = 0; i < strategies.size(); i++) {
            auto& strat = strategies[i];
            auto& sim_name = sim_names[i];
            strat->run(ti);
            positions_map[sim_name].push_back(strat->getPosition());
        }
    }
    for (int i = 0; i < strategies.size(); i++) {
        dumpPositions(i);
    }
}

void Simulator::dumpPositions(int strat_id) {
    BaseStrategy* strat = strategies[strat_id];
    const auto& instruments = strat->getInstruments();
    const auto& sim_name = sim_names[strat_id];
    const auto& positions_path = sim_name + ".csv";
    const auto& positions = positions_map[sim_name];

    std::cout << "Dumping positions at: " << positions_path << std::endl; 
    std::ofstream pfile(positions_path);
    if (pfile.is_open()) {
        //Dumping headers
        std::string headers;
        for (auto& instrument: instruments) {
            headers += ',' + instrument;
        }
        pfile << headers << std::endl;
        //Dumping positions
        for (int i = 0; i < timestamps.size(); i++) {
            std::string content;
            content += timestamps[i] + ",";
            for (auto& position: positions[i]) {
                if (position != 0) {
                   content += std::to_string(position);
                }
                content += ",";
            }
            content.pop_back();
            content += '\n';
            pfile << content;
        }
    }
    pfile.close();
}

