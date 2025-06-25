#include <iostream>
#include <vector>
using namespace std;


struct Node {
    int index;
    vector<Node*> adjacents;
};

typedef vector<vector<int>> Matrix;

class Graph {
    private:
        vector<Node*> nodes;
        Matrix matrix;
    
    public:
        Graph(vector<Node*> nodes) {
            this->nodes = nodes;
            int nodes_size = nodes.size();
            matrix = init_matrix(nodes_size);
        }

        Matrix init_matrix(int nodes_size){
            vector<vector<int>> matrix(nodes_size, vector<int>(nodes_size, 0));
            return matrix;
        }

        void maps_graph(){
            cout << "node: " << nodes[0]->index << endl;
            for(Node* node: nodes){
                for(Node* adjt: node->adjacents){
                    matrix[node->index - 1][adjt->index - 1] = 1;
                    matrix[adjt->index - 1][node->index - 1] = 1;
                }
            }
        }

        void show_graph(){
            for(vector<int> line : this->matrix){
                cout << "[ " ;
                for(int node: line){
                    cout << node << " ";
                }
                cout << "]" << endl;
            }
        }

        void DFS(){

        }

        void BFS(){

        }

};

int main(){
    Node a, b, c, d, e;
    a.index = 1;
    b.index = 2;
    c.index = 3;
    d.index = 4;
    e.index = 5;

    a.adjacents = { &b, &c };
    b.adjacents = { &a };
    c.adjacents = { &a };
    d.adjacents = { &e };
    e.adjacents = { &d };

    vector<Node*> nodes = { &a, &b, &c, &d, &e };
    Graph graph(nodes);
    graph.maps_graph();
    graph.show_graph();

    return 0;
}