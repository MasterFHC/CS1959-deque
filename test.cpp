#include <bits/stdc++.h>
#include "deque.hpp"
// using namespace std;
int main(){
    sjtu::deque<int> d;
    d.push_back(1);
    d.push_back(2);
    d.push_back(3);
    const sjtu::deque<int> dd = d;
    sjtu::deque<int>::const_iterator cit;
    for(cit = dd.begin();cit!=dd.end();cit++){
        // (*cit) = 4;
        std::cout<<(*cit);

    }
    return 0;
}