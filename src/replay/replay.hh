#include <unistd.h>
#include <queue>

#include  <cstdlib>
#include  <cstdio>
#include  <time.h>
#include <algorithm>
#include <iostream>
#include <string>
#include <cstring>
#include <cstdio>
#include <fstream>
#include <vector>

int line_number = 0;
typedef struct __data_block_t{
    double value;
    int next;
    int index;
    int y;
    __data_block_t(double _value, int _next, int _index, int _y){
        value = _value;
        next = _next;
        index = _index;
        y = _y;
    }
    __data_block_t(){
        value = 0;
        next = 0;
        index = 0;
        y = 0;
    }
} data_block_t;


typedef struct __index{
    int next;
    int index;
    __index(int _next, int _index){
        next = _next;
        index = _index;
    }
} index_t;

int compute_count = 0;
int block_count = 0;


class multi_t{
    public:
    std::queue <index_t *> index_fifo;
    std::queue <int> value_fifo;
    int id;
    int gi;
    int push_gi;
    data_block_t * db;
    double sum [2];
    multi_t(int _id, data_block_t* _db){
        id  = _id;
        db = _db;
        gi = 0;
        push_gi = 0;
        sum[0] = 0;
        sum[1] = 0;
    }
    ~multi_t(){
    }
    int load_index();
    int cycle(int * global_buff,int gb_index);

};

class DRAM_t{
    public:
    int * global_buff;
    int gb_index;
    multi_t ** multis;
    int ** matrix;
    std::fstream traceFile;
    std::fstream commandFile;
    int cycles;
    data_block_t * read_line;
    int global_buf_length;
    DRAM_t (char * filename1, char* filename2){
        cycles = 0;
        compute_count = 0;
        block_count = 0;
        gb_index = 0;
        multis = new multi_t*[num_multis];
        traceFile.open(filename1, std::ios::in); 
        commandFile.open(filename2, std::ios::in);
        std::string command;    
        getline(commandFile, command);
        global_buf_length = stoi(command);
        getline(commandFile, command);

        global_buff = new int [global_buf_length];
        read_line =  new data_block_t[num_multis];
        for (int i = 0; i < global_buf_length ;i++){
            global_buff[i] = i % stoi(command);
        }
        for (int i = 0; i <  num_multis; i++){
            multis[i] = new multi_t(i, &read_line[i]);
        }
    }
    ~DRAM_t(){
        traceFile.close();
        commandFile.close();
    }
    void load_index();
    int cycle(int forward);
    void replay();
    int load_trace();

};
