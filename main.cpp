#include "graph/Graph.h"

int main() {
    Matrix matrix = {
        {0, 1, 0, 1, 1},
        {1, 0, 1, 0, 0},
        {0, 1, 0, 1, 0},
        {1, 0, 1, 0, 0},
        {1, 0, 0, 0, 0},
    };
    Graph graph;
    graph.insert_from_matrix(matrix);
    graph.show_matrix_representation();
    graph.DFS(0);
    graph.DFS_Cycle(0);
    graph.BFS(4, 0);
    return 0;
}
