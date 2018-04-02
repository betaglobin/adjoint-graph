#include "declarations.h"

using std::string;

void Read_Graph::processing()
{
    preprocessing();
    read_columns();
}


void Read_Graph::read_columns()
{
    //the following method reads how many rows and columns in each row are in the file
    int col_count = 1;
    int row_count = 1;
    int check;
    char preproc_name[] = "preprocess.txt";
    char temp_name[] = "temp.txt";

    FILE *read_file;
    FILE *save_file;

    read_file = fopen(preproc_name, "r");
    save_file = fopen(temp_name, "w");

    while ((check = getc(read_file)) != EOF) {
        if (check == ' '){ //space as a separator
            ++col_count;
        }

        if (check == '\n') { //once its ended...
            fprintf(save_file, "%d", col_count); //..save number of columns in seperate file...
            ++row_count; //...and increment row counter
            fprintf(save_file, "\n");
            col_count = 1;
        }
    }
    fprintf(save_file, "%d", col_count);

    fclose(read_file);
    fclose(save_file);
}

void Read_Graph::preprocessing()
{
    //re-write a file, without a colon, to the new one
    while(true) {
        std::cout << "Provide a name of a graph-containing file:" << std::endl;
        std::cin >> file_name;
        std::cout << "Loading...\n";

        char preproc_name[] = "preprocess.txt";
        int check;

        FILE *read_file;
        FILE *save_file;

        read_file = fopen(file_name, "r");
        save_file = fopen(preproc_name, "w");

        if (read_file != NULL) {

            while ((check = fgetc(read_file)) != EOF) {
                if (check == ':') { //simply skip the colon...
                    continue;
                } else {
                    fputc(check, save_file); //and re-write everything to preprocess.txt
                }
            }
            fclose(save_file);
            fclose(read_file);

            int temp = 0;
            int current = 0;

            save_file = fopen(preproc_name, "r");

            while (!feof(save_file)) {
                fscanf(save_file, "%d", &current);
                if (temp < current)
                    temp = current;
            }

            fprintf(save_file, "\b");
            fclose(save_file);
            std::cout<< temp<<"TO NAJWIEKSZA WARTOSC"<<std::endl;

            break;
        } else if (read_file == NULL)
            std::cout << "Such file doesn't exist! Please provide a name of the file again..." << std::endl;

    }
}

void Main_Graph::initialize_matrix()
{
    int i, j;
    for (i = 0; i < SIZE; i++) {
        for(j = 0; j < SIZE; j++) {
            adjac_matrix[i][j] = 0;
        }
    }

    for (i = 0; i < SIZE*2; i++) {
        for(j = 0; j < SIZE*2; j++) {
            orig_matrix[i][j] = 0;
        }
    }
}

void Main_Graph::initialize_array(int array[], int n)
{
    int i;
    for (i = 0; i < n; i++)
            array[i] = 0;
}


void Main_Graph::print_adjac()
{
    int i, j;
    for (i = 1; i < SIZE; i++) {
        for (j = 1; j < SIZE; j++) {
            std::cout << adjac_matrix[i][j] << " ";
        }
        std::cout << "\n";
    }
}


void Main_Graph::print_original()
{
    int i, j;
    for (i = 1; i < SIZE*2; i++) {
        for (j = 1; j < SIZE*2; j++) {
            std::cout << orig_matrix[i][j] << " ";
        }
        std::cout << "\n";
    }
}


void Main_Graph::adjacency_matrix()
{
    initialize_matrix();
    generate_comb(); //PREPARING STUFF FOR FURTHER METHODS

    FILE *temp_fp;
    char temp_name[] = "temp.txt";
    temp_fp = fopen(temp_name, "r");

    //READ NUMBER OF COLUMNS IN EACH ROW FROM TEMP.TXT AND PUT THEM IN THE QUEUE
    std::queue <int> queue_columns;

    int val;
    while (!feof(temp_fp))
    {
        fscanf(temp_fp, "%d", &val);
        queue_columns.push(val);

    }
    fclose(temp_fp);

    FILE *prep_fp;
    char prep_name[] = "preprocess.txt";
    prep_fp = fopen(prep_name, "r");

    //READ EVERYTHING FROM PREPROCESS.TXT AND PUT IT IN THE QUEUE
    std::queue<int> queue_graph;

    int all;
    while (!feof(prep_fp)) {
        fscanf(prep_fp, "%d", &all);
        queue_graph.push(all);
    }
    fclose(prep_fp);

    //TAKE A VALUE (NO. OF COLUMNS IN n ROW) FROM QUEUE AND USE IT
    int num_col;
    int vertex_in, vertex_out;


    while (!queue_columns.empty()) { //repeat this until queue is empty and you're done

        num_col = queue_columns.front(); //this bits reads the number of column in a row
        vertex_out = queue_graph.front(); //this guy represents the first vertex (OUT)

        int i;
        for (i = 1; i < num_col; i++) //repeat this number-of-columns times
        {
            queue_graph.pop(); //remove last vertex
            vertex_in = queue_graph.front(); //and read the next one
            adjac_matrix[vertex_out][vertex_in] += 1; //put 1 in following coordinates; YES, arc exists
        }
        queue_columns.pop();
        queue_graph.pop();
    }

    int answer;
    std::cout << "Adjacency matrix is now ready." << std::endl;
    std::cout << "Do you want to print it?" << std::endl;
    std::cout << "YES (1) / NO (2)" << std::endl;

    while (true) {

        std::cin >> answer;

        if (answer == 1) {
            print_adjac();
            break;
        } else if (answer != 2)
            std::cout << "Wrong answer! Choose number: YES (1) / NO (2)" << std::endl;

        else if (answer == 2)
            break;
    }

}


