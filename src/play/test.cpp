#include "DRAM.hh"
#include "matrix.hh"


int main (int argc, char ** argv){
    //matrix_t * mat = new matrix_t(argc, argv);
    //mat->dump_matrix();
    int test[128] = {1,0,0,0,0,9,0,0,0,0,0,0,0,0,0,1,1,7,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,8,0,0,0,0,0,0,2,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,4,0,0,0,0,0,0,0,0};
    DRAM_t * DRAM = new DRAM_t(NULL);
    DRAM->multis[0]->load_index(test);
    DRAM->multis[0]->load_index(test);

    return 0;
}   
 