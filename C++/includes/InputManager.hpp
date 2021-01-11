#ifndef INPUTMANAGER_HPP_
#define INPUTMANAGER_HPP_

#include "Subnet.hpp"

#include <array>
#include <vector>
#include <algorithm>
#include <iostream>
#include <regex>
#include <memory>
#include <chrono>
#include <random>

class InputManager {
    public:
        int numberOfNodes;
        //std::vector<Subnet> tree;
        const std::array<std::string,5> commands {"add", "del", "check", "bench", "dump"};   
        const std::string choice {"Please type add/del/check/show/bench command.\n"};
        const std::string addOkNotification {"Add OK\n"};
        const std::string addFailureNotification {"Add Failure\n"};
        const std::string badSyntax {"Bad Syntax\n"};
        const std::string delOkNotification {"Del OK\n"};
        const std::string delFailureNotification {"Del Failure\n"};
        std::unique_ptr<Subnet> root {nullptr};

        InputManager();

        void getCommand();
        void run();
        bool getAddDelArguments(std::string &input,uint32_t &address, uint32_t &mask);
        bool getCheckArgument(std::string &input,uint32_t &address);
        bool getBenchArguments(std::string &input,int &counter, std::vector<uint32_t> &addresses);
        bool validateData(std::string input, std::string command);
        uint32_t ipStringToUint(std::string address);
        uint32_t maskStringToUint(std::string mask);
        uint32_t maskUintToUint(uint32_t &&mask);
        bool isInteger(std::string &input);
        void Test();

};


#endif //INPUTMANAGER_HPP_