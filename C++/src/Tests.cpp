#include "Tests.hpp"

void Tests::createSubnetTest(uint32_t a, uint32_t m){
    printSubnet(Subnet());
    printSubnet(Subnet(a, m));
}