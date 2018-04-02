#include "declarations.h"

int main()
{
    Read_Graph Reader;
    Reader.processing();

    Main_Graph Graph;
    Graph.adjacency_matrix();
    Graph.is_adjoint();
    Graph.is_multi();
    Graph.is_linear();
    Graph.save_graph();

    return 0;
}