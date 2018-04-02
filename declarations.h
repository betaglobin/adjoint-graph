#ifndef METHODS_H_
#define METHODS_H_

#include <queue>
#include <iostream>
#include <fstream>
#define SIZE 24


class Read_Graph
{
private:
    char file_name[20];
    void preprocessing();
    void read_columns();

public:
    void processing();
};

class Main_Graph
{
private:
    int adjac_matrix[SIZE][SIZE], orig_matrix[SIZE*2][SIZE*2];
    int out_neighs_1[SIZE], in_neighs_1[SIZE];
    int out_neighs_2[SIZE], in_neighs_2[SIZE];
    int arcs_vertexes[SIZE][3];
    int mirr_test, lin_test, adjont;
    std::queue <int> queue_1_pair;
    std::queue <int> queue_2_pair;
    std::queue <int> qu_1_pair_linear;
    std::queue <int> qu_2_pair_linear;
    std::queue <int> buffer;
    int visited[SIZE*2][3];
    //int corr_visited[SIZE][3];


    int go_deeper(int v);
    void initialize_matrix();
    void initialize_array(int array[], int n);
    void convert();
    void cleaning();
    void generate_comb();
    void print_original();


public:
    void adjacency_matrix();
    void is_multi();
    void is_adjoint();
    void is_linear();
    void save_graph();
    void print_adjac();

};

#endif