void Main_Graph::save_graph()
{
    if (adjont == 1) {
        convert();

        int answer;
        std::cout << "Convertion of the adjont graph to original graph has been acomplished." << std::endl;
        std::cout << "Do you want to print adjacency matrix of the original graph?" << std::endl;
        std::cout << "YES (1) / NO (2)" << std::endl;

        while (true) {

            std::cin >> answer;

            if (answer == 1) {
                print_original();
                break;
            } else if (answer != 2)
                std::cout << "Wrong answer! Choose number: YES (1) / NO (2)" << std::endl;

            else if (answer == 2)
                break;
        }

        int i, j, vertex;
        FILE *fp;
        char saved_file[] = "saved_graph.txt";

        fp = fopen(saved_file, "w");

        for (i = 1; i < SIZE*2; i++) {
            int printed = 0;
            for (int z = 1; z < SIZE*2; z++){
                if (orig_matrix[i][z] > 0) {
                    fprintf(fp, "%d: ", i);
                    printed = 1;
                    break;
                }

            }

            for (j = 1; j < SIZE*2; j++) {
                vertex = orig_matrix[i][j];
                if (vertex == 1)
                    fprintf(fp, "%d ", j);
                else if (vertex > 1){
                    for (int z = 0; z < vertex; z++) {
                        fprintf(fp, "%d ", j);
                    }
                }

                if (j == SIZE*2 - 1 && printed == 1)
                    fprintf(fp, "\n");
            }
        }
        fclose(fp);
    }
    else if (adjont == 0){
        std::cout<<"\n"<<std::endl;
        std::cout<<"Provided graph is not adjont!\nIts converstion to the original graph can't be done."<<std::endl;
    }

    cleaning();
}



void Main_Graph::cleaning()
{
    remove("temp.txt");
    remove("preprocess.txt");
}



void Main_Graph::generate_comb() //GENERATE 2-COMBINATION FROM N-ELEMENT SET
{
    int i, j;
    for (i = 1; i < SIZE; i++) {
        for (j = 1; j < (SIZE-1); j++) {
            if (i > 1 && j + 1 < i)
                continue;

            else if (j + 1 <= (SIZE-1) && i != j + 1) {
                queue_1_pair.push(i);
                queue_2_pair.push((j+1));
            }
        }
    }
}


void Main_Graph::is_multi()
{
    std::cout<<"1-GRAPH TEST\n============="<<std::endl;
    int i, j;
    int check = 1;
    for (i = 0; i < SIZE; i++) {
        for (j = 0; j < SIZE; j++) {
            if (adjac_matrix[i][j] > 1) {
                std::cout << "Provided graph G IS NOT 1-graph.\n" << std::endl;
                check = 0;
                lin_test = 0;
                break;
            }
        }
        if (check == 0)
            break;
    }

    if (check == 1) {
        std::cout << "Provided graph G IS 1-graph.\n" << std::endl;
        lin_test = 1;
    }
}

