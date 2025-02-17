// transpose_mode_access.cpp
#include <iostream>
#include <c7x.h>
#include <c7x_scalable.h>

using namespace std;
using namespace c7x; 

#define IMG_WIDTH  (16)
#define IMG_HEIGHT (32)
#define IMG_STRIDE (IMG_WIDTH)
#define NUM_CH     (3)

int main(){
    int32_t img_buffer[NUM_CH * IMG_STRIDE * IMG_HEIGHT];
    for(int32_t ch = 0; ch < NUM_CH; ch++) {
        int32_t *pIn = (int32_t *)&img_buffer[(ch * (IMG_STRIDE * IMG_HEIGHT))];
        int32_t val = 0;
        for(int32_t row = 0; row < IMG_HEIGHT; row++) {
            for(int32_t col = 0; col < IMG_WIDTH; col++) {
                pIn[(row * IMG_STRIDE) + col] = val++;
            }
        }
    }
    for(int32_t ch = 0; ch < NUM_CH; ch++) {
        int32_t *pIn = (int32_t *)&img_buffer[(ch * (IMG_STRIDE * IMG_HEIGHT))];
        cout<<"Ch = "<<ch<<" : "<<endl;    
        for(int32_t row = 0; row < IMG_HEIGHT; row++) {
            for(int32_t col = 0; col < IMG_WIDTH; col++) {
                cout<<pIn[(row * IMG_STRIDE) + col]<<" ";
            }
            cout<<endl;
        }
    }
    __SE_TEMPLATE_v1 seTemplate = __gen_SE_TEMPLATE_v1();
    seTemplate.ELETYPE   = se_eletype<int_vec>::value;
    seTemplate.VECLEN    = se_veclen<int_vec>::value;
    seTemplate.TRANSPOSE    = __SE_TRANSPOSE_32BIT;
    seTemplate.DIMFMT = __SE_DIMFMT_4D;
    const int vec_len = element_count_of<int_vec>::value;
    seTemplate.ICNT0 = IMG_WIDTH;
    seTemplate.ICNT1 = vec_len;              
    seTemplate.DIM1 = IMG_STRIDE;
    seTemplate.ICNT2 = IMG_HEIGHT/vec_len;   
    seTemplate.DIM2 = (IMG_STRIDE * vec_len);
    seTemplate.ICNT3 = NUM_CH;               
    seTemplate.DIM3 = IMG_STRIDE * IMG_HEIGHT;
    __SE0_OPEN((void *)&img_buffer[0], seTemplate);
    int32_t ctr = 0;
    for(int32_t ch = 0; ch < NUM_CH; ch++) {
        cout<<"Ch = "<<ch<<endl;    
        for(int32_t row = 0; row < (IMG_HEIGHT/vec_len); row++) {
            for(int32_t col = 0; col < IMG_WIDTH; col++) {
                int_vec vIn = strm_eng<0, int_vec>::get_adv();
                printf("vIn[%d] = ", ctr++);
                vIn.print();
            }
            cout<<endl;
        }
    }
}