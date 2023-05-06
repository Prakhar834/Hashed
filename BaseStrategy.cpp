#include "BaseStrategy.hpp"
#include <iostream>
#include <fstream>
#include <iomanip>

BaseStrategy::BaseStrategy() {}
BaseStrategy::BaseStrategy(std::unordered_map<std::string, std::string> params) {}

void BaseStrategy::run(std::string ts) {}

std::vector<double> BaseStrategy::getPosition() {
    return position;
}

std::vector<std::string> BaseStrategy::getInstruments() {
    return instruments;
}

