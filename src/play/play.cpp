
#include "play.hh"
#include <unistd.h>
#include <queue>
#define DEBUG 0

int multi_t::load_index(){
    if (ii >= length){
        fprintf(traceFile, "-1, -1, -3456, -3456, ");
    }
    if (index_fifo.size() == index_fifo_size){
        fprintf(traceFile, "-1, -1, -3456, -3456, ");
        return -1;
    }
   /*  int j = 0;
    while (true){
        j = 0;
        if (r[0]->ii > r[1] ->ii){
            j = 1;
        }
        r[j] -> ii++;
        //std::cout << j << " " << r[0]->ii << " " << r[1] ->ii << "\n";
        if (r[j]->ii>length){
            fprintf(traceFile, "-1, -1, -3456, -3456, ");
            break;
        }
        if(r[j]->row[r[j]->ii] != 0){ //find one
            int find_next = 0;
            int next = 0;

            while (r[j]->ii + find_next + 1 < length){
                find_next++;
                //std::cout << "find 0 " << r[j]->ii << " " << find_next << "\n";

                if ((r[j]->ii + find_next) % global_bandwidth == 0){
                    int find_next_other = 0;
                    while (r[!j]->ii + find_next_other + 1 < length){
                        find_next_other++;
                        if (r[!j]->row[r[!j]->ii + find_next_other] != 0){
                            if ((r[!j]->ii + find_next_other) / global_bandwidth > gb_index_index){
                                gb_index_index ++; 
                                next = 1;
                            }
                            break;
                        }
                    }
                    break;
                }
                if(r[j]->row[r[j]->ii + find_next] != 0){
                    break;
                }
            }
    
            if (r[j]->row[r[j]->ii] == -1){
                if (next == 1){
                    index_t *i = new index_t(!(r[j]->row[r[j]->ii]>0), next, r[j]->ii%16, r[j]->ii, j);
                    index_fifo.push(i);
                    fprintf(traceFile, "-1, %d, -3456, -3456, ", next);
                    break;
                }
            } else {
                index_t *i = new index_t(!(r[j]->row[r[j]->ii]>0), next, r[j]->ii%16, r[j]->ii, j);
                index_fifo.push(i);
                fprintf(traceFile, "%d, %d, -3456, -3456, ", r[j]->ii%16, next);
                break;
            }
            //fprintf(traceFile, "%d, %d,  ", ii%16, next);
        }
    } */

   if (all_index.size() == 0){
        std::vector <index_t *> tmp;
        // add all index to all_index
        for (int j = 0; j < greedy; j++){
            for (int i = 0; i < global_bandwidth; i++){
                if ( r[j]-> row[ii + i] != 0){
                    index_t *ind = new index_t(!(r[j]->row[ii+i]>0), 0, (ii + i)%16, ii + i, j);
                    tmp.push_back(ind);
                    //std::cout << "push " << r[j]-> ii + i << " " << r[j]-> row[r[j]-> ii + i] << " " <<  ind -> index << " "  << ind->y << "\n";
                }
            }
        } 
        if (tmp.size() == 0){
            index_t *i = new index_t(0 , 1, ii%16, ii, 0);
            tmp.push_back(i);
        }
    
        // std::cout << tmp.size() << ":";
        // for (size_t i = 0; i < tmp.size(); i++)
        // {
        //     std::cout << tmp[i] -> index << ", ";
        // }
        // std::cout << "\n";
        //no interleaving
        if (interleave == 1){
            while (true){
                for (int  i = 0; i < 4; i++){
                    for (int j = 0; j < tmp.size(); j++){
                        if (tmp[j] -> index / 4 == i){
                            all_index.push(tmp[j]);
                            tmp.erase(tmp.begin()+j); 
                            break;
                        } 
                    }
                }
                if (tmp.size() == 0){
                    all_index.back() -> next = 1;
                    break;
                }
            }
        } else {
            int size =  tmp.size();
            for (int j = 0; j < size; j++){
                all_index.push(tmp[j]);
            }      
        }
        all_index.back() -> next = 1;

        // std::cout << all_index.size() << ":";
        // for (size_t i = 0; i < all_index.size(); i++) {
        //     std::cout << all_index.front() -> index << ", ";
        //     all_index.push(all_index.front());
        //     all_index.pop();
        // }
        // std::cout << "\n";
        ii += global_bandwidth;
        index_fifo.push(all_index.front());
        fprintf(traceFile, "%d, %d, -3456, -3456,", all_index.front()->index, all_index.front()->next);            
        all_index.pop();
    } else { 
        index_fifo.push(all_index.front());
        //std::cout << "pushing " << all_index.front() -> index <<  "\n";
        fprintf(traceFile, "%d, %d,  -3456, -3456,", all_index.front()->index, all_index.front()->next);            
        all_index.pop();
    } 
    
    return 0;
}

