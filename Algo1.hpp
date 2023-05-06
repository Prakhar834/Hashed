#ifndef ALGO1_H
#define ALGO1_H

#include "BaseStrategy.hpp"

class Algo1: public BaseStrategy {
    private:
        DataManager* close_price;
    public:
        Algo1(std::unordered_map<std::string, std::string> params);
        void run(std::string ts);
};
#endif