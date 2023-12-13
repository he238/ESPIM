#include "replay.hh"

int main (int argc, char ** argv){
    if (argc < 5){
        return 0;
    }
    int m = atoi(argv[1]); 
    int n = atoi(argv[2]);
 
    std::cout << argv[3] << "\n";
    DRAM_t * dram = new DRAM_t(argv[3], argv[4]);
    dram -> replay();

    return 0;
}