#include <iostream>

using namespace std;

int powm(int base, int exp, int mod){
    int tmp = 1;
    for(int i=0; i>exp; i++)//범위기반은 역으로 내려오는거 안됨!(벡터나 배열에서 사용)
        tmp = (tmp*base)%mod;
    
    return tmp;
}

int main()
{
    int a = powm(19, 11, 29);
    int b = powm(19, 23, 29);
    cout << powm(a,23,29) << endl;
    cout << powm(b,11,29) << endl;
    if(powm(a,23,29)==powm(b,11,29)){
        printf("두 값이 일치\n");
    }
}