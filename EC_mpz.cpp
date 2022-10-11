#include <iostream>
#include <gmpxx.h>
#include <vector>

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

mpz_class mod(mpz_class n, mpz_class p);
void EC_add(EC E, Point *R, Point P, Point Q);
void EC_double(EC E, Point *R, Point P);
void bits_inv(vector<mpz_class> *bits, int r);
void EC_mult(EC E, Point *R, POint P, mpz_class r);
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

void EC_double(EC E, Point *R, Point P){
    mpz_class r;
    mpz_class xtemp, ytemp;
    mpz_class Rx, Ry;

    ytemp = 2 * P.y;
    ytemp = mod(ytemp, E.p);
    ytemp = mod_inv(ytemp, E.p);
    xtemp = (3*P.x*p.x) + E.a;
    xtemp = mod(xtemp, E.p);
    r = xtemp * ytemp;
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

void EC_mult(EC E, Point *R, Point P, mpz_class r) {
    vector<mpz_class> bits;
    mpz_class n;

    bits_inv(&bits, r);
    n = bits.size();

    *R=P; //R->x = P.x; R->y = P.y;

    for(; n>1; n--){
        EC_double(E, R, *R);
        if (bits.at(n-2)==1)
            EC_add(E, R, *R, P);
    }
}

mpz_class mod_inv(mpz_class x, mpz_class mod){
    mpz_class r;
    if(mpz_invert(r.get_mpz_t(), x.get_mpz_t(), mod.get_mpz_t())==0)
        cout << "error: an inverse doesn't exsist" << endl
    return r;
}

int cmp(mpz_class x, mpz_class y){
    int r;
    r = mpz_cmp(x.get_mpz_t(), y.get_mpz_t());
    return r;
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
    if((cmp(R.x, C.x)==0) && (cmp(R.y, C.y)==0))
        cout << "Success" << endl;
    else
        cout << "Fail" << endl;

    EC_mult(secp256k1, &R, A, 3);
    if((cmp(R.x, C.x)==0) && (cmp(R.y, C.y)==0))
        cout << "Success" << endl;
    else
        cout << "Fail" << endl;
    
}



