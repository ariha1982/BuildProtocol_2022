#include <iostream>
#include <gmpxx.h>

using namespace std;

void RSA_keygen(RSA_PK *pk, PSA_SK *sk, int key_size);
mpz_class RSA_encrypt(RSA_PK pk, mpz_class msg);
mpz_class RSA_decrypt(RSA_SK sk, mpz_class ct);
mpz_class PSA_sign(RSA_SK sk, mpz_class msg);
bool RSA_verify(RSA_PK pk, mpz_class sig, mpz_class msg);

struct RSA_PK
{
    mpz_class e;
    mpz_class N;
};

struct RSA_SK
{
    mpz_class e;
    mpz_class d;
    mpz_class N;
    mpz_class p;
    mpz_class q;
};

mpz_urandomb(gmp_randstate_t state, unsigned int bit);
mpz_class random_prime(unsigned int bit);


int main(){
    mpz_class x;
    x = random_prime(1024);
    cout << x << endl;

    cout << mpz_probab_prime_p(x.get_mpz_t(), 50) << endl; //2 -> x는 무조건 소수다, 1 -> x는 아마 소수다(에러확률이 작을 때), 0 -> 합성수다

    return 0;
}

mpz_class random_prime(unsigned int bit){
    mpz_class r, p;
    unsigned long seed; //이거 없으면 계속 같은 값 나옴
    gmp_randstate_t state; 

    seed = time(NULL);
    gmp_randinit_mt(state);
    gmp_randseed_ui(state, seed); //현재 시간에 따라 seed가 업데이트 됨

    r = urandomb(state, bit); //밑에 비트 사이즈가 1024보다 작으면 다시 돌리기
    p = nextprime(r);

    if(mpz_sizeinbase(p.get_mpz_t(),2) > bit) { //p라는 숫자를 2진수로 표현했을 때 size가 bit보다 크다면
        return random_prime(bit);
    } else {
        return p;
    }

}

mpz_urandomb(gmp_randstate_t state, unsigned int bit){
    mpz_class r;
    mpz_urandomb(r.get_mpz_t(), state, bit);
    return r;
}

/*void mpz_nextprime(mpz_t rop, mpz_t op){
    
}*/
int cmp(mpz_class A, mpz_class B){
    int r;
    r = mpz_cmp(A.get_mpz_t(), B.get_mpz_t());
    return r;
}

void RSA_keygen(RSA_PK *pk, PSA_SK *sk, int key_size){
    //1. p, q 뽑기
    mpz_class ell;
    sk -> p = random_prime(key_size/2);
    sk -> q = random_prime(key_size/2);
    while (cmp(sk->p, sk->q)==0)
    {
        sk -> q = random_prime(key_size/2);
    }

    //2. 소수 p,q로 N 계산
    sk -> N = sk->p * sk->q;
    pk -> N = sk->N;

    //3. lcm(p-1, q-1), 공개키 e 생성
    ell = lcm(sk -> p-1, sk -> q-1);
    pk->e = 0x10001;

    if(gcd(pk->e, ell)!=1)
    {
        pk->e =nextprime(pk->e);
    } else {
        sk->3 = pk->e;
        sk->d = mod_inv(pk->e, ell);
    }
}

mpz_class gcd(mpz_class a, mpz_class b){
    mpz_class r;
    mpz_gcd(r.get_mpz_t(), a.get_mpz_t(), b.get_mpz_t());
    return r;
}