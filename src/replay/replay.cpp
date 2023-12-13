#include "../play/config.hh"
#include "replay.hh"

#include <unistd.h>
#include <queue>
#include <iostream>
#include <fstream>
#include <bits/stdc++.h>

#include <string>
#define DEBUG 0

int multi_t::load_index(){    
    index_t *i = new index_t( db -> next, db -> index);      
    //std::cerr << "finding index" << id << " " << gi << " " << db -> index << "\n";
    if (index_fifo.size() != data_fifo_size){
        ///std::cerr << "pushing index" << id << " " << gi << " " << db -> index << " "<< value_fifo.size() << "\n";
        index_fifo.push(i);
    }
    return 0;
}

int multi_t::cycle( int * global_buff, int gb_index){
    //std::cout  << "cycle " << id << " " << gb_index << " " << gi << " " << index_fifo.size() <<  " " << value_fifo.size() << "\n";
    int block = 0;
    //global block
    for (int i = 0; i < 8; i++){
     
        if (index_fifo.size() == 0){
            block = 1;
            //std::cout << "index_fifo.size()" << "\n";
            break;
        } 
        if (value_fifo.size() == data_fifo_size){
            block = 1;
           //std::cout << "value_fifo.size()" << "\n";
            break;
        }
        index_t *current = index_fifo.front();
        if (gi > gb_index){
             break;
        }

        index_fifo.pop();
        if (DEBUG)
            std::cout << "pop index " << " db->index: " << current->index << " db->next " << current->next << " gi " << gi << " gb_index " << gb_index << "\n";
        if (current->index != -1){
            if (DEBUG){
                std::cout << "push value " << " value: " << global_buff[current->index] << "\n";
            }
            value_fifo.push(global_buff[current->index]);
        }
        if (current -> next == 1){
            gi++;
            block = 0;
            break;
        }
        if (i == 7){
            block = 1;
            break;
        }
    }
    //multiply
    if (index_fifo.size() == index_fifo_size){

    } else {
        index_t *i = new index_t(db->next, db->index);
        index_fifo.push(i);
        if (DEBUG)
            std::cout << "push index " << " db->index: " << db->index << " db->next " << db->next << " gi " << gi << " gb_index " << gb_index << "\n";
    }
 
    if (value_fifo.size() == 0){
        block_count += 1;   
    } else{
        int value = value_fifo.front();
        if (value != db->value && db->value != -1 && db->value != 0){
            //fprintf(stderr, "Wrong Value %d %d %d %d %d %f %d\n", line_number,id, gi, gb_index, value, db->value, db->y);
        } 
        // else {
        //    fprintf(stderr, "Right Value %d %d %d %d %d %f %d\n", line_number, id, gi, gb_index, value, db->value, db->y);
        // }
        sum[db->y] += db->value * value;
        // if (db->y == 1 && id == 0)
        //     std::cout <<db->value << " " << value << "\n";
        if ( db->value == 0){
            block_count += 1;
        } else {
            compute_count += 1;
        }
        value_fifo.pop();
     }
    return block;
}
int DRAM_t::load_trace(){
    std::string buff;    
    getline(traceFile, buff);

    if (buff.c_str()[0] == '/' && buff.c_str()[1] == '/' ){
        getline(traceFile, buff);
    }
    const char * str = buff.c_str();
    //std::cout << str << "\n";
    char *token = strtok((char *)str, ", ");
    for (int i = 0; i < num_multis; i++){
        read_line[i].index = atoi(token);
        token = strtok(NULL, ", ");
        read_line[i].next = atoi(token);
        token = strtok(NULL, ", ");
        read_line[i].value = atof(token);
        token = strtok(NULL, ", ");
        read_line[i].y = atoi(token);
        token = strtok(NULL, ", ");
    }
    return 0;
}
int DRAM_t::cycle(int forward){
    load_trace();
    line_number++;
    int block = 0;
    for (int i = 0; i < num_multis; i++){
        block != multis[i] -> cycle(&global_buff[gb_index*global_bandwidth], gb_index);
    }    
    if (forward == 1 && gb_index < global_buf_length/ global_bandwidth){
        gb_index += 1;
    } 
    return  block;
}

void DRAM_t::load_index(){
    int load_size = 0;
    while (load_size < load_index_size){
        load_trace();
        for (int i = 0; i < num_multis; i++){
            multis[i] -> load_index();
        }
        load_size++; 
    }    
}

void DRAM_t::replay(){
    std::string command;    
    int cycleCount = 0;
    while (getline(commandFile, command)){
        //std::cout  << "cycle " << cycles << "\n";
        cycles++;
        cycleCount++;   
        if (strcmp("Load", command.c_str()) == 0){
            load_index();
        } else if (strcmp("NextCompute", command.c_str()) == 0){
            cycle(1);
        } else if (strcmp("Compute", command.c_str()) == 0){
            cycle(0);
        } else {
            std::cerr << "Error " << "\n";
        }
    }
    std::cout  << "Finihsed in " << cycleCount << "\n";
}

int main (int argc, char ** argv){
    if (argc < 3){
        return 0;
    }   

    //std::cout  << argv[3] << "\n";
    DRAM_t * dram = new DRAM_t(argv[1], argv[2]);
    dram -> replay();
    std::cout << "Total computen " << compute_count << " " << block_count << "\n";
    for (int i = 0; i < num_multis; i++){
       // std::cout << dram->multis[i]->sum[0] << " " << dram->multis[i]->sum[1] << "\n";
    }
    return 0;
}