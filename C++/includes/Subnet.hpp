#ifndef SUBNET_HPP_
#define SUBNET_HPP_

#include <iostream>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <memory>
#include <chrono>

struct Subnet {
    uint32_t netAddress;
    uint32_t mask;
    uint32_t broadcastAddress;
    bool isValid;
    std::vector<std::unique_ptr<Subnet>> subnets;


    Subnet();
    Subnet(uint32_t address, uint32_t mask);
    ~Subnet();
};


void printSubnet(const Subnet &subnet);
bool compareSubnets(const std::unique_ptr<Subnet> &a, const std::unique_ptr<Subnet> &b);
bool add(std::unique_ptr<Subnet> &root, uint32_t &address, uint32_t &mask);
bool del(std::unique_ptr<Subnet> &root, uint32_t &address, uint32_t &mask);
bool check(const std::unique_ptr<Subnet> &root, uint32_t &address);
bool check2(const std::unique_ptr<Subnet> &root, uint32_t &address);
bool bench(const std::unique_ptr<Subnet> &root, int &counter, std::vector<uint32_t> &addresses);
bool isSubnet(const Subnet &a, const Subnet &b);
void addressMaskToString(uint32_t &address, uint32_t &mask);
void dump(const std::unique_ptr<Subnet> &root);


#endif //SUBNET_HPP_