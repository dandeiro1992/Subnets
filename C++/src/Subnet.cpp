#include "Subnet.hpp"

Subnet::Subnet(){
    netAddress          = 0x00000000;
    mask                = 0x00000000;
    broadcastAddress    = 0xffffffff;
    isValid             = false;
}

Subnet::Subnet(uint32_t a, uint32_t m){
    mask                = m;
    netAddress          = a & m;
    broadcastAddress    = netAddress ^ (~m);
    isValid             = true;
}

Subnet::~Subnet(){

}

bool isSubnet(const Subnet &a, const Subnet &b){ //is a subnet of b
    if( (a.netAddress>=b.netAddress) && (a.broadcastAddress<=b.broadcastAddress) ){
        return true;
    }
    else{
        return false;
    }
}
bool isEqual(const Subnet &a, const Subnet &b){ //is a = b
    if( (a.netAddress == b.netAddress) && (a.broadcastAddress == b.broadcastAddress) ){
        return true;
    }
    else{
        return false;
    }
}

bool isAdded(std::unique_ptr<Subnet> &node, std::unique_ptr<Subnet> &s){
    if ( isSubnet(*s,*node) ){
        node->subnets.emplace_back(std::forward<std::unique_ptr<Subnet>>(s));
        return true;
    }
    else{
        return false;
    }
}

bool isAddressOf(uint32_t &address, const Subnet *&node){
    return ( ( ( address >= node->netAddress ) && ( address <= node->broadcastAddress) ) ? true : false );
}
void printSubnet(const Subnet &subnet){
    std::cout<<"netAddress:\t\t"<<std::setw(8)<<std::setfill('0')<<std::hex<<subnet.netAddress<<"\n";
    std::cout<<"mask:\t\t\t"<<std::setw(8)<<std::setfill('0')<<std::hex<<subnet.mask<<"\n";
    std::cout<<"broadcastAddress:\t"<<std::setw(8)<<std::setfill('0')<<std::hex<<subnet.broadcastAddress<<"\n";
    std::cout<<"isValid:\t\t"<<subnet.isValid<<"\n";
}

bool compareSubnets(const std::unique_ptr<Subnet> &a, const std::unique_ptr<Subnet> &b){
    return (a->netAddress <= b->netAddress);
}

void addressMaskToString(uint32_t &address, uint32_t &mask){
    std::string prefix {""};
    for ( int i=0; i<3; i++){
        prefix += std::to_string(((address & (0xFF000000>>i*8))>>(24-i*8)));
        prefix += ".";
    }
    prefix += std::to_string(address & 0x000000FF);
    prefix += "/";
    int counter {0};
    for (int i=0; i<32; i++){
        if (mask & (0x80000000 >> i)){
            counter++;
        }
    }
    prefix += std::to_string(counter);
    std::cout<<prefix;
}
bool add(std::unique_ptr<Subnet> &root, uint32_t &address, uint32_t &mask){
    if (not root){
        root = std::make_unique<Subnet>();
    }
    std::unique_ptr<Subnet> node = std::make_unique<Subnet>(address,mask);

    if( isEqual(*root, *node) ){
        if ( root->isValid ){
            std::cout<<"podana siec juz istnieje\n";
            return false;
        }
        root->isValid = true;
        return true;
    }
    else{
        Subnet *current = root.get();
        std::vector<std::unique_ptr<Subnet>>::iterator it;
        std::vector<std::unique_ptr<Subnet>>::iterator itEqual;
        while ( true ){
            // if ( (isEqual(*node, *current)) ){ // jeśli jest taki sam to nie można dodać
            //     std::cout<<"podana siec juz istnieje\n";
            //     return false;
            // }
            itEqual = std::find_if(current->subnets.begin(), current->subnets.end(), [&node](std::unique_ptr<Subnet> &s){return isEqual(*node, *s);});
            if( itEqual != current->subnets.cend() ){ // jeśli jest taki sam jak któras z podsieci
                std::cout<<"podana siec juz istnieje\n";
                return false;
            } 
            it = std::find_if(current->subnets.begin(), current->subnets.end(), [&node](std::unique_ptr<Subnet> &s){return isSubnet(*node, *s);});
            if( it != current->subnets.cend()){ //jeśli znaleziono podsieć to tam idziemy
                current = it->get();
            }
            else { // jeśli nie to :
                    std::for_each(current->subnets.begin(), current->subnets.end(), 
                    [&](std::unique_ptr<Subnet> &s){return isAdded(node, s);});
                    std::sort(node->subnets.begin(), node->subnets.end(), compareSubnets);
                    current->subnets.emplace_back(std::forward<std::unique_ptr<Subnet>>(node));
                    current->subnets.erase(std::remove_if(current->subnets.begin(), current->subnets.end(), 
                        [](std::unique_ptr<Subnet> &s){return (s == nullptr);}), current->subnets.end() );
                    std::sort(current->subnets.begin(), current->subnets.end(), compareSubnets);
                    current->subnets.shrink_to_fit();
                    return true;
                }
            }
    }
    //return false;
}