void Main_Graph::is_adjoint()
{
    int vertex_1, vertex_2;
    bool condition = true;

    mirr_test = 1; //initialization

    while (condition)
    {
        //READ THE FIRST PAIR FROM QUEUES STORING 2-COMBINATION OF VERTEXES
        vertex_1 = queue_1_pair.front();
        vertex_2 = queue_2_pair.front();
        qu_1_pair_linear.push(vertex_1); //prepare pairs of vertexes for linearity test
        qu_2_pair_linear.push(vertex_2);

        initialize_array(out_neighs_1, SIZE);
        initialize_array(out_neighs_2, SIZE);

        //GO TO MATRIX[X][...] AND MATRIX[Y][...]
        int i;

                for (i = 1; i < SIZE; i++) {
                    if(adjac_matrix[vertex_1][i] != 0)
                        out_neighs_1[i] = 1;  //array for storing out-neighbours of vertex
                    else
                        continue;
                }

                for (i = 1; i < SIZE; i++) {
                    if(adjac_matrix[vertex_2][i] != 0)
                        out_neighs_2[i] = 1; //array for storing out-neighbours of vertex
                    else
                        continue;
                }

        //COMPARE OUT-NEIGHBOURS 1. IF THE INTERSECTION IS EMPTY?
        // 2. IF IS EMPTY, THEN CHECK IF SETS ARE THE SAME
        //auxiliary variables for checking if
        int check = 0;                        // the intersection of the out-neighbours sets is empty
        int mirror = 1;                       // the sets of the out-neighbours are equal

            for (i = 0; i < SIZE; i++) {
                if (out_neighs_1[i] == out_neighs_2[i] && (out_neighs_1[i] == 1 || out_neighs_2[i] == 1)) {
                    check = 1; //the intersection is not empty
                    break;
                }
                else
                    check = 0;
            }

            if (check == 1) { //if the intersection is not empty then it MUST be checked
                for (i = 0; i < SIZE; i++) {
                    if (out_neighs_1[i] != out_neighs_2[i]) {
                        mirror = 0;
                        break; //if at least one is not the same, then WHOLE out-neighbour sets are not the same
                    }
                    else {
                        mirror = 1;
                    }
                }
            }

        queue_1_pair.pop();
        queue_2_pair.pop();

        if (queue_1_pair.empty()) {
            condition = false;
            mirr_test = 1;
        }

        if (check == 1 && mirror == 0) { // (p ==> q) = 0          iff p = 1 and q = 0
            condition = false;
            mirr_test = 0;
        }
    }

    std::cout<<"\nADJONT TEST\n==========="<<std::endl;
    if (mirr_test == 1) {
        std::cout << "Provided graph G IS adjoint.\n" << std::endl;
        adjont = 1;
    }

    else if (mirr_test == 0) {
        std::cout << "Provided graph G IS NOT adjoint.\n" << std::endl;
        adjont = 0;
    }
}


void Main_Graph::is_linear()
{
    int vertex_1, vertex_2;
    bool condition = true;

    mirr_test = 1; //initialization
    lin_test = 1;

    while (condition) {
        //READ THE FIRST PAIR FROM QUEUES STORING 2-COMBINATION OF VERTEXES
        vertex_1 = qu_1_pair_linear.front();
        vertex_2 = qu_2_pair_linear.front();

        int i;
        initialize_array(in_neighs_1, SIZE);
        initialize_array(in_neighs_2, SIZE);
        initialize_array(out_neighs_1, SIZE);
        initialize_array(out_neighs_2, SIZE);

        //LOAD OUT-NEIGHBOURS FOR THAT PAIR OF VERTEXES
        for (i = 1; i < SIZE; i++) {
            if (adjac_matrix[vertex_1][i] != 0)
                out_neighs_1[i] = 1;
            else
                continue;
        }

        for (i = 1; i < SIZE; i++) {
            if (adjac_matrix[vertex_2][i] != 0)
                out_neighs_2[i] = 1;
            else
                continue;
        }

        //LOAD THEIR IN-NEIGHBOURS

        for (i = 1; i < SIZE; i++) {
            if (adjac_matrix[i][vertex_1] != 0)
                in_neighs_1[i] = 1;
            else
                continue;
        }

        for (i = 1; i < SIZE; i++) {
            if (adjac_matrix[i][vertex_2] != 0)
                in_neighs_2[i] = 1;
            else
                continue;
        }

        //COMPARE OUT-NEIGHBOURS
        // 1. IF THE INTERSECTION IS EMPTY?
        // 2. IF IT'S NOT EMPTY, THEN CHECK IF
        // OUT-NEIGHBOURS ARE THE SAME AND
        // SETS OF IN-NEIGHBOURS ARE NOT EMPTY AND ARE THE SAME
        // 2.' IF IS EMPTY, DO NOTHING (TRUE)

        //auxiliary variables for checking if
        int out_check = 0;              //the intersection of the out-neighbours sets is empty
        int out_mirror = 1;             //the of the out-neighbours are equal
        int in_check = 1;               //the intersection of the in-neighbours sets is empty

        for (i = 1; i < SIZE; i++) {
            if ((out_neighs_1[i] == out_neighs_2[i]) && ((out_neighs_1[i]) > 0)) {
                out_check = 1; //the intersection is not empty
                break;
            } else
                out_check = 0;
        }

        if (out_check == 1) { //if the intersection is not empty then it MUST be checked
            for (i = 1; i < SIZE; i++) {
                if ((out_neighs_1[i] != out_neighs_2[i]) && ((out_neighs_1[i]) > 0)) {
                    out_mirror = 0;
                    break; //if at least one is not the same, then the WHOLE out-neighbour sets are not the same
                } else if ((out_neighs_1[i] == out_neighs_2[i]) && ((out_neighs_1[i]) > 0))
                    out_mirror = 1;
            }
        }


        if (out_check == 1) {
            for (i = 1; i < SIZE; i++) {
                if (in_neighs_1[i] == in_neighs_2[i] && (in_neighs_2[i] == 1)) {
                    in_check = 0;   //the intersection is not empty, but it should be for being linear
                    break;
                } else
                    in_check = 1;
            }
        }

        qu_1_pair_linear.pop();
        qu_2_pair_linear.pop();

        if (qu_1_pair_linear.empty()) {
            condition = false;
            lin_test = 1;
        }

        if (out_check == 1 && (out_mirror == 0 && in_check == 1)) {
            condition = false;
            lin_test = 0;
        }

        if (out_check == 1 && (out_mirror == 1 && in_check == 0)) {
            condition = false;
            lin_test = 0;
        }

        if (out_check == 1 && (out_mirror == 0 && in_check == 0)) {
            condition = false;
            lin_test = 0;
        }

    }

    std::cout<<"LINEARITY TEST\n==============="<<std::endl;
    if (lin_test == 1)
        std::cout<<"Provided graph G IS linear.\n"<<std::endl;

    else if (lin_test == 0)
        std::cout<<"Provided graph G IS NOT linear.\n"<<std::endl;
}


