// vector_predications_boundary_awareness.cpp
#include <iostream>
#include <c7x.h>
#include <c7x_scalable.h>

using namespace std;
using namespace c7x; 

#define IMG_WIDTH  (28)
#define IMG_HEIGHT (8)
#define IMG_STRIDE (IMG_WIDTH)
int main(){
    int32_t img_buffer[IMG_STRIDE * IMG_HEIGHT];
    int32_t *pIn = (int32_t *)&img_buffer[0];
    int32_t val = 0;
    for(int32_t row = 0; row < IMG_HEIGHT; row++) {
        for(int32_t col = 0; col < IMG_WIDTH; col++) {
            pIn[(row * IMG_STRIDE) + col] = val++;
        }
    }
    pIn = (int32_t *)&img_buffer[0];
    for(int32_t row = 0; row < IMG_HEIGHT; row++) {
        for(int32_t col = 0; col < IMG_WIDTH; col++) {
            cout<<pIn[(row * IMG_STRIDE) + col]<<" ";
        }
        cout<<endl;
    }
    __SE_TEMPLATE_v1 seTemplate = __gen_SE_TEMPLATE_v1();
    seTemplate.ELETYPE   = se_eletype<int_vec>::value;
    seTemplate.VECLEN    = se_veclen<int_vec>::value;
    seTemplate.DIMFMT = __SE_DIMFMT_2D;
    seTemplate.ICNT0 = IMG_WIDTH;
    seTemplate.ICNT1 = IMG_HEIGHT;  
    seTemplate.DIM1 = IMG_STRIDE;
    __SE0_OPEN((void *)&img_buffer[0], seTemplate);
    for(int32_t ctr = 0; ctr < (IMG_HEIGHT * ceil(IMG_WIDTH/16.0)); ctr++) {
        int_vec vIn = strm_eng<0, int_vec>::get_adv();
        cout<<"vIn["<<ctr<<"] = ";
        vIn.print();
    }
    __SE0_CLOSE();
}