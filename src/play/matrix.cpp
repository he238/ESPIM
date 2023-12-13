#include "matrix.hh"
#include <cmath>
#include <iostream>

int global_bandwidth = 16;
int index_fifo_size = 4;
int data_fifo_size = 4;
int num_multis = 16*11;
int load_index_size = 4;
int sram_BD = 3;
int aggr_load = 1; 
int interleave  = 1;
float sparity = 0.9;
int greedy = 1;


void matrix_t::print_matrix(){
    for (int i = 0; i < m; i++){
        for (int j = 0; j < n; j++){
           printf("%lf,", mat[i][j]);
        }
        printf("\n");
    }
    printf("\n\n\n\n");

    for (int i = 0; i < num_multis; i++){
        std::cout << i << " : ";
        for (int j = 0; j < length; j++){
           printf("%lf,", res_mat[0][i][j]);
        }
        printf("\n");
    }
        printf("\n\n\n\n");

    for (int i = 0; i < num_multis; i++){
        for (int j = 0; j < length; j++){
            std::cout << i << " : ";
            printf("%lf,", res_mat[1][i][j]);
        }
        printf("\n");
    }
}
void matrix_t::dump_matrix(){

    for (int i = 0; i < m; i++){
        for (int j = 0; j < n; j++){
           printf("%lf,", mat[i][j]);
        }
        printf("\n");
    }
    exit(0);
}
struct __sort{
    int count ;
    double * ptr;
};
bool struct_sorter(struct __sort const & lhs,struct __sort const& rhs) {
    return lhs.count < rhs.count;
}
void matrix_t::sort_rows(){

    s = new struct __sort [m];
    for (int i = 0; i < m; i++){
        s[i].ptr = mat[i];
        s[i].count = 0;
        for (int j = 0; j < n; j++){
            s[i].count += !!mat[i][j];
        }
    }
    std::sort(s, s+m, &struct_sorter);
    for (int i = 0; i < m; i++){
        mat[i] = s[i].ptr;
    }
    delete [] s;
}

void matrix_t::reshape_matrix(){
    res_mat = new double**[num_multis];
    for (int k  = 0; k < num_multis; k++){
        res_mat[k] = new double*[greedy];
    }
    // res_mat[0] = new double*[num_multis];
    // res_mat[1] = new double*[num_multis];
    length = m * n / num_multis / greedy;    
    for (int i = 0; i < num_multis; i++) {
        for (int k  = 0; k < greedy; k++){
            res_mat[i][k] = new double[length];
        }   
    }
    //int one = 0;
    // for (int i = 0; i < m; i++){
    //     for (int j = 0; j < n; j++){ 
    //         if ( mat[i][j] > 0){
    //             one ++;
    //         }
    //     }
    // }
    //std::cout << "total_ones " << one << "\n";
    // for (int k  = 0; k < greedy; k++){
    //     for (int i = 0; i < m/greedy; i++){
    //         for (int j = 0; j < n; j++){
    //             res_mat[k][i%num_multis][n*(i/num_multis) + j] = mat[i][j];
    //         }
    //     }
    // }
    //int average_one = one / m * 4;
 
    int row_used = 0;
    int ptr_base[num_multis][greedy];
    for (int i = 0; i < num_multis; i++){
        for (int j = 0; j < greedy;j++){
            ptr_base[i][j] = 0;
        }
    }
    //std::cout << m << " " << length << "\n";
    for (int i = 0; i < m / greedy; i++){
        int multi = i % num_multis;
        // if ( mat[m - i - 1][0] < -10){
        //     std::cout << i << " " << 0 << "\n";
        // }
        for (int j = 0; j < n ;j++){
            res_mat[multi][0][ptr_base[multi][0] + j] =  mat[m - i - 1][j];
        
        }
        mat[m - i - 1][0] = -100;
        ptr_base[multi][0] += n;

        for (int k = 1; k < greedy; k++){
            // if ( mat[i *(greedy - 1) +k][0] < -10){
            //     std::cout << i << " " << k << "\n";
            // }
            for (int j = 0; j < n ;j++){
                res_mat[multi][k][ptr_base[multi][k] + j] = mat[i *(greedy - 1) +k-1][j];
    
            }
            ptr_base[multi][k] += n;
            mat[i *(greedy - 1) +k-1][0] = -100;
        }
    }

    //std::cout << "total_ones " << one << "\n";

    for (int k  = 0; k < greedy; k++){
        //res_mat[0][k][0] = 0;
    }
    // for (int i = 0; i < num_multis; i++) {
    //     for (int k  = 0; k < greedy; k++){
    //         for (int j = 0; j < length; j++){
    //             if (res_mat[i][k][j] > 0){
    //                 one ++;
    //             }
    //         }
    //     }   
    // }
    // std::cout << "total_ones " << one << "\n";
    // for (int i = 0; i < num_multis; i++){
    //     for (int j = 0; j < greedy;j++){
    //         std::cout << ptr_base[i][j] << " ";
    //     } 
    //     std::cout << "\n";
    // }
    //Wexit(0);
    return;
}

