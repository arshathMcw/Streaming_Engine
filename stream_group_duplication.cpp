#include <iostream>
#include <c7x.h>
#include <c7x_scalable.h>

using namespace std;
using namespace c7x;

#define ARR_LENGTH (20)

int main(){
    int32_t input[ARR_LENGTH];
    int32_t *ptrInput = (int32_t *)&input[0];
    for(int32_t idx = 0;idx < ARR_LENGTH;idx++){
        ptrInput[idx] = (idx + 1) * 10;
        cout<<ptrInput[idx]<<" ";
    }
    cout<<endl;
    __SE_TEMPLATE_v1 seTemplate = __gen_SE_TEMPLATE_v1();
    seTemplate.ELETYPE   = se_eletype<int_vec>::value;
    seTemplate.GRPDUP = __SE_GRPDUP_ON; // Turn on the group duplications
    seTemplate.VECLEN    = __SE_VECLEN_2ELEMS;
    seTemplate.DIMFMT = __SE_DIMFMT_1D;
    seTemplate.ICNT0 = ARR_LENGTH;
    seTemplate.ICNT1 = 1;  seTemplate.DIM1 = 0;
    seTemplate.ICNT2 = 1;  seTemplate.DIM2 = 0;
    __SE0_OPEN((void *)&input[0], seTemplate);
    for(int32_t ctr = 0; ctr < ARR_LENGTH/2; ctr++) {
        int_vec vIn = strm_eng<0, int_vec>::get_adv();
        printf("vIn[%d] = ", ctr);
        vIn.print();
    }
    __SE0_CLOSE();
}

