#ifndef DATAMANAGER_H
#define DATAMANAGER_H

#include <vector>
#include <unordered_map>
#include <string>

class DataManager {
    public:
        std::vector<std::string> columns;
        std::vector<std::vector<double>> values;
        std::vector<std::string> timestamps;
        std::unordered_map<std::string, int> ts_index;
    public:
        static DataManager* getInstance(std::string path);
        DataManager(std::string path);
        void loadData(std::string path);
        int getIndex(std::string ts);
        // const std::vector<double> getValues(std::string ts) 
        // const std::vector<double> getValues(int index)

};
#endif