#include <iostream>
#include <c7x.h>
#include <c7x_scalable.h>
using namespace std;
using namespace c7x;

#define IMG_WIDTH  (32)
#define IMG_HEIGHT (8)
#define IMG_STRIDE (IMG_WIDTH)
#define NUM_CH     (3)

// 0
// 256
// 512

int main(){
    int32_t image1[NUM_CH * IMG_STRIDE * IMG_HEIGHT],image2[NUM_CH * IMG_STRIDE * IMG_HEIGHT],output[NUM_CH * IMG_STRIDE * IMG_HEIGHT];
    for(int32_t ch = 0; ch < NUM_CH; ch++) {
        int32_t *pIn = (int32_t *)&image1[(ch * (IMG_STRIDE * IMG_HEIGHT))];
        int32_t *pIn2 = (int32_t *)&image2[(ch * (IMG_STRIDE * IMG_HEIGHT))];
        int32_t val = 0;
        for(int32_t row = 0; row < IMG_HEIGHT; row++) {
            for(int32_t col = 0; col < IMG_WIDTH; col++) {
                pIn[(row * IMG_STRIDE) + col] = val++;
                pIn2[(row * IMG_STRIDE) + col] = val++;
            }
        }
    }
    int32_t *outIdx=&output[0];
    __SE_TEMPLATE_v1 seTemplate = __gen_SE_TEMPLATE_v1();
    seTemplate.ELETYPE   = se_eletype<int_vec>::value;
    seTemplate.VECLEN    = se_veclen<int_vec>::value;
    seTemplate.DIMFMT = __SE_DIMFMT_3D;
    seTemplate.ICNT0 = IMG_WIDTH;
    seTemplate.ICNT1 = IMG_HEIGHT;  
    seTemplate.DIM1 = IMG_STRIDE;
    seTemplate.ICNT2 = NUM_CH;      
    seTemplate.DIM2 = IMG_STRIDE * IMG_HEIGHT;
    __SE0_OPEN((void *)&image1[0], seTemplate);
    __SE1_OPEN((void *)&image2[0], seTemplate);
    const int vec_len = element_count_of<int_vec>::value;
    for(int32_t ctr = 0; ctr < (NUM_CH * IMG_HEIGHT * (IMG_WIDTH/vec_len)); ctr++) {
        int_vec vIn1 = strm_eng<0, int_vec>::get_adv();
        int_vec vIn2 = strm_eng<1, int_vec>::get_adv();
        // vIn1.print();
        // vIn2.print();
        int_vec vOutC = (int_vec)(0);
        vOutC = vIn1 + vIn2;
        *(int_vec *) (outIdx) = vOutC;
        outIdx += vec_len;
    }
    __SE0_CLOSE();
    cout<<"Result : "<<endl;
    for(int32_t ch = 0; ch < NUM_CH; ch++) {
        int32_t *out = (int32_t *)&output[(ch * (IMG_STRIDE * IMG_HEIGHT))];
        for(int32_t row = 0; row < IMG_HEIGHT; row++) {
            for(int32_t col = 0; col < IMG_WIDTH; col++) {
                cout<<out[(row * IMG_STRIDE) + col]<<" ";
            }
            cout<<endl;
        }
        cout<<endl;
    }
}