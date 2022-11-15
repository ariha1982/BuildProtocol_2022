#include <stdio.h>
#define BLOCK_SIZE 16

int main()
{
    unsigned char msg[] = "Hello this is a test";
    int len, pad;

    /* padding on */
    // len = sizeof(msg);
    // pad = BLOCK_SIZE - (len % BLOCK_SIZE);
    // unsigned char p[len+pad];

    len = ((sizeof(msg)/16)+1) * BLOCK_SIZE;
    unsigned char pmsg[len];
    pad = 

    for(int i=0; i<sizeof(p); i++){
        p
    }

    int num = pmsg[len-1];
    unsigned char msg2[len-num]; //패딩을 아예 제거한 배열
    for(int i=0; i<(len-num); i++)
    {
        printf("%d ", pmsg[i]);
        printf("\n");
    }

    return 0;
}