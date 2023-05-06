#include "DataManager.hpp"
#include "Utils.hpp"
#include <unordered_map>
#include <iostream>
#include <fstream>
#include <vector>

//Store the instances of DataManager based on data_source_path.
static std::unordered_map<std::string, DataManager*> instances;

//Singleton class behavior for given data_source_path. 
//Thus same memory object will be used by multiple algos

//This function can be used for any data set of TXN matrix style with 
//first line being symbols, others being (Timestamp,value1,value2....), refer to "Close.csv"
//Same function for (RETURNS, CLOSE_PRICES) etc.
DataManager* DataManager::getInstance(std::string path) {
    if (instances.find(path) == instances.end()) {
        instances[path] = new DataManager(path);
    }
    return instances[path];
}
        
DataManager::DataManager(std::string path) {
    values.clear();
    loadData(path);
}

void DataManager::loadData(std::string path) {      
    std::ifstream infile(path);
    std::string line;
    if(infile.is_open()) {
        std::getline(infile, line);
        line = line.substr(1);
        auto tokens = Utils::splitStringHelper(line); 
        columns = tokens;
        int index = 0;
        while(std::getline(infile, line)) {
            std::size_t pos = line.find_first_of(',');
            std::string ts = line.substr(0, pos);
            std::string rest = line.substr(pos+1) + " ";
            std::vector<double> tokens = Utils::splitStringHelperDouble(rest);
            values.push_back(tokens);
            timestamps.push_back(ts);
            ts_index[ts] = index;
            index++;
        }
    } else {
        std::cout << "Error reading file " << path << std::endl;
        exit(0);
    }
}

//Read only reference passed for strategies to work on
const std::vector<std::vector<double>>& DataManager::getValues() {
    return values;
}

std::vector<std::string> DataManager::getColumns() {
    return columns;
}

int DataManager::getIndex(std::string ts) {
    if (ts_index.find(ts) == ts_index.end()) {
        return -1;
    }
    return ts_index[ts];
}
