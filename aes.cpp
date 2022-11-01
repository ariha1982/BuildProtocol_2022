#include <iostream>
#include <gmpxx.h>

using namespace std;

void shift_rows(unsigned char *msg);
void inv_shift_rows(unsigned char *msg);
void sub_bytes(unsigned char *msg);
void inv_sub_bytes(unsigned char *msg);
void mix_columns(unsigned char *msg);
void inv_mix_columns(unsigned char *msg);
void add_round_key(unsigned char *msg);
void print_state(unsigned char *msg);

#define ROUND 10 //라운드 수(AES-128)
#define N 4 //key size in words

int main(){
    unsigned char msg[16] = {0x04, 0x00, 0x12, 0x18,
                            0x02, 0x11, 0x18, 0x0F,
                            0x13, 0x0E, 0x06, 0x11,
                            0x00, 0x0F, 0x07, 0x18};
    
    print_state(msg);

}

void print_state(unsigned char *msg)
{
    for(int i=0; i<N; i++)
    {
        printf("0x%02X ", msg[i]);
        printf("0x%02X ", msg[i+4]);
        printf("0x%02X ", msg[i+8]);
        printf("0x%02X ", msg[i+12]);
        printf("\n");
    }
}