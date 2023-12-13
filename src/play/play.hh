#include "config.hh"
#include "matrix.hh"

#include <unistd.h>
#include <queue>

int global_multi_count = 0;
typedef struct __index{
    int isReal, next, index, y, x;
    __index(int _isReal, int _next, int _index, int _x, int _y){
        isReal = _isReal;
        next = _next;
        index = _index;
        x = _x;
        y = _y;
    }
} index_t;

typedef struct __value{
    int x;
    int y;
    double value;
    __value(int _x, int _y, double _value){
        x = _x;
        y = _y;
        value = _value;
    }
} value_t;


typedef struct __row{
    int ii;
    int vi;
    int gi;
    double * row;
    __row(double * _row){
        ii = 0; 
        vi = 0;
        gi = 0;
        row = _row;
    }
} row_t;




class multi_t{
    public:
    std::queue <index_t *> index_fifo;
    std::queue <value_t *> value_fifo;
    std::queue <index_t *> all_index;

    row_t ** r; 
    double sum [2];
    int id;
    int ii;
    // int vi;
    int gi;
    int finished;
    FILE * traceFile;
    int blockReason[10]; 
    int length;
    int gb_index_index;
    int n = 0;
    multi_t(int _id, double *** row,  FILE * _traceFile, int _length, int _n){
        id  = _id;
        finished = 0;
        r = new row_t * [greedy];
        for (int i = 0; i < greedy; i++){
            r[i] = new row_t(row[id][i]);
        }
        // r[0] = new row_t(row1); 
        // r[1] = new row_t(row2); 
        sum[0] = 0;
        ii = 0;
        gi = 0;
        sum[1] = 0; 
        n = _n;
        traceFile = _traceFile;
        length = _length;
        for (int i = 0; i < 10; i++){
            blockReason[i] = 0;
        }
        //reference();
        gb_index_index = 0;
    }
    ~multi_t(){
    }
    int load_index();
    int cycle(int * global_buff, int gb_index);
    void reference();
};

class DRAM_t{
    public:
    int * global_buff;
    int gb_index;
    int gi;
    multi_t ** multis;
    double *** matrix;
    FILE * traceFile;
    FILE * commandFile;
    int length;
    int cycles;
    int m; 
    int n;
    int load_count;
    DRAM_t (double *** _matrix, int size, int global_buf_length, int _m, int _n){
        length = global_buf_length;
        m = _m;
        n = _n;
        cycles = 0;
        load_count = 0;
        global_buff = new int [length];
        traceFile = fopen("Trace.txt", "w");
        commandFile = fopen("Command.txt", "w");
        for (int i = 0; i < length; i++){
            global_buff[i] = i;
        }
        gb_index = 0; 
        matrix = _matrix;
        multis = new multi_t*[num_multis];
        for (int i = 0; i < num_multis; i++){
            multis[i] = new multi_t (i , _matrix, traceFile, length, _n);
        }
        fprintf(commandFile, "%d\n%d\n", length, n);
    }
    ~DRAM_t(){
        fclose(traceFile);
        fclose(commandFile);
    }
    void load_index();
    int cycle();
    void play();

};
