#include "Algo2.hpp"

//Algo logic:
//Take 1000 * current_price worth positions 
//whenever current prices crosses moving average price of given window size
//else take 0 position
//MAVG is calculated in O(1) per symbol on iteration
Algo2::Algo2(std::unordered_map<std::string, std::string> params): BaseStrategy(params) {
    close_price = DataManager::getInstance(params["close_price_path"]);
    instruments = close_price->columns;
    position.resize(instruments.size(), 0);
    mavg_sum.resize(instruments.size(), 0);
    window = std::stoi(params["maw"]); // Read maw -> (moving average window size), param from config
}

void Algo2::run(std::string ts) {
    int ti = close_price->getIndex(ts);
    calculateMovingAverageSum(ti);
    if (ti < window) {
        return;
    }
    for (int i = 0; i < instruments.size(); i++) {
        double mavg = mavg_sum[i] / window;
        if (close_price->values[ti][i] > mavg) {
            position[i] = close_price->values[ti][i] * 1000;
        } else {
            position[i] = 0;
        }
    }
}

//Note: Here MAVG is fetching in O(1) time for a particular symbol
//Storing moving average sum on each iteration.
//removing value from end of queue when window size passes threshold.
void Algo2::calculateMovingAverageSum(int ti) {
    for (int i = 0; i < instruments.size(); i++) {
        mavg_sum[i] += close_price->values[ti][i];
        if (ti - window >= 0) {
          mavg_sum[i] -= close_price->values[ti-window][i];
        }
    }
}