bool del(std::unique_ptr<Subnet> &root, uint32_t &address, uint32_t &mask){
    if (not root){
        std::cout<<"Struktura pusta, najpierw coś dodaj\n";
        return false;
    }
    std::unique_ptr<Subnet> node = std::make_unique<Subnet>(address,mask);

    if( isEqual(*root, *node) ){        // jeśli usuwamy root (0.0.0.0/0)
        if ( root->isValid ){
            root->isValid = false;
            return true;
        }
        return false;
    }
    else{
        Subnet *current = root.get();
        std::vector<std::unique_ptr<Subnet>>::iterator it;
        std::vector<std::unique_ptr<Subnet>>::iterator itEqual;
        while ( true ){
            itEqual = std::find_if(current->subnets.begin(), current->subnets.end(), [&](std::unique_ptr<Subnet> &s){return isEqual(*node, *s);});
            if ( itEqual != current->subnets.cend() ){ // jeśli znalazł jakąś równość to przesuwa po
                current->subnets.insert(current->subnets.end(), std::make_move_iterator((*itEqual)->subnets.begin()), std::make_move_iterator((*itEqual)->subnets.end()) );
                //current->subnets.erase(itEqual);
                current->subnets.erase(std::remove_if(current->subnets.begin(), current->subnets.end(),  //usuwanie elementu unique_ptr(ustawianie go na nullptr)
                    [&](std::unique_ptr<Subnet> &s){return isEqual(*node, *s);}), current->subnets.end() );

                current->subnets.erase(std::remove_if(current->subnets.begin(), current->subnets.end(), 
                    [](std::unique_ptr<Subnet> &s){return (s == nullptr);}), current->subnets.end() ); //czyszczenie
                std::sort(current->subnets.begin(), current->subnets.end(), compareSubnets);
                return true;
            }
            else {
                it = std::find_if(current->subnets.begin(), current->subnets.end(), [&](std::unique_ptr<Subnet> &s){return isSubnet(*node, *s);});
                if( it != current->subnets.cend()){ //jeśli znaleziono podsieć to tam idziemy
                current = it->get();
                }
                else {
                    return false;
                }
            }
            }
    }
    return false;
}

bool check(const std::unique_ptr<Subnet> &root, uint32_t &address){
    if (not root){
        return false;
    }
    Subnet *current = root.get();
    while ( true ){
        if ( (address >=current->netAddress) && (address <=current->broadcastAddress) ){
            std::vector<std::unique_ptr<Subnet>>::iterator it;
            it = std::find_if(current->subnets.begin(), current->subnets.end(), [&address](std::unique_ptr<Subnet> &s){
                                                                                return (( ( address >= s->netAddress ) && ( address <= s->broadcastAddress ) ) ? true : false); 
                                                                                });
            if ( it!= current->subnets.cend() ){
                current = it->get();
            }
            else {
                return (current->isValid) ? true : false;
            }
        }
    }
    return false;
}

bool check2(const std::unique_ptr<Subnet> &root, uint32_t &address){
    if (not root){
        return false;
    }
    Subnet *current = root.get();
    while ( true ){
        if ( (address >=current->netAddress) && (address <=current->broadcastAddress) ){
            std::vector<std::unique_ptr<Subnet>>::iterator it;
            it = std::find_if(current->subnets.begin(), current->subnets.end(), [&address](std::unique_ptr<Subnet> &s){
                                                                                return (( ( address >= s->netAddress ) && ( address <= s->broadcastAddress ) ) ? true : false); 
                                                                                });
            if ( it!= current->subnets.cend() ){
                current = it->get();
            }
            else {
                if( current->isValid ){
                    addressMaskToString(current->netAddress, current->mask);
                    return true;
                }
                else{
                    return false;
                }
                //return (current->isValid) ? true : false;
            }
        }
    }
    return false;
}

bool bench(const std::unique_ptr<Subnet> &root, int &counter, std::vector<uint32_t> &addresses){
    size_t addressesSize {addresses.size()};
    auto start_time = std::chrono::high_resolution_clock::now();
    for (int i=0; i<counter;i++){
        check(root, addresses[counter % addressesSize]);
    }
    auto end_time = std::chrono::high_resolution_clock::now();
    auto time = end_time - start_time;
    std::cout<<"time "<<time/std::chrono::milliseconds(1)<<"\n";
    return true;
}

void dump(const std::unique_ptr<Subnet> &root){
    if ( ! not root ){
        printSubnet(*root);
        for(auto &v : root->subnets){
            printSubnet(*v);
        }
    }
    
}