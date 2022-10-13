#include <iostream>
#include <vector>
#include <gmpxx.h>

using namespace std;

struct EC //y^2 = x^3 + ax + b (mod p)
{
    mpz_class a;
    mpz_class b;
    mpz_class p;
};

struct Point //(x, y)
{
    mpz_class x;
    mpz_class y;
};

struct EC secp256k1 = {0, 7, mpz_class{"1"}};
struct Point G = {mpz_class{"1"}, mpz_class{"1"}};

//함수선언
mpz_class urandomm(gmp_randstate_t state, const mpz_class n);
void EC_mult(EC E, Point *R, Point P, mpz_class r);
void bits_inv(vector<mpz_class> *bits, mpz_class r);
mpz_class mod(mpz_class n, mpz_class p);
void EC_add(EC E, Point *R, Point P, Point Q);
mpz_class mod_inv(mpz_class x, mpz_class mod);
void EC_double(EC E, Point *R, Point P);
int cmp(mpz_class x, mpz_class y);



int main()
{
    mpz_class dA, dB;
    Point A, B, temp; //최종으로 A,B가 갖게 된 값
    mpz_class p = 1;
    mpz_class g = 2;
    gmp_randstate_t state;
    gmp_randinit_mt(state);

    //1. 랜덤으로 개인키 d 뽑기
    dA = urandomm(state, p-1);
    while(dA==0 || dA==1){
        dA = urandomm(state, p-1);
    }
    dB = urandomm(state, p-1);
    while(dB==0 || dB==1){
        dB = urandomm(state, p-1);
    }
    //?alice와 bob의 개인키는 같으면 안되는가?

    //2. Q = d * G
    EC_mult(secp256k1, &A, G, dA);
    EC_mult(secp256k1, &B, G, dB);

    //3. QA, QB 교환(돌아가기만 하면 되니까 딱히 필요 없는 부분)
    temp.x = A.x;
    temp.y = A.y;
    A.x = B.x;
    A.y = B.y;
    B.x = temp.x;
    B.y = temp.x;

    //4. Share = d * Q, 3번이 없으면 A에 dB, B에 dA를 mult
    EC_mult(secp256k1, &A, A, dA);
    EC_mult(secp256k1, &B, B, dB);

    //5. cmp
    if(cmp(A.x, B.x)==0 && cmp(A.y, B.y)==0)
        cout << "Success!" << endl;
    else
        cout << "Fail..." << endl;
}

mpz_class urandomm(gmp_randstate_t state, const mpz_class n)
{
    mpz_class r;
    mpz_urandomm(r.get_mpz_t(), state, n.get_mpz_t());
    return r;
}

void EC_mult(EC E, Point *R, Point P, mpz_class r){
    vector<mpz_class> bits;
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

void bits_inv(vector<mpz_class> *bits, mpz_class r){
    for(; r>0; r/=2){ //?mpz_class로 그냥 나눗셈과 나머지 연산이 가능한가? ㅇㅇ
        (*bits).push_back(r%2);
    }
}

mpz_class mod(mpz_class n, mpz_class p){
    mpz_class r;
    mpz_mod(r.get_mpz_t(), n.get_mpz_t(), p.get_mpz_t());
    return r;
}

mpz_class div(mpz_class a, mpz_class b){ //나눗셈 함수?
    mpz_class r;
    mpz_div(r.get_mpz_t(), a.get_mpz_t(), b.get_mpz_t());
    return r;
}

void EC_add(EC E, Point *R, Point P, Point Q){
    mpz_class r, xtemp, ytemp, Rx, Ry;

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

mpz_class mod_inv(mpz_class x, mpz_class mod){
    mpz_class r;
    mpz_invert(r.get_mpz_t(), x.get_mpz_t(), mod.get_mpz_t());
    return r;
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