#include "Algo1.hpp"


Algo1::Algo1(std::unordered_map<std::string, std::string> params) : BaseStrategy(params) {
    close_price = DataManager::getInstance(params["close_price_path"]);
    instruments = close_price->columns;
    position.resize(instruments.size(), 0);
}

void Algo1::run(std::string ts) {
    int ti = close_price->getIndex(ts);
    if (ti < 2 || ti == -1) {
        return;
    }
    for (int i = 0; i < instruments.size(); i++) {
        if (close_price->values[ti-2][i] != 0) {
            position[i] = -(close_price->values[ti-1][i] - close_price->values[ti-2][i])/close_price->values[ti-2][i] * 10000;
        }
     }
}

