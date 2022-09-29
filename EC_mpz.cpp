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

mpz_class mod_inv(mpz_class c, mpz_class mod);
int cmp(mpz_class x, mpz_class y);
mpz_class mod(mpz_class n, mpz_class p){
    mpz_class r;
    mpz_mod(r.get_mpz_t(), n.get_mpz_t(), p.get_mpz_t()); // r = n mod p
    return r;
}



