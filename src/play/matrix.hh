#ifndef MATRIX_HH
#define MATRIX_HH


#include <cstdlib>
#include <cstdio>
#include <time.h>
#include <algorithm>
#include <iostream>
#include <string>
#include <cstring>
#include <cstdio>
#include <fstream>
#include <vector>
#include "config.hh"

class matrix_t
{
    public:
    double ** mat;
    double *** res_mat;

    int m, n;
    int ** result_matrix;
    int length;
    int d_m = 0;
    int d_n = 0;
    struct __sort * s;
    matrix_t (int argc, char ** argv){
        if (argc < 2){
            return;
        }
        split(argv[1]);
        d_m = m;
        d_n = n;
        if (argc == 3){
            //std::cout << "random Generated" << argc << "\n"; 
            random_gen();
        } else {
            pruning(argv[1]);
        }
        dummy_ones();
        sort_rows();
        reshape_matrix();
        //print_matrix();
        //exit(0);
    }   
    ~matrix_t() {
        for (int i = 0; i < m; i++) {
            delete [] this->mat[i];
        }
        delete []  this->mat;
    }
    void dummy_ones();
    void sort_rows ();
    void print_matrix();
    int * reference_vector(int * global_buffer);
    void dump_matrix();
    void compress();
    void reshape_matrix();
    void pruning(char * filename);
    void split(const char *str);    
    void random_gen();
};

#endif