void multi_t::reference(){
    for (int j = 0; j < length; j++){
        if ( r[0]->row[j] > 0){
            sum[0] += r[0]->row[j] * (j %n);
        }

    }
   for (int j = 0; j < length; j++){
        if ( r[1]->row[j] > 0){
            sum[1] += r[1]->row[j] * (j %n);
            //std::cout << r[1]->row[j] << " " << (j %n) << "\n";
        }
    }

    std::cout << sum[0] << " " << sum[1] << "\n";
    return;
}

int multi_t::cycle(int * global_buff, int gb_index){
    if (DEBUG == 1){
        std::cout << "cycle " << " " << id << " " << gb_index << " " << gi << " " << index_fifo.size() <<  " " << value_fifo.size() << "\n";
    }
    blockReason[0] = 0;
    blockReason[1] = 0;
    blockReason[2] = 0;
    blockReason[3] = 0;

    int prev_index = -1;
    int block = 0;
    //global block        
    for (int i = 0; i < sram_BD; i++){
        if (index_fifo.size() == 0){
            blockReason[0] = 1;
            block = 1;
            break;
        } 
        if (value_fifo.size() == data_fifo_size){
            blockReason[1] = 1;
            block = 1;
            break;
        }
        index_t *current = index_fifo.front();
        if (prev_index != -1 && prev_index / 4 == current->index / 4){
            blockReason[3] = 1;
            prev_index = -1;
            block = 1;
            break;
        }
	    prev_index = current->index;
        if (gi > gb_index || gb_index == length/global_bandwidth){
            break;
        }
        index_fifo.pop();
        if (DEBUG == 1){
            std::cout << "Poping index " << id << " " << current->y << " " << current->x << " " << current->index << " " << gi <<"\n";
        }
        if (current->isReal == 0){
            value_t  * v = new value_t(current->x, current->y,  r[current->y] -> row[current->x]);
            value_fifo.push(v);
            if (DEBUG == 1)
                std::cout << "Pushing value " << id << " " << current->x << " " << current->y << "\n";
        }
        if (current -> next == 1){
            //r[current->y]->gi++;
            gi++;
            block = 0;
            break;
        }
        if (i == sram_BD - 1){
            blockReason[2] = 1;
            block = 1;
            break;    
        }
    }   
   /* //multiply
    //+++++++++++++++++++++++++++++++++++++++++++++++
    // if (r[0]->ii > length && r[1] ->ii > length){
    //     block = 0;
    // }
    // if (index_fifo.size() == index_fifo_size){
    //     fprintf(traceFile, "-1, -1, ");
    // } else {
    //     //find next avaiable location and then determine j
    //     int j = 0;
    //     while (true){
    //         j = 0;
    //         if (r[0]->ii > r[1] ->ii){
    //             j = 1;
    //         }
    //         r[j] -> ii++;
    //         //std::cout << j << " " << r[0]->ii << " " << r[1] ->ii << "\n";
    //         if (r[j]->ii>=length){
    //             fprintf(traceFile, "-1, -1, ");
    //             break;
    //         }
    //         if(r[j]->row[r[j]->ii] != 0){ //find one
    //            //std::cout << "find 0 " << r[j]->ii << " " << r[j]->row[r[j]->ii] << "\n";

    //             int find_next = 0;
    //             int next = 0;
    //             while (r[j]->ii + find_next + 1 < length){
    //                 find_next++;
    //                 if ((r[j]->ii + find_next) % global_bandwidth == 0){
    //                     int find_next_other = 0;
    //                     //if (id == 0)
    //                         //std::cout << "find 1 " << r[j]->ii << " " << find_next << "\n";
    //                     while (r[!j]->ii + find_next_other + 1 < length){
    //                         find_next_other++;
    //                         if (r[!j]->row[r[!j]->ii + find_next_other] != 0){
                  
    //                             if ((r[!j]->ii + find_next_other) / global_bandwidth > gb_index_index){
    //                                 gb_index_index ++;
    //                                 next = 1;
    //                             }
    //                             break;
    //                         }
    //                     }
    //                     break;
    //                 }
    //                 if(r[j]->row[r[j]->ii + find_next] != 0){
    //                     break;
    //                 }
    //             }
       
    //             if (r[j]->row[r[j]->ii] == -1){
    //                 if (next == 1){
    //                     index_t *i = new index_t(!(r[j]->row[r[j]->ii]>0), next, r[j]->ii%16, r[j]->ii, j);
    //                     index_fifo.push(i);
    //                     fprintf(traceFile, "-1, %d, ", next);
    //                     break;
    //                 }
    //             } else {
    //                 index_t *i = new index_t(!(r[j]->row[r[j]->ii]>0), next, r[j]->ii%16, r[j]->ii, j);
    //                 index_fifo.push(i);
    //                 fprintf(traceFile, "%d, %d, ", r[j]->ii%16, next);
    //                 break;
    //             }
    //             //fprintf(traceFile, "%d, %d,  ", ii%16, next);

    //         }
    //     }
    //     if (DEBUG)
    //         std::cout<< "Pushing index " << id << " " << j << " " << r[j]->ii << " " << gb_index << "\n";
    // }
    //+++++++++++++++++++++++++++++++++++++++++++++++=
    
    // //find all next locations in next global bd
    //------------------------------------------------ */
   
    if (ii >= length){
        block = 0;
        fprintf(traceFile, "-1, -1, ");
    } else if (index_fifo.size() == index_fifo_size){
        fprintf(traceFile, "-1, -1, ");
    } else if (all_index.size() == 0){
        std::vector <index_t *> tmp;
        // add all index to all_index
        for (int j = 0; j < greedy; j++){
            for (int i = 0; i < global_bandwidth; i++){
                if ( r[j]-> row[ii + i] != 0){
                    index_t *ind = new index_t(!(r[j]->row[ii+i]>0), 0, (ii + i)%16, ii + i, j);
                    tmp.push_back(ind);
                    //std::cout << "push " << r[j]-> ii + i << " " << r[j]-> row[r[j]-> ii + i] << " " <<  ind -> index << " "  << ind->y << "\n";
                }
            }
        } 
        if (tmp.size() == 0){
            index_t *i = new index_t(0 , 1, ii%16, ii, 0);
            tmp.push_back(i);
        }
    
        // std::cout << tmp.size() << ":";
        // for (size_t i = 0; i < tmp.size(); i++)
        // {
        //     std::cout << tmp[i] -> index << ", ";
        // }
        // std::cout << "\n";
        //no interleaving
       
        // interleaving
        if (interleave == 1){

            while (true){
                for (int  i = 0; i < 4; i++){
                    for (int j = 0; j < tmp.size(); j++){
                        if (tmp[j] -> index / 4 == i){
                            all_index.push(tmp[j]);
                            tmp.erase(tmp.begin()+j); 
                            break;
                        } 
                    }
                }
                if (tmp.size() == 0){
                    all_index.back() -> next = 1;
                    break;
                }
            }
        } else {
            int size =  tmp.size();
            for (int j = 0; j < size; j++){
                all_index.push(tmp[j]);
            }
        }

        all_index.back() -> next = 1;

        // std::cout << all_index.size() << ":";
        // for (size_t i = 0; i < all_index.size(); i++) {
        //     std::cout << all_index.front() -> index << ", ";
        //     all_index.push(all_index.front());
        //     all_index.pop();
        // }
        // std::cout << "\n";
        ii += global_bandwidth;
        index_fifo.push(all_index.front());
        fprintf(traceFile, "%d, %d, ", all_index.front()->index, all_index.front()->next);            
        all_index.pop();
    } else { 
        index_fifo.push(all_index.front());
        //std::cout << "pushing " << all_index.front() -> index <<  "\n";
        fprintf(traceFile, "%d, %d, ", all_index.front()->index, all_index.front()->next);            
        all_index.pop();
    }
   
    //------------------------------------------------

      
    if (value_fifo.size() == 0){
        fprintf(traceFile, "0, 0, ");
        //std::cout << gb_index << " " << length/global_bandwidth << "\n";
        if (gb_index >= length/global_bandwidth - 1){
            finished = 1;
        }
    } else {
        value_t * v;
        v = value_fifo.front();
        value_fifo.pop();
        fprintf(traceFile, "%lf, %d, ", r[v->y] -> row[v->x], v->y);
        global_multi_count++;
        if (DEBUG)
            std::cout << "Poping value " << id << " " << v->y << " " << v -> x << " " << v -> value << " " << r[v->y] -> row[v->x] <<"\n";
    }
    //std::cerr << "cycle " << " " << id << " " << gb_index << " " << gi << " " << index_fifo.size() <<  " " << value_fifo.size() << " " << block << "\n";
    if (DEBUG){
        if (block == 1){
            std::cout << "Block " << " " << id << " " << gb_index << " " << gi << " " << index_fifo.size() <<  " " << value_fifo.size() << " " << ii << " " << length << " " << block  << blockReason[0] << blockReason[1]<< blockReason[2] << "\n";
        }
    }
    return block;
}

