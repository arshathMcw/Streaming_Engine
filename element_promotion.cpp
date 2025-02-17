#include <iostream>
#include <c7x.h>
#include <c7x_scalable.h>
using namespace std;
using namespace c7x;
#define ARRAY_LENGTH (20)
int main(){
    int16_t input[ARRAY_LENGTH];
    int16_t *ptrInput = (int16_t *)&input[0];
    for(int16_t idx = 0; idx < ARRAY_LENGTH; idx++) {
        ptrInput[idx] = idx * -1;
    }
    for(int16_t idx = 0; idx < ARRAY_LENGTH; idx++) {
        cout<<ptrInput[idx]<<" ";
    }
    cout<<endl;
    __SE_TEMPLATE_v1 seTemplate = __gen_SE_TEMPLATE_v1();
    seTemplate.ELETYPE   = se_eletype<short_vec>::value;
    seTemplate.PROMOTE   = __SE_PROMOTE_2X_SIGNEXT;
    seTemplate.VECLEN    = se_veclen<int_vec>::value;
    seTemplate.DIMFMT = __SE_DIMFMT_1D;
    seTemplate.ICNT0 = ARRAY_LENGTH;
    __SE0_OPEN((void *)&input[0], seTemplate);
    int32_t numItrs = ceil(ARRAY_LENGTH / 16.0);

    for(int32_t ctr = 0; ctr < numItrs; ctr++) {
        int_vec vIn = strm_eng<0, int_vec>::get_adv();
        cout<<"vIn["<<ctr<<"] = ";
        vIn.print();
    }
    __SE0_CLOSE();
}