void matrix_t::dummy_ones(){
    int dummy_count = 0;
    for (int i = 0; i < m; i++){
        for (int j = 0; j < n; j+=16){
            int one = 0;
            for (int k = 0; k < 16; k++){
                if (mat[i][j+k] != 0){
                    one = 1;
                    break;
                }
            }
            if (one == 0){
                mat[i][j] = -1;    
                dummy_count ++;
            }
        }
    }
    //std::cout << dummy_count << "\n";
}

int * matrix_t::reference_vector(int * global_buffer){
    int * results_vector = new int [this->m];
    for (int i = 0; i < this->m ; i++){
        int tmp = 0;
        for (int j = 0; j < this->n ; j++){
            tmp = tmp + global_buffer[j] * this -> mat[i][j];
        }
       // std::cout << tmp << "\n";
    }
    return results_vector; 
}

void matrix_t::split(const char *str)
{
    std::vector<std::string>  result;
    char c = '.';
    do {
        const char *begin = str;
        while(*str != c && *str)
            str++;
        result.push_back(std::string(begin, str));
    } while (0 != *str++);
    this->m = stoi(result[5]);
    this->n = stoi(result[6]);
    return;
}

void matrix_t::random_gen(){
    if (m % num_multis != 0) {
        this -> m = (m / num_multis + 1) * num_multis ;
    }    this->mat = new double*[m];
    for (int i = 0; i < m; i++) {
        this->mat[i] = new double[n];
    }   
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n ; j ++){
            if ((random()%10 == 0)){
                this->mat[i][j] = j;
            } else {
                this->mat[i][j] = 0;
            }
        }
    }   
}

void matrix_t::pruning(char * filename){
    std::ifstream infile;
    infile.open(filename);
    //std::cerr<< filename << " ";
    //std::vector<std::string> size = split(filename, '.');
    //std::cout << this->m << " " << this->n << "\n";
    double * sort_arary = new double[m*n];
    double * unsort_arary = new double[m*n];

    std::string words;
    infile >> words;
    const char s[2] = ",";
    char *token;
    //std::cout << words << "\n";

    token = strtok((char*)words.data(), s);
    sort_arary[0]=fabsl(atof(token));
    int count = 1;
    while( (token = strtok(NULL, s)) != NULL && count < m*n) {
        //std::cout << token << "\n";
        sort_arary[count++] = fabsl(atof(token));
        //std::cout << count << "\n";
    }
    //exit(0);
    //std::cout << sort_arary[0] << " "  << sort_arary[count -1] << " " << sort_arary[(int)(count*0.1)] << " " << sort_arary[(int)(count*0.9)] << "\n";
    if (m % num_multis != 0) {
        this -> m = (m / num_multis + 1) * num_multis ;
    }

    this->mat = new double*[m];
    for (int i = 0; i < m; i++) {
        this->mat[i] = new double[n];
    }   

    for (int i = 0; i < count; i++){
        unsort_arary[i] = sort_arary[i]; 
    }

    std::sort(sort_arary, sort_arary+count);
    //std::cout << sort_arary[0] << " "  << sort_arary[count-1] << " " << sort_arary[(int)(count*0.1)] << " " << sort_arary[(int)(count*0.9)] << "\n";
    double cutoff = sort_arary[(int)(count*sparity)];
    for (int i = 0; i < count; i++) {
        if (unsort_arary[i] > cutoff){
            mat[i/n][i%n] = unsort_arary[i];
        } else {
            mat[i/n][i%n] = 0;
        }
    }
    int i = count;
    while(i < m * n) {
        mat[i/n][i%n] = 0;
        i++;
    }
    int ones = 0;
    for (int i = 0; i < m; i++){
        for (int j = 0; j < n; j++){
            //std::cout << mat[i][j] << ", "; 
            if (mat[i][j] != 0){
                ones++;
            }
        }
       //std::cout << "\n";
    }
    //std::cout<< " " << ones << "\n";
    delete sort_arary;
    delete unsort_arary;
}
