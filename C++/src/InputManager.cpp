#include "InputManager.hpp"

using myVector = std::vector<std::array<uint32_t,2>>;
using tp = std::chrono::time_point<std::chrono::system_clock>;
using duration = std::chrono::duration<float>;

template<typename T>
T random(T range_from, T range_to) {
    std::random_device rand_dev;
    std::mt19937 generator(rand_dev());
    std::uniform_int_distribution<T> distr(range_from, range_to);
    return distr(generator);
}

InputManager::InputManager(){numberOfNodes = 0;};


void InputManager::Test(){
    int benchTestLoopCounter {100000000};
    myVector testVectorAdd(2000000,{1,1});
    myVector testVectorDel(2000000,{1,1});
    std::vector<uint32_t> testVectorBench;
    uint32_t rangeFrom {0};
    uint32_t rangeToAddress {4294967295};
    uint32_t rangeToMask {32};
    tp start = std::chrono::system_clock::now(); // Checking how much time prepating data takes
    for(auto &a : testVectorAdd){
        a[0] = random(rangeFrom, rangeToAddress);
        a[1] = maskUintToUint(random(rangeFrom, rangeToMask));
    }
    for(auto &a : testVectorDel){
        a[0] = random(rangeFrom, rangeToAddress);
        a[1] = maskUintToUint(random(rangeFrom, rangeToMask));
    }

    for (int i=0; i<1000; i++){
        testVectorBench.emplace_back(random(rangeFrom, rangeToAddress));
    }

    duration d = std::chrono::system_clock::now() - start;
    std::cout << "preparing data takes:\t"<< d.count()<<"\n";

    add(root, testVectorAdd[0][0], testVectorAdd[0][1]);
    numberOfNodes++;
     std::cout<<"Performing bench for\t"<<numberOfNodes<<" nodes\n";
    bench(root, benchTestLoopCounter, testVectorBench);

    for (int i=0; i < 1000000; i+=100000){
        start = std::chrono::system_clock::now();
        for (int j=0; j<100000; j++){           //Adding and deleting 100'000 subnets
            if (add(root, testVectorAdd[i+j][0],testVectorAdd[i+j][1])){
                numberOfNodes++;
            }
            if (del(root,testVectorDel[i+j][0],testVectorDel[i+j][1])){
                numberOfNodes--;
            }
        }
        d = std::chrono::system_clock::now() - start;
        std::cout << "Adding and deleting 100000 nodes:\t"<< d.count()<<"\n";
        // when adding/deleting done - performing bench
        std::cout<<"Performing bench for\t"<<numberOfNodes<<" nodes\n";
        bench(root, benchTestLoopCounter, testVectorBench);
    }
}
void InputManager::getCommand(){
    std::string command {""};
    std::cout<<choice;
    while( std::cin >> command ){
        std::string arguments{""};
        std::getline(std::cin, arguments);
        arguments.erase(0,1);
        if (std::count(commands.begin(), commands.end(), command)){
            if( command == commands[0]){                //add
                uint32_t address {0};
                uint32_t mask {0};
                if( getAddDelArguments(arguments, address, mask)){
                    if( add(root, address, mask)){
                        std::cout<<addOkNotification;
                        numberOfNodes++;
                    }
                    else {
                        std::cout<<addFailureNotification;
                    }
                }
                else {
                    std::cout<<commands[0]<<" "<<badSyntax;
                }
            } 
            else if (command == commands[1]){               //del
                uint32_t address {0};
                uint32_t mask {0};
                
                if( getAddDelArguments(arguments, address, mask)){
                    if( del(root, address, mask)){
                        numberOfNodes--;
                        std::cout<<delOkNotification;
                    }
                    else {
                        std::cout<<delFailureNotification;
                    }
                }
                else {
                    std::cout<<commands[1]<<" "<<badSyntax;
                }
            }
            else if (command == commands[2]){               //check
                uint32_t address {0};
                if( getCheckArgument(arguments, address) ){
                    if( check2(root, address)){
                        std::cout<<"Check Success\n";
                    }
                    else {;
                        std::cout<<"Check Failure\n";
                    }
                }
                else {;
                    std::cout<<commands[2]<<" "<<badSyntax;
                }
            }
            else if (command == commands[3]){               //bench
                int counter {0};
                std::vector<uint32_t> addresses;
                if( getBenchArguments(arguments, counter, addresses) ){
                    if( bench(root, counter, addresses)){
                        std::cout<<"Success for "<<numberOfNodes<<" \n";
                    }
                    else {
                        std::cout<<"Failure\n";
                    }
                }
                else {;
                    std::cout<<commands[3]<<" "<<badSyntax;
                }
            }
            else if (command == commands[4]){               //bench
                dump(root);
            }
            std::cout<<"\n";
        }
        else {
            std::cout<<badSyntax;
            std::cout<<"\n";
        }
        std::cout<<choice;
    }
}

