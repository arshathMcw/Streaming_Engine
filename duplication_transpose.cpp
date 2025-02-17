// duplication_transpose.cpp
#include <iostream>
#include <c7x.h>
#include <c7x_scalable.h>

using namespace std;
using namespace c7x; 

#define ARRAY_WIDTH  (16)
#define ARRAY_HEIGHT (16)
#define ARRAY_STRIDE (ARRAY_WIDTH)

int main(){
    int32_t dataArray[ARRAY_STRIDE * ARRAY_HEIGHT];
    int32_t *pData = (int32_t *)&dataArray[0];
    int32_t val = 0;    
    for(int32_t row = 0; row < ARRAY_HEIGHT; row++) {
        for(int32_t col = 0; col < ARRAY_WIDTH; col++) {
            pData[(row * ARRAY_STRIDE) + col] = val++;
        }
    }
    for(int32_t row = 0; row < ARRAY_HEIGHT; row++) {
        for(int32_t col = 0; col < ARRAY_WIDTH; col++) {
            cout<<pData[(row * ARRAY_STRIDE) + col]<<" ";
        }
        cout<<endl;
    }
    __SE_TEMPLATE_v1 seTemplate = __gen_SE_TEMPLATE_v1();
    seTemplate.ELETYPE   = se_eletype<int_vec>::value;
    seTemplate.VECLEN    = se_veclen<int_vec>::value;
    seTemplate.ELEDUP    = __SE_ELEDUP_16X;
    seTemplate.TRANSPOSE = __SE_TRANSPOSE_32BIT;
    const int vec_len = element_count_of<int_vec>::value;
    seTemplate.DIMFMT = __SE_DIMFMT_3D;
    seTemplate.ICNT0 = ARRAY_WIDTH;
    seTemplate.ICNT1 = vec_len;                 
    seTemplate.DIM1 = ARRAY_STRIDE;
    seTemplate.ICNT2 = (ARRAY_HEIGHT/vec_len);  
    seTemplate.DIM2 = (ARRAY_STRIDE * vec_len);
    __SE0_OPEN((void *)&dataArray[0], seTemplate);
    for(int32_t col = 0; col < ARRAY_WIDTH; col++) {
    printf("Col - %d = \n", col);
    for(int32_t ctr = 0; ctr < vec_len; ctr++) {
            int_vec vIn = strm_eng<0, int_vec>::get_adv();           
            cout<<"vIn["<<ctr<<"] = ";
            vIn.print();
        }
    }
}