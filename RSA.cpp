#include <iostream>
#include <gmpxx.h>

using namespace std;

void RSA_keygen(RSA_PK *pk, PSA_SK *sk, int key_size);
mpz_class RSA_encrypt(RSA_PK pk, mpz_class msg);
mpz_class RSA_decrypt(RSA_SK sk, mpz_class ct);
mpz_class RSA_sign(RSA_SK sk, mpz_class msg);
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
    bool rop;
    mpz_class msg, sig;
    struct RSA_PK pk;
    struct RSA_SK sk;

    msg = mpz_class{""}
    
    RSA_keygen(&pk, &sk, 2048); //이후 코드 입력 필요
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

mpz_class urandomb(gmp_randstate_t state, unsigned int bit){
    mpz_class r;
    mpz_urandomb(r.get_mpz_t(), state, bit);
    return r;
}

mpz_class nextprime(mpz_class n){
    mpz_class r;
    mpz_nextprime(r.get_mpz_t(), n.get_mpz_t());
    return r;
}

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
    pk->e = 0x10001; //연산을 빠르게 하기 위해 가운데가 0인 값을 사용

    if(gcd(pk->e, ell)!=1)
    {
        pk->e =nextprime(pk->e);
    }
    sk->e = pk->e;

    //4. 개인키 d 계산
    sk->d = mod_inv(pk->e, ell);
}

mpz_class gcd(mpz_class a, mpz_class b){
    mpz_class r;
    mpz_gcd(r.get_mpz_t(), a.get_mpz_t(), b.get_mpz_t());
    return r;
}

mpz_class mod_inv(mpz_class x, mpz_class mod){
    mpz_class r;
    if(mpz_invert(r.get_mpz_t(), x.get_mpz_t(), mod.get_mpz_t())==0)
        cout << "error: an inverse doesn't exsist" << endl
    return r;
}

mpz_class RSA_encrypt(RSA_PK pk, mpz_class msg){ // ct = m^e mod N
    mpz_class ct;
    ct = powm(msg, pk.e, pk.N);
    return ct;
}

mpz_class RSA_decrypt(RSA_SK sk, mpz_class ct){ // m = ct^d mod N
    mpz_class msg;
    msg = powm(ct, sk.d, sk.N);
    return msg;
}

mpz_class powm (mpz_class base, mpz_class exp, mpz_class mod){
    mpz_class r;
    mpz_powm(r.get_mpz_t(), base.get_mpz_t(), exp.get_mpz_t(), mod.get_mpz_t());
    return r;
}

mpz_class RSA_sign(RSA_SK sk, mpz_class msg){ //Sign = Msg ^ d mod N
    mpz_class sig;
    sig = powm(msg, sk.d, sk.N); //. 말고 ->는 안 되나?
    return sig;
}

bool RSA_verify(RSA_PK pk, mpz_class sig, mpz_class msg){ //M' = Sign ^ e mod N -> M' == M?
    mpz_class mTest;
    mTest = powm(sig, pk.e, pk.N);
    if(cmp(mTest, msg)==0){
        return true;
    } else {
        return false;
    }
}

mpz_class RSA_decrypt_CRT(RSA_SK sk, mpz_class ct){
    mpz_class m1, m2, invM1, invM2, m;

    m1 = powm(ct, sk.d, sk.p);
    m2 = powm(ct, sk.d, sk.q);

    invM1 = mod_inv(sk.q, sk.p);
    invM2 = mod_inv(sk.p, sk.q);

    m = m1*sk.q*invM1;
    m = mod(m, sk.N);
    m += m2*sk.p.invM2;
    m = mod(m, sk.N);

    return m;
}