void Main_Graph::convert() {
    int i, j;
    for (i = 0; i < 1; i++) {
        for (j = 0; j < 3; j++) {
            arcs_vertexes[i][j] = 0; //initialization

        }
    }

    for (i = 0; i < SIZE*2; i++) {
        for (j = 0; j < 3; j++) {
            visited[i][j] = 0; //initialization

        }
    }

    for (i = 1; i < SIZE*2;i++) {
        visited[i][2] = i;
    }

    int index = 1;
    for (i = 1; i < SIZE; i++) {
        for (j = 0; j < 3; j++) {
            if (j == 0) {
                arcs_vertexes[i][j] = index;
                index++;
            }
            if (j == 1) {
                arcs_vertexes[i][j] = index; //initialization
                index++;
            }

            if (j == 2)
                arcs_vertexes[i][j] = i; //initialization
        }
    }

    /*                                                         // UNLOCK FOR PRINTING
    for (i = 0; i < SIZE; i++) {
        for (j = 0; j < 3; j++) {
            std::cout << arcs_vertexes[i][j] << "  ";
        }
        std::cout << "\n";
    }
    */

    // ========================= END OF INITIALIZATION-RELATED BITS OF CODE

    for (i = 1; i < SIZE; i++) {
        int arc = arcs_vertexes[i][2]; //take an arc from the increasing-order list and save it's index
        int is_loop;
        is_loop = go_deeper(arc); // check whether a vertex with arc's index has any out-neighbours AND THE LOOP

        if (buffer.empty()) { //if it doesn't have any, skip the iteration
            continue;
        }

        int check = 0;
        while (!buffer.empty()) { //if it does have, then take the out-neighbour
            int x = buffer.front();

            if (is_loop == 1 && check == 0){
                arcs_vertexes[arc][1] = arcs_vertexes[arc][0]; // if loop exist then make this move
                check = 1;
                visited[arc][0] = 1;
            }

            if (visited[x][0] == 1){
                arcs_vertexes[arc][1] = arcs_vertexes[x][0];
            }

            if (visited[x][0] == 0) {
                arcs_vertexes[x][0] = arcs_vertexes[arc][1]; //...and make a switch
                visited[x][0] = 1;

            }
            buffer.pop();

            }
        }


                                                           //UNLOCK FOR PRINTING
    std::cout<<"="<<std::endl;
    for (i = 0; i < SIZE; i++){
        for (j = 0; j < 3; j++){
            std::cout<<arcs_vertexes[i][j]<<"  ";
        }
        std::cout<<"\n";
    }


    for (i = 1; i < SIZE; i++){
        orig_matrix[arcs_vertexes[i][0]][arcs_vertexes[i][1]] += 1;
    }

}

int Main_Graph::go_deeper(int v)
{
    //check if provided vertex has any out-neigbhours
    int check = 0;
    for (int i = 1; i < SIZE; i++){
        if (adjac_matrix[v][i] == 1){
            if (i == v){
                check = 1;
            }
            buffer.push(i); //stores out-neigbours of the vertex
        }
    }
    if (check == 0)
        return 0;
    else if(check == 1)
        return 1;
}