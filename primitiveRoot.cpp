#include <iostream>
#include <vector>

using namespace std;

bool is_primitive(int base, int mod){
    int exp=2;
    int r = base * base;
    for(; r!=base; exp++){
        r *= base;
        r %= mod;
    }
    return exp == mod;
}

vector<int> primitive_root(int mod){
    vector<int> v;
    for(int base=2; base<mod; base++){
        if(is_primitive(base, mod)){
            v.push_back(base);
        }
    }
    return v;
}

int main() {
    for(const int& i: primitive_root(29)){
        cout << i + "," << endl; //?
    }
    return 0;
}