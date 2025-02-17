#include <iostream>
#include <c7x.h>
#include <c7x_scalable.h>

using namespace std;
using namespace c7x; 

#define LENGTH (64)

int main(){
    int16_t input[LENGTH];
    int16_t *ptrInput = (int16_t *)&input[0];
    for(int32_t idx = 0;idx < LENGTH;idx++){
        ptrInput[idx] = idx * 10;
        cout<<ptrInput[idx]<<" ";
    }  
    cout<<endl;
    __SE_TEMPLATE_v1 seTemplate = __gen_SE_TEMPLATE_v1();
    seTemplate.ELETYPE = se_eletype<short_vec>::value;
    seTemplate.VECLEN  = se_veclen<int_vec>::value;
    seTemplate.DECIM   = __SE_DECIM_2;
    seTemplate.PROMOTE = __SE_PROMOTE_2X_SIGNEXT;
    seTemplate.DIMFMT = __SE_DIMFMT_1D;
    seTemplate.ICNT0 = LENGTH;
    __SE0_OPEN((void *)&input[0], seTemplate);
    int32_t numItrs = std::ceil(LENGTH / (16.0 * 2));
    for(int32_t ctr = 0; ctr < numItrs; ctr++) {
        int_vec vIn = strm_eng<0, int_vec>::get_adv();
        printf("vIn[%d] = ", ctr);
        vIn.print();
    }
    __SE0_CLOSE();
}