#include <iostream>
#include <gmpxx.h>

using namespace std;

struct Point
{
    mpz_class x;
    mpz_class y;
};

struct EC
{
    mpz_class a;
    mpz_class b;
    mpz_class p;
};

struct EC secp256k1;
struct Point G;

void EC_add(EC E, Point *R, Point P, Point Q);
mpz_class mod_inv(mpz_class c, mpz_class mod);
int cmp(mpz_class x, mpz_class y);
mpz_class mod(mpz_class n, mpz_class p){
    mpz_class r;
    mpz_mod(r.get_mpz_t(), n.get_mpz_t(), p.get_mpz_t()); // r = n mod p
    return r;
}

void EC_add(EC E, Point *R, Point P, Point Q){
    mpz_class r;
    mpz_class xtemp, ytemp;
    mpz_class Rx, Ry;

    xtemp = Q.x - P.x;
    xtemp = mod(xtemp, E.p);
    xtemp = mod_inv(xtemp, E.p);
    ytemp = Q.y - P.y;
    ytemp = mod(ytemp, E.p);
    r = xtemp * ytemp;
    r = mod(r, E.p);

    Rx = (r*r - P.x - Q.x);
    Rx = mod(Rx, E.p);
    R -> x = Rx;

    Ry = r*(P.y - Rx) - P.y;
    Ry = mod(Ry, E.p);
    R -> y = Ry;
}

void EC_double(EC E, Point *R, Point P, Point Q){
    mpz_class r;
    mpz_class xtemp, ytemp;
    mpz_class Rx, Ry;

    xtemp = P.y;
    xtemp = mod(xtemp, E.p);
    xtemp = mod_inv(xtemp, E.p);
    ytemp = 3*P.x*p.x + E.a;
    ytemp = mod(ytemp, E.p);
    r = ytemp * ytemp;
    r = mod(r, E.p);

    Rx = r*r - (2*P.x);
    Rx = mod(Rx, E.p);
    R -> x = Rx;

    Ry = r*(P.x - Rx) - P.y;
    Ry = mod(Ry, E.p);
    R -> y = Ry;
}

void bits_inv(vector<mpz_class> *bits, int r){
    for(; r>0; r /= 2){ //2로 나눈 몫이 r로 업데이트
            (*bits).push_back(r % 2);
    }
} //for문 돌면서 벡터에 저장하므로 원래 비트의 역순으로 저장됨

void EC_mult(EC E, Point *R, POint P, mpz_class r) {
    vector<mpz_class> bits;
    mpz_class n;

    bits_inv(&bits, r);
    n = bits.size();

    *R=P;

    for(; n>1; n--){
        EC_double(E, R, *R);
        if (bits.at(n-2)==1)
            EC_add(E, R, *R, P);
    }
}

int main() {
    Point A, B, C;
    Point R;
    EC E;

    //A=G
    A = {mpz_class{"0x9BE667EF9DCBBAC55A06295CE870B07029BFCDB2DCE28D959F2815B16F81798"},
        mpz_class{"0x483ADA7726A3C4655DA4FBFC0E1108A8FD17B448A68554199C47D08FFB10D4B8"}}
    //B=2G
    B = {mpz_class{""}}

    EC_add(secp256k1, &R, A, B);
    EC_double(secp256k1, &R, A, B);
    if((cmp(R.x, C.x)==0) && (cmp(R.y, C.y)==0))
}



