#include "Algo1.hpp"


Algo1::Algo1(std::unordered_map<std::string, std::string> params) : BaseStrategy(params) {
    close_price = DataManager::getInstance(params["close_price_path"]);
    instruments = close_price->getColumns();
    position.resize(instruments.size(), 0);
}

void Algo1::run(std::string ts) {
    int ti = close_price->getIndex(ts);
    if (ti < 2 || ti == -1) {
        return;
    }
    
    auto& prices = close_price->getValues();
    for (int i = 0; i < instruments.size(); i++) {
        if (prices[ti-2][i] != 0) {
            position[i] = -(prices[ti-1][i] - prices[ti-2][i])/prices[ti-2][i] * 10000;
        }
     }
}