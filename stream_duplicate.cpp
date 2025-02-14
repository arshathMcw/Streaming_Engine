#include <iostream>
#include <c7x.h>
#include <c7x_scalable.h>

using namespace std;
using namespace c7x;

#define ARRAY_LENGTH (20)

int main(){
    int32_t input[ARRAY_LENGTH];
    int32_t *ptrInput = (int32_t *)&input[0];
    for(int32_t idx = 0;idx < ARRAY_LENGTH;idx++){
        ptrInput[idx] = (idx + 1) * 10; 
        cout<<ptrInput[idx]<<" ";
    }
    cout<<endl;
    __SE_TEMPLATE_v1 seTemplate = __gen_SE_TEMPLATE_v1();
    seTemplate.ELETYPE   = se_eletype<int_vec>::value;
    seTemplate.VECLEN    = se_veclen<int_vec>::value;
    seTemplate.ELEDUP    = __SE_ELEDUP_16X;
    seTemplate.DIMFMT = __SE_DIMFMT_1D;
    seTemplate.ICNT0 = ARRAY_LENGTH;
    __SE0_OPEN((void *)&input[0], seTemplate);
    for(int32_t ctr = 0; ctr < ARRAY_LENGTH; ctr++) {
        int_vec vIn = strm_eng<0, int_vec>::get_adv();
        printf("vIn[%d] = ", ctr);
        vIn.print();
    }
    __SE0_CLOSE();

}