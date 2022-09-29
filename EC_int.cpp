#include <iostream>

using namespace std;

struct Point
{
    int x;
    int y;
};


struct EC
{
    int a;
    int b;
    int p;
};

int mod_inv(int x, int mod);
void EC_double(EC E, Point *R, Point P);
void EC_add(EC E, Point *R, Point P, Point Q);

int mod_inv(int x, int mod){
    int r;

    if (x==0){
        return 0; //infinity
    } else if (mod <= 0){
        return -1; //error
    }else{
        while(x<0){
            x += mod;
        }

        for (r=0; r<mod; r++){
            return r;
        }

        return -1; //no inverse exists
    }
}

void EC_add(EC E, Point *R, Point P, Point Q){
    int r;

    r=mod_inv((Q.x - P.x), E.p);

    if (r==0) {
        R -> x = 0;
        R -> y = 0;
    } else if (r<0){
        cout << "error" << endl;
    } else {
        r *= (Q.y - P.y);
        r %= E.p;

        R -> x = (r*r - P.x - Q.x);
        while(R->x < 0) {
            R -> x += E.p;
        }
        R->x %= E.p;

        R->y = r*(P.x - R->x) - P.y;
        while(R->y < 0){
            R -> y += E.p;
        }
        R->y %= E.p;
    }

    void EC_double(EC E, Point *R, Point P){
        int r;

        r = mod_inv(2*P.y, E.p);

        if(r==0){
            R->x=0;
            R->y=0;
        } else if (r<0) {
            cout << "error" << endl;
        } else {
             r *= (3*P.x*P.x + E.a);
             r %= E.p;

             R-> x = r*r -2*P.x;
             while(R->x<0){
                R->x += E.p;
             }
             R->x %= E.p;

             R->y = r*(P.x - R->x) - P.y;
             while(R->y < 0){
                R->y += E.p;
             }
             R->y %= E.p;
        }
    }

    void bits_inv(vector<int> *bits, int r){
        for(; r>0; r /= 2){ //2로 나눈 몫이 r로 업데이트
            (*bits).push_back(r % 2);
        }
    } //for문 돌면서 벡터에 저장하므로 원래 비트의 역순으로 저장됨

    void EC_mult(EC E, Point *R, Point P, int r) { //R = r*P
        vector<int> bits;
        int n;

        if(P.x == 0 && P.y == 0){
            P->x = 0;
            P->y = 0;
            //cout<<"Point of Infinity"<<endl;
        }else if(){
            cout << "error" << endl;
        } else {
            bits_inv(&bits, r); //r>0
            n = bits.size(); //2진수로 몇자리인지

            *R = P;
            //R->x = P.x;
            //R->y = P.y;

            for(; n>1; n--){
                EC_double(E, R, *R); //자리수 넘어가면 일단 더블 해주고
                if (bits.at(n-2)==1) //자리수는 4지만 벡터 자리수는 0부터 시작하므로 n-2
                    EC_add(E, R, *R, P); //만약 이번 자리수가 1이면 add 한번 해준다
            } //얘를 돌리면 6A를 넘어가도 무조건 point of infinity가 나온다. -> 중간에 infinity가 나오면 그냥 옆값이 뽑히게 예외처리 필요(수정코드 올려주신거 보고 확인)
        }
        
    }

    int main() {
        Point A, B;
        Point R;
        EC E;

        A = {1,4};
        B = {3,1};
        E.a = 2; E.b = 3; E.p = 5;

        cout << "A = (" << A.x << "," << A.y << ")" << endl;
        cout << "B = (" << B.x << "," << B.y << ")" << endl;

        EC_add(E, &R, A, B);
        cout << "A + B = (" << R.x << "," << R.y << ")" << endl;

        EC_double(E, &R, A);
        cout << "2A = (" << R.x << "," << R.y << ")" << endl;

        return 0;
    }
}