#include <stdio.h>
#include <gmpxx.h>
#include <aes.h>

void H_doub(unsigned char *p);
template<class It>void int2bnd(int n, It begin, It end);
void AES_GCM_encrypt(unsigned char *ct, unsigned char *pmsg, int len, unsigned char *iv, unsigned char *key);
void AES_GCM_decrypt(unsigned char *pmsg, unsigned char *ct, int len, unsigned char *iv, unsigned char *key);

int main()
{
    unsigned char X[16] = {0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                             0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
    H_doub(X);

    for(int i=0; i<16; i++)
        printf("0x%02X ", X[i]);
    printf("\n");

    return 0;
}

void H_doub(unsigned char *p)
{
    bool right_most_bit = p[15] & 1;
    for(int i=15; i>0; i--)
        p[i] = (p[i] >> 1) | (p[i-1] << 7); 
    p[0] >>= 1;
    if(right_most_bit)
        p[0]^0b11100001; //이 뒤는 0이랑 xor하는거니까 하나마나이므로 연산이 필요한 0번째 블록만 연산
}

template<class It>void int2bnd(int n, It begin, It end)
{
    for(It i=end;  i!=begin; n/=0x100)
        *--i = mpz_class{n%100}.get_ui();
}

void AES_GCM_encrypt(unsigned char *ct, unsigned char *pmsg, int len, unsigned char *iv, unsigned char *key)
{
    unsigned char ivNctr[16];
    memcpy(ivNctr, iv, 12);

    for(int i=0; i<len; i+=16){ //한 블록이 16byte 이므로
        int2bnd(i/16 + 1, ivNctr+12, ivNctr+16);
        AES_encrypt(ivNctr, key, ct+i); //0~15저장 --> 16~29 저장 이런식
    }

    for(int i=0; i<len; i++) //xor이 블록 단위로 되지 않으므로 쫙 연결된 상태에서 한 비트씩 xor
    {
        ct[i] ^= pmsg[i];
    }
}

void AES_GCM_decrypt(unsigned char *pmsg, unsigned char *ct, int len, unsigned char *iv, unsigned char *key)
{
    unsigned char ivNctr[16];
    memcpy(ivNctr, iv, 12);

    for(int i=0; i<len; i+=16){ 
        int2bnd(i/16 + 1, ivNctr+12, ivNctr+16);
        AES_encrypt(ivNctr, key, pmsg+i);
    }

    for(int i=0; i<len; i++)
    {
        pmsg[i] ^= ct[i];
    }
}