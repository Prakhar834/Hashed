#ifndef UTILS_H
#define UTILS_H

#include <vector>
#include <string.h>
#include <cstddef>
#include <string>
#include <sstream>
#include <iostream>

class Utils {
    public:
        static std::vector<double> splitStringHelperDouble(std::string str, char delim=',') {
            std::vector<double> tokens;
            std::istringstream iss(str);
            std::string token;
            while (std::getline(iss, token, delim)) {
                if (token == "" || token == " ") {
                    tokens.push_back(0);
                    continue;
                }
                tokens.push_back(std::stod(token)); 
            }
            return tokens;  
        }

        static std::vector<std::string> splitStringHelper(std::string str, char delim=',') {
            std::vector<std::string> tokens;
            std::istringstream iss(str);
            std::string token;
            while (std::getline(iss, token, delim)) {
                tokens.push_back(token); 
            }
            return tokens;  
        }

        static std::vector<std::string> getTimeRange(int freq, std::string stime, std::string etime) { 
            struct tm sdt_time = getDateTimeStruct(stime);
            struct tm edt_time = getDateTimeStruct(etime);
            std::vector<std::string> time_range;
            for (; mktime(&sdt_time) <= mktime(&edt_time); ++sdt_time.tm_hour) {
                char buffer[40];
                strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", &sdt_time);
                time_range.push_back(std::string(buffer));
            }
            return time_range;
        }

        static tm getDateTimeStruct(std::string ts) {
            struct tm date;
            memset(&date,0,sizeof(date));
            auto tokens = splitStringHelper(ts, ' ');
            auto dt_tokens = splitStringHelper(tokens[0], '-');
            date.tm_mday = std::stoi(dt_tokens[2]);
            date.tm_mon = std::stoi(dt_tokens[1]) - 1;
            date.tm_year = std::stoi(dt_tokens[0]) - 1900;
            if (tokens.size() == 2) {
                auto time_tokens = splitStringHelper(tokens[1], ':');
                date.tm_hour = std::stoi(time_tokens[0]);
                date.tm_min = std::stoi(time_tokens[1]);
                date.tm_sec = std::stoi(time_tokens[2]);
            }
            return date;
        }
};
#endif