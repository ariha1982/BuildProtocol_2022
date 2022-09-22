#include <gmpxx.h>

using namespace std;

mpz_class powm (mpz_class base, mpz_class exp, mpz_class mod){
    mpz_class r;
    mpz_powm(r.get_mpz_t(), base.get_mpz_t(), exp.get_mpz_t(), mod.get_mpz_t());
    return r;
}