/*
 * cc.cpp: Connected Component (CC) benchmark main
 * (c) Mohammad Hasanzadeh Mofrad, 2019
 * (e) m.hasanzadeh.mofrad@gmail.com 
 */

#include <iostream>
#include <unistd.h>
 
#include "mpi/env.hpp"
#include "mat/graph.hpp"

#include "cc.h"

int main(int argc, char** argv) { 
    Env::init();    
    double time1 = Env::clock();   
    if(argc != 3 and argc != 4) {
        if(Env::is_master)
            std::cout << "\"Usage: " << argv[0] << " <file_path> <num_vertices>\"" << std::endl;
        Env::exit(1);
    }
    std::string file_path = argv[1]; 
    ip num_vertices = std::atoi(argv[2]);
    bool directed = false;
    bool transpose = false;
    bool self_loops = true;
    bool acyclic = false;
    bool parallel_edges = false;
    Tiling_type TT = _NUMA_;
    Compression_type CT = _TCSC_;
    Hashing_type HT = _BUCKET_;
    /* Connected component execution */
    bool stationary = false;
    // Engine requirement for nonstationary algorithms on directed graphs
    if(not stationary and directed)
        transpose = not transpose; 
    Graph<wp, ip, fp> G;    
    G.load(file_path, num_vertices, num_vertices, directed, transpose, self_loops, acyclic, parallel_edges, TT, CT, HT);
    bool gather_depends_on_apply = true;
    bool gather_depends_on_iter  = false;
    CC_Program<wp, ip, fp> V(G, stationary, gather_depends_on_apply, gather_depends_on_iter);   
    V.execute();
    V.checksum();
    V.display();
    V.free();
    G.free();

    double time2 = Env::clock();
    Env::print_time("Connected component end-to-end", time2 - time1);
    Env::finalize();
    return(0);
}
