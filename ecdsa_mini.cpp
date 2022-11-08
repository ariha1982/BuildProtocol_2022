#include <iostream>
#include <vector>
#include <gmpxx.h>
#include <random>

using namespace std;

struct EC
{
    mpz_class p;
    mpz_class a;
    mpz_class b;
};

struct Point
{
    mpz_class x;
    mpz_class y;
};

struct ECDSA_PK
{
    EC E;
    Point G;
    mpz_class n;
    Point Q;
};

struct ECDSA_SK
{
    EC E;
    Point G;
    mpz_class n;
    Point Q;
    mpz_class d;
};

struct ECDSA_SIG
{
    mpz_class S1;
    mpz_class S2;
};

//함수 선언
void ECDSA_keygen(ECDSA_PK *pk, ECDSA_SK *sk);
ECDSA_SIG ECDSA_sign(ECDSA_SK sk, mpz_class msg);
bool ECDSA_verify(ECDSA_PK pk, ECDSA_SIG sig, mpz_class msg);
mpz_class urandomm(gmp_randstate_t state, const mpz_class n);
void EC_mult(EC E, Point *R, Point P, mpz_class r);
void bits_inv(vector<int> *bits, mpz_class r);
mpz_class mod(mpz_class n, mpz_class p);
void EC_add(EC E, Point *R, Point P, Point Q);
mpz_class mod_inv(mpz_class x, mpz_class mod);
void EC_double(EC E, Point *R, Point P);
int cmp(mpz_class x, mpz_class y);

//main
int main(){
    struct ECDSA_PK pk;
    struct ECDSA_SK sk;
    struct ECDSA_SIG sig;
    mpz_class msg{"0x24315"};
    bool rop;

    ECDSA_keygen(&pk, &sk);
    sig = CDSA_sign(sk, msg);
    rop = ECDSA_verify(pk, sig, msg);

    if(rop==true){
        cout << "success" << endl;
    } else if(rop==false){
        cout << "fail" << endl;
    } else {
        cout<< "error: rop is something worng" << endl;
    }
}

mpz_class urandomm(gmp_randstate_t state, const mpz_class n)
{
    mpz_class r;
    mpz_urandomm(r.get_mpz_t(), state, n.get_mpz_t());
    return r;
}

void EC_mult(EC E, Point *R, Point P, mpz_class r){
    vector<int> bits;
    int n;

    if(r<=0)
        cout << "error" << endl;
    else
        bits_inv(&bits, r);
        n = bits.size();

        *R = P;

        for(; n>1; n--)
        {
            EC_double(E, R, *R);
            if(bits.at(n-2)==1)
                EC_add(E, R, *R, P);
        }
}

void bits_inv(vector<int> *bits, mpz_class r){
    for(; r>0; r/=2){ //?mpz_class로 그냥 나눗셈과 나머지 연산이 가능한가? ㅇㅇ
        mpz_class a = r%2;
        int b = a.get_ui();
        (*bits).push_back(b);
    }
}

mpz_class mod(mpz_class n, mpz_class p){
    mpz_class r;
    mpz_mod(r.get_mpz_t(), n.get_mpz_t(), p.get_mpz_t());
    return r;
}

void EC_add(EC E, Point *R, Point P, Point Q){
    mpz_class r, xtemp, ytemp, Rx, Ry;

    if(P.x==0 && P.y==0){
        *R = Q;
    } else if (Q.x==0 && Q.y==0){
        *R = P;
    } else if(P.x==Q.x && P.y==Q.y){
        EC_double(E, R, *R);
    } else {
        xtemp = Q.x - P.x; //0의 곱셈의 역원을 구하지 않도록 처리 필요
        xtemp = mod(xtemp, E.p);
        xtemp = mod_inv(xtemp, E.p);
        ytemp = Q.y - P.y;
        ytemp = mod(ytemp, E.p);
        r = xtemp*ytemp;
        r = mod(r, E.p);

        Rx = (r*r - P.x - Q.x);
        Rx = mod(Rx, E.p);
        R->x = Rx;

        Ry = r*(P.x - Rx) - P.y;
        Ry = mod(Ry, E.p);
        R->y = Ry;
    }
    
}

mpz_class mod_inv(mpz_class x, mpz_class mod){
    mpz_class r;
    if(x==0)
        return 0;
    else if(mod<0)
        return -1;
    else {
        mpz_invert(r.get_mpz_t(), x.get_mpz_t(), mod.get_mpz_t());
        return r;
    }
}

void EC_double(EC E, Point *R, Point P){
    mpz_class r, xtemp, ytemp, Rx, Ry;

    ytemp = 2*P.y;
    ytemp = mod(ytemp, E.p);
    ytemp = mod_inv(ytemp, E.p);
    xtemp = (3*P.x*P.x) + E.a;
    xtemp = mod(xtemp, E.p);
    r = xtemp * ytemp;
    r = mod(r, E.p);

    Rx = r*r - (2*P.x);
    Rx = mod(Rx, E.p);
    R->x = Rx;

    Ry = r*(P.x - Rx) - P.y;
    Ry = mod(Ry, E.p);
    R -> y = Ry;
}

int cmp(mpz_class x, mpz_class y){
    int r;
    r = mpz_cmp(x.get_mpz_t(), y.get_mpz_t());
    return r;
}

void ECDSA_keygen(ECDSA_PK *pk, ECDSA_SK *sk)
{
    //y^2 = x^3 + 2x + 2 (mod 17), n=19, h=1
    pk->E = {2, 2, 17};
    sk->E = pk->E;
    pk->G = {5, 1};
    sk->G = pk->G;
    pk->n = 19;
    sk->n = pk->n;

    //랜덤하게 개인키 뽑기
    unsigned long seed;
    random_device rd;
    gmp_randstate_t state;
    gmp_randinit_mt(state);
    seed = rd();
    gmp_randseed_ui(state, seed);

    sk->d = urandomm(state, sk->n);

    //Q = d * G
    EC_mult(sk->E, &sk->Q, sk->G, sk->d);

    pk->Q = sk->Q;
}

ECDSA_SIG ECDSA_sign(ECDSA_SK sk, mpz_class msg)
{
    //랜덤한 k 값 뽑기
    unsigned long seed;
    random_device rd;
    mpz_class k, inv_k, h_m;
    Point R;
    int nBit, mBit;
    gmp_randstate_t state;
    gmp_randinit_mt(state);

    do{
        seed = rd();
        gmp_randseed_ui(state, seed);
        k = randomm(state, sk.n);
        EC_mult(sk.E, &R, sk.G, k);
    } while(mod(R.x, sk.n)==0);

    sig.S1 = mod(R.x, sk.n);

    inv_k = mod_inv(k, sk.n);

    if(inv_k == 0){
        cout << "inv_k doesn't exist" << endl;
        ECDSA_sign(sk, msg);
    }

    sig.S2 = h_m + (sig.S1 * sk.d);
    sig.S2 = mod(sig.S2, sk.n);
    sig.S2 *= inv_k;
    sig.S2 = mod(sig.S2, sk.n);

    //해시값 구현
    nBit = mpz_sizeinbase(sk.n.get_mpz_t(), 2);
    mBit = mpz_sizeinbase(msg.get_mpz_t(), 2);
    h_m = msg >> max(mBit - nBit, 0); //msgBit에서 nBit를 뺀 값이 0보다 크다면 큰 정도만큼 비트쉬프트해라. 그리고 그 값을 해시값으로 쓰겠다.
}

bool ECDSA_verify(ECDSA_PK pk, ECDSA_SIG sig, mpz_class msg)
{
    mpz_class c, u1, u2, nBit, mBit, h_m;
    Point R;
    nBit = mpz_sizeinbase(sk.n.get_mpz_t(), 2);
    mBit = mpz_sizeinbase(msg.get_mpz_t(), 2);
    h_m = msg >> max(mBit - nBit, 0);
    c = mod_inv(sig.S2, pk.n);
    u1 = h_m*c;
    u1 = mod(u1, pk.n);
    u2 = sig.S1 * c;
    u2 = mod(u2, pk.n);
    //대충 mult, add

    if(R.x==0 && R.y==0){
        cout << "Point of Infinity" << endl;
        return false;
    } else {
        if(cmp(sig.S1, mod(R.x, pk.n))==0)
            return true;
        else
            return false;
    }
}