#ifndef ALGO2_H
#define ALGO2_H

#include "BaseStrategy.hpp"

class Algo2: public BaseStrategy {
    private:
        DataManager* close_price;
        std::vector<double> mavg_sum;
        int window;
    public:
        Algo2(std::unordered_map<std::string, std::string> params);
        void run(std::string ts);
        void calculateMovingAverageSum(int ti);
};
#endif