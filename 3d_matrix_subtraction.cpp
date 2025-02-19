/*
To run this
cd scripts
./3d_mat_sub.sh
*/
#include <iostream>
#include <c7x.h>
#include <c7x_scalable.h>

using namespace std;
using namespace c7x;

void matrix_sub_streaming_engine(int row,int col,int depth){
    int32_t mat1[depth][row][col] , mat2[depth][row][col] , output[depth][row][col];
    for(int d = 0;d < depth;d++){
        for(int r = 0;r < row;r++){
            for(int c = 0;c < col;c++){
                mat1[d][r][c] = r+c+d;
                mat2[d][r][c] = (r+c+d) * -1;
            }
        }
    }
    int32_t *outIdx=&output[0][0][0];
    __SE_TEMPLATE_v1 seTemplate = __gen_SE_TEMPLATE_v1();
    seTemplate.ELETYPE   = se_eletype<int_vec>::value;
    seTemplate.VECLEN    = se_veclen<int_vec>::value;
    seTemplate.DIMFMT = __SE_DIMFMT_3D;
    seTemplate.ICNT0 = col;
    seTemplate.ICNT1 = row;  
    seTemplate.DIM1 = col;
    seTemplate.ICNT2 = depth;      
    seTemplate.DIM2 = row * col;
    __SE0_OPEN((void *)&mat1[0][0][0], seTemplate);
    __SE1_OPEN((void *)&mat2[0][0][0], seTemplate);
    const int vec_len = element_count_of<int_vec>::value;
    int times = (row * col * depth) / vec_len;
    for(int t = 0;t < times ;t++){
        int_vec vIn1 = strm_eng<0, int_vec>::get_adv();
        int_vec vIn2 = strm_eng<1, int_vec>::get_adv();
        int_vec vOutC = (int_vec)(0);
        vOutC = vIn1 - vIn2;
        *(int_vec *) (outIdx) = vOutC;
        outIdx += vec_len;
    }
    __SE0_CLOSE();
    __SE1_CLOSE();
    cout<<"Result : "<<endl;
    for(int32_t ch = 0; ch < depth; ch++) {
        int32_t *out = (int32_t *)&output[ch][0][0];
        for(int32_t r = 0; r < row; r++) {
            for(int32_t c = 0; c < col; c++) {
                cout<<out[(r * col) + c]<<" ";
            }
            cout<<endl;
        }
        cout<<endl;
    }
}
int main(){
	int row,col,depth;
    cout<<"Enter Row Size : ";
    cin>>row;
    cout<<"Enter Column Size : ";
    cin>>col;
    cout<<"Enter Depth Size : ";
    cin>>depth;
    matrix_sub_streaming_engine(row,col,depth);
}




// Test Appliction utilize borh address generator
// Dividide by 4 (4096 / 4)
// Each 
// convert to 2D array and Transpose it uint64_t
// Insert into new Array And Store 