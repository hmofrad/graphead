/*
 * pr.cpp: PageRank benchmark main
 * (c) Mohammad Hasanzadeh Mofrad, 2019
 * (e) m.hasanzadeh.mofrad@gmail.com 
 */

#include <iostream>
#include <unistd.h>
 
#include "mpi/env.hpp"
#include "mat/graph.hpp"

#include "pr.h"

int main(int argc, char** argv) {
    Env::init();    
    double time1 = Env::clock();
    if(argc != 3 and argc != 4) {
        if(Env::is_master)
            std::cout << "\"Usage: " << argv[0] << " <file_path> <num_vertices> [<num_iterations=INF>]\"" << std::endl;
        Env::exit(1);
    }
    std::string file_path = argv[1]; 
    ip num_vertices = std::atoi(argv[2]);
    ip num_iterations = (argc > 3) ? (uint32_t) atoi(argv[3]) : 0;
    bool directed = true;
    bool transpose = false;
    bool self_loops = true;
    bool acyclic = false;
    bool parallel_edges = true;
    Tiling_type TT = _NUMA_;
    Compression_type CT = _TCSC_CF_;
    Hashing_type HT = _BUCKET_;
    /* Degree execution */
    Graph<wp, ip, fp> G;    
    G.load(file_path, num_vertices, num_vertices, directed, transpose, self_loops, acyclic, parallel_edges, TT, CT, HT);
    
    bool stationary = true;
    bool gather_depends_on_apply = false;
    bool apply_depends_on_iter  = false;


    
    Ordering_type OT = _ROW_;
    Deg_Program<wp, ip, fp> V(G, stationary, gather_depends_on_apply, apply_depends_on_iter, OT);
    V.execute(1);
    V.checksum();
    
    OT = _ROW_;
    PR_Program<wp, ip, fp> VR(G, stationary, gather_depends_on_apply, apply_depends_on_iter, OT);
    VR.initialize(V);
    V.free();
    VR.execute(num_iterations); // Vertex execution
    VR.checksum();
    VR.display();
    VR.free();
    G.free();
    
/*    
    Deg_Program<wp, ip, fp> V(G, stationary, gather_depends_on_apply, apply_depends_on_iter);
    V.execute(1);
    V.checksum();    
    G.free_mat();
    Env::barrier();

    transpose = true;
    Graph<wp, ip, fp> GR;    
    GR.load(file_path, num_vertices, num_vertices, directed, transpose, self_loops, acyclic, parallel_edges, TT, CT, HT);
    PR_Program<wp, ip, fp> VR(GR, stationary, gather_depends_on_apply, apply_depends_on_iter);
    VR.initialize(V);
    V.free();
    G.free_graph();
    VR.execute(num_iterations); // Vertex execution
    VR.checksum();
    VR.display();
    VR.free();
    GR.free();    
*/    
    double time2 = Env::clock();
    Env::print_time("PageRank end-to-end", time2 - time1);
    Env::finalize();
    return(0);
}
