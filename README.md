# Graphite
   Graphite HPC Graph Analytics System 
## Install
    make
## Unistall
    make clean
## Usage
    mpirun -np Nprocesses bin/./deg   Graph.[bin/txt]   Nvertices
    mpirun -np Nprocesses bin/./pr    Graph.[bin/txt]   Nvertices [Niterations]
    mpirun -np Nprocesses bin/./sssp  GraphW.[bin/txt]  Nvertices
    mpirun -np Nprocesses bin/./bfs   Graph.[bin/txt]   Nvertices
    mpirun -np Nprocesses bin/./cc    Graph.[bin/txt]   Nvertices
## Contact
    Mohammad Hasanzadeh Mofrad
    m.hasanzadeh.mofrad@gmail.com