int DRAM_t::cycle(){
    int block = 0;
    int done = 1;
    for (int i = 0; i < num_multis; i++){
        if ( multis[i] -> finished == 0){
            done = 0;
        } 
    }   

    for (int i = 0; i < num_multis; i++){
        if (multis[i] -> cycle(&global_buff[gb_index*global_bandwidth], gb_index)){
            block = 1;
        } 
        //std::cout << i << " " << block << "\n";
        //fprintf(traceFile, " | ");
    }    
    fprintf(traceFile, " \n");
    //std::cout << "block " << block << "\n";
    if (block == 0 && gb_index < length/global_bandwidth){
        fprintf(commandFile, "NextCompute\n");
        gb_index += 1;
    } else {
        fprintf(commandFile, "Compute\n");
    }
    return done;
}

void DRAM_t::load_index(){
    load_count++;
    fprintf(commandFile, "Load\n");
    //std:cout << "load" << gb_index << "\n";

    int load_size = 0;
    fprintf(traceFile, "//+++++++++++++++++++++++++\n");

    while (load_size < load_index_size){
        for (int i = 0; i < num_multis; i++){
            multis[i] -> load_index();
        }
        load_size++; 
        fprintf(traceFile, "\n");
    } 

}

void DRAM_t::play(){
    //std::cout  << length << "\n";
    int reason[10] ;
    for (int i = 0; i < 10; i++){
        reason[i] = 0;
    }
    
    // for (int i = 0; i < 4; i++){
    //     load_index();
    //     cycles++;
    // }
   // while (true){
    for (int i = 0; i <30000; i++){
        //std:cout << "cycle " << cycles << "\n";
        end:
        // std::cout << gb_index << " ";
        // for (int j= 0; j < num_multis ; j++){
        //     std::cout <<  multis[j] -> value_fifo.size() << " " <<  multis[j] -> index_fifo.size() << " | ";
        // }
        // std::cout << "\n";
        //std::cout << "+++++"  << gb_index <<  "+++++"  << cycles << "\n";

        cycles++;
        if (cycle()){
            //fprintf(stderr, "%d %d \n", cycles, m *n / 16/16);
            break;
        }
        int block_count = 0;
        for (int i = 0; i < num_multis; i++){
            if (multis[i]->blockReason[0] == 1){
               block_count ++;
            }
        }
        if (block_count >= 1 && gb_index != length/global_bandwidth){
            if (aggr_load == 1){
                load_index();
                //std::cout << "+++++++++++++++++++++++++\n";
            }
        }


        for (int i = 0; i < num_multis; i++){
            if (multis[i]->blockReason[0] == 1){
                reason[0] += 1;
                goto end;
            }
        }

        for (int i = 0; i < num_multis; i++){
            if (multis[i]->blockReason[1] == 1){
                reason[1] += 1;
                goto end;
            }
        }

        for (int i = 0; i < num_multis; i++){
            if (multis[i]->blockReason[2] == 1){
                reason[2] += 1;
                goto end;
            }
        }
          for (int i = 0; i < num_multis; i++){
            if (multis[i]->blockReason[3] == 1){
                reason[3] += 1;
                goto end;
            }
        }
    }

    //fprintf(stderr, " %d %d %d\n", reason[0], reason[1], reason[2]);

    std::cerr << "index fifo empty " << reason[0] << "\n";
    std::cerr << "value fifo full " << reason[1] << "\n";
    std::cerr << "SRAM bandwidth " << reason[2] << "\n";
    std::cerr << "conflict bandwidth " << reason[3] << "\n";

    //std::cerr << load_count << "\n";
    std::cout << cycles << " ";
}

int main (int argc, char ** argv){

    index_fifo_size = atoi(argv[2]);
    data_fifo_size = atoi(argv[2]);
    aggr_load = atoi(argv[3]); 
    interleave  = atoi(argv[4]);;
    sparity = atof(argv[5]);
    greedy = atoi(argv[6]);
    std::cout << argv[1] << "_" << index_fifo_size << "_"<< aggr_load << "_" << interleave << "_" << sparity << "_" << greedy << " ";
    
    matrix_t * mat = new matrix_t(argc, argv);
    //mat -> print_matrix();;
    DRAM_t * DRAM = new DRAM_t(mat->res_mat, mat->m * mat->n, mat->length, mat -> d_m, mat->d_n);
    DRAM -> play();
    //std::cout << "global_multi_count" << global_multi_count << "\n";
    return 0;
}   
