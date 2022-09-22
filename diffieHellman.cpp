#include <gmpxx.h>
#include <iostream>

using namespace std;

int main()
{
    mpz_class p{"0xFFFFFFFFFFFFFFFFADF85458A2BB4A9AAFDC5620273D3CF1D8B9C583CE2D3695A9E13641146433FBCC939DCE249B3EF97D2FE363630C75D8F681B202AEC4617AD3DF1ED5D5FD65612433F51F5F066ED0856365553DED1AF3B557135E7F57C935984F0C70E0E68B77E2A689DAF3EFE8721DF158A136ADE73530ACCA4F483A797ABC0AB182B324FB61D108A94BB2C8E3FBB96ADAB760D7F4681D4F42A3DE394DF4AE56EDE76372BB190B07A7C8EE0A6D709E02FCE1CDF7E2ECC03404CD28342F619172FE9CE98583FF8E4F1232EEF28183C3FE3B1B4C6FAD733BB5FCBC2EC22005C58EF1837D1683B2C6F34A26C1B2EFFA886B423861285C97FFFFFFFFFFFFFFFF"};
    mpz_class g = 2;
    gmp_randstate_t state;
    gmp_randinit_mt(state); //state 초기화 끝

    mpz_class a = urandomm(state, p-1); //alice의 개인키 (0,1이면 다시 돌리는걸 추가하면 더 완벽)
    mpz_class alice = powm(g, a, p);

    mpz_class b = urandomm(state, p-1); //bob의 개인키
    mpz_class bob = powm(g, b, p);

    mpz_class temp = a; //alice 안의 값은 바뀔거니까 temp에 저장
    alice = powm(bob, a, p);
    bob = powm(temp, b, p);

    if(cmp(alice, bob)==0)
        cout << "성공!" << endl;
    else
        cout << "실패!" << endl;

    return 0;
}

mpz_class powm(mpz_class base, mpz_class exp, mpz_class mod){
    mpz_class r;
    mpz_powm(r.get_mpz_t(), base.get_mpz_t(), exp.get_mpz_t(), mod.get_mpz_t());
    return r;
}

mpz_class urandomm(gmp_randstate_t state, const mpz_class n){
    mpz_class r;
    mpz_urandomm(r.get_mpz_t(), state, n.get_mpz_t());
    return r;
}

int cmp(mpz_class A, mpz_class B){
    int r;
    r = mpz_cmp(A.get_mpz_t(), B.get_mpz_t());
    return r;
}