bool InputManager::getAddDelArguments(std::string &input, uint32_t &a, uint32_t &m){
    if ( validateData(input, commands[0])){
        std::string address = input.substr(0,input.find("/"));
        std::string mask    = input.substr(input.find("/")+1,input.length());
        a = ipStringToUint(address);
        m = maskStringToUint(mask);
        return true;
    }
    else 
        return false;
}

bool InputManager::getCheckArgument(std::string &input, uint32_t &a){
    if ( validateData(input,commands[2]) ){
        a = ipStringToUint(input);
        return true;
    }
    else
        return false;
}

bool InputManager::getBenchArguments(std::string &input, int &counter, std::vector<uint32_t> &addresses){
    std::string number {input.substr(0, input.find(" "))};
    input.erase(0,input.find(" ")+1);
    if (!isInteger(number)){
        return false;
    }
    counter = atoi(number.c_str());
    std::string line{""};

    while( input.find(" ") != std::string::npos ){
        size_t ptr {input.find(" ")};
        line = input.substr(0, ptr);
        if (ptr < input.length()-1){
            input.erase(0,ptr+1);
        }
        else{
            input.erase(0,ptr+1);
        }
        if ( validateData(line, commands[3])){
            addresses.emplace_back(ipStringToUint(line));
        }
        else{
            return false;
        }
    }
    line = input;
    line.erase(std::remove_if(line.begin(), line.end(), ::isspace), line.end());
    if ( validateData(line, commands[3])){
            addresses.emplace_back(ipStringToUint(line));
        }
        else{
            return false;
        }
    return true;
}
bool InputManager::validateData(std::string input, std::string command){
    std::regex addDelRegex("(([0-9]|[1-9][0-9]|1[0-9][0-9]|2[0-4][0-9]|25[0-5])\\.){3}([0-9]|[1-9][0-9]|1[0-9][0-9]|2[0-4][0-9]|25[0-5])(\\/)([0-9]|[0-2][0-9]|3[0-2])");
        std::regex checkBenchRegex("(([0-9]|[1-9][0-9]|1[0-9][0-9]|2[0-4][0-9]|25[0-5])\\.){3}([0-9]|[1-9][0-9]|1[0-9][0-9]|2[0-4][0-9]|25[0-5])");
    if ( (command =="add") || (command == "del")){
        if (std::regex_match(input, addDelRegex)){
            return true;
        }
        else {
            return false;
        }
    }
    if ((command =="check") || (command == "bench")){
        if (std::regex_match(input, checkBenchRegex)){
            return true;
        }
        else {
            return false;
        }
    }
}


void InputManager::run(){
    root->mask += 0xff;
}

uint32_t InputManager::ipStringToUint(std::string a){
    uint32_t ip_address {0};

    for(int i=0;i<4;i++)
    {
        ip_address^=(atoi(a.substr(0,a.find(".")).c_str()))<<(3-i)*8;
        a.erase(0,a.find(".")+1);
    }
    return ip_address;
}

uint32_t InputManager::maskStringToUint(std::string m){
    uint32_t mask {0};
    int counter = atoi(m.c_str());
    for (int i=0; i<counter; i++){
        mask ^= (0x80000000 >> i);
    }
    return mask;
}

uint32_t InputManager::maskUintToUint(uint32_t &&counter){
    uint32_t mask {0};
    for (int i=0; i<counter; i++){
        mask ^= (0x80000000 >> i);
    }
    return mask;
}
bool InputManager::isInteger(std::string &input){
    for (int i=0; i<input.length(); i++){
        if (!std::isdigit(input[i])){
            return false;
        }
    }
    return true;
}
