#include <iostream>
#include <vector>
#include <limits.h>
using namespace std;

enum class Color
{
    White,
    Gray,
    Black
};

struct Vertex
{
    int index;
    Color color;
    Vertex* ancester;
    vector<Vertex*> adjacents;
    int discovered;
    int finalized;
};

typedef vector<vector<int>> Matrix;
typedef vector<Vertex*> Queue;

class Graph
{
    private:
        vector<Vertex*> vertices;
        Matrix matrix;
    
    public:
        Graph(){}

        void insert_from_vertices(vector<Vertex*> incomming_vertices){
            vertices = incomming_vertices;
            matrix = init_matrix(vertices.size());
            maps_matrix();
        }

        void insert_from_matrix(Matrix incomming_matrix){
            matrix = incomming_matrix;
            maps_vertices(matrix);
        }

        string color_to_string(Color c)
        {
            switch (c)
            {
                case Color::White:
                    return "White";
                
                case Color::Gray:
                    return "Gray ";
                
                case Color::Black:
                    return "Black";
                default:
                    return "White";
            }
        }

        Matrix init_matrix(int vertices_size)
        {
            vector<vector<int>> matrix(vertices_size, vector<int>(vertices_size, 0));
            return matrix;
        }

        void maps_matrix()
        {
            for(Vertex *vertex: vertices)
            {
                for(Vertex* adjt: vertex->adjacents)
                {
                    matrix[vertex->index][adjt->index] = 1;
                    matrix[adjt->index][vertex->index] = 1;
                }
            }
        }

        void maps_vertices(Matrix m) {
            // Primeiro, criar todos os nós e armazená-los no heap
            vertices.clear();  // Limpa qualquer conteúdo anterior
            for (size_t i = 0; i < m.size(); i++) {
                Vertex* vertex = new Vertex();  // Aloca no heap
                vertex->index = i;
                vertex->color = Color::White;
                vertex->ancester = nullptr;
                vertex->discovered = 0;
                vertex->finalized = 0;
                vertices.push_back(vertex);
            }

            // Depois, preencher as adjacências
            for (size_t i = 0; i < m.size(); i++) {
                for (size_t j = 0; j < m[i].size(); j++) {
                    if (m[i][j] == 1) {
                        vertices[i]->adjacents.push_back(vertices[j]);
                    }
                }
            }
        }

        void show_matrix_representation()
        {
            cout << "Matrix representation..." << endl;
            int count = 0;
            for(vector<int> line : this->matrix)
            {
                if (count == 0){
                    cout << "   ";
                    for (size_t i = 0; i < line.size(); i++)
                    {
                        cout << i << " ";
                    }
                    cout << "\n";
                }
                cout << count << "|" << " " ;
                for(int vertex: line)
                {
                    cout << vertex << " ";
                }
                cout << "\n";
                count++;
            }
            cout << endl;
        }

        void show_vertex(Vertex* vertex)
        {
            cout << "vertex: (" << vertex->index << ") | ";
            cout << "Color: " << color_to_string(vertex->color) << " | ";
            cout << "Discovered: " << vertex->discovered << " | ";
            cout << "Final: " << vertex->finalized << " | ";
            if (vertex->ancester != nullptr){
                cout << "Ancester: " << (vertex->ancester)->index << " |";
            } else {
                cout << "Ancester: Null |";
            }
            string linha(82, '-');
            cout << "\n" << linha << endl;
        }

        void DFS_Visit(Vertex* vertex, int &time)
        {
            vertex->color = Color::Gray;
            time++;
            vertex->discovered = time;
            for(Vertex* adj: vertex->adjacents)
            {
                if (adj->color == Color::White)
                {
                    adj->ancester = vertex;
                    DFS_Visit(adj, time);
                }
            }
            vertex->color = Color::Black;
            vertex->finalized = ++time;
            show_vertex(vertex);
        }

        void DFS()
        {
            cout << "Starting DFS..." << endl;
            cout << "Setting every adjacent..." << endl;
            for (Vertex* vertex: vertices)
            {
                vertex->color = Color::White;
                vertex->ancester = NULL;
            }
            
            int time = 0;
            
            for (Vertex* vertex : vertices)
            {
                if (vertex->color == Color::White)
                    DFS_Visit(vertex, time);
            }
            cout << "DFS finished...\n" << endl;
        }

        vector<int> BFS_backtrack(Vertex* vertex, vector<int> path_vector){
            if (vertex->ancester == nullptr)
            {
                path_vector.push_back(vertex->index);
                return path_vector;
            }
            path_vector.push_back(vertex->index);
            return BFS_backtrack(vertex->ancester, path_vector);
        }

        void BFS(int starting_vertex, int ending_vertex)
        {
            vector<int> path_vector;
            Vertex* n = vertices[starting_vertex];
            cout << "Starting BFS..." << endl;
            
            
            cout << "Setting every adjacent..." << endl;
            for (Vertex* adj : n->adjacents)
            {
                adj->color = Color::White;
                adj->discovered = INT_MAX;
                adj->ancester = nullptr;
            }
            
            n->color = Color::Gray;
            n->discovered = 0;
            n->ancester = nullptr;
            cout << "Initial ";
            show_vertex(n);

            cout << "Populating queue with initial vertex..." << endl;
            Queue queue = {n};

            while (queue.size() != 0)
            {
                Vertex* vertex = queue.front();
                // desenfileira
                queue.erase(queue.begin());
                for (Vertex* adj : vertex->adjacents)
                {
                    if (adj->color == Color::White)
                    {
                        cout << "Visiting ";
                        adj->color = Color::Gray;
                        adj->discovered = vertex->discovered + 1;
                        adj->ancester = vertex;
                        show_vertex(adj);

                        // Se encontrarmos o caminho de a até b...
                        if (adj->index == ending_vertex){
                            path_vector = BFS_backtrack(adj, path_vector);
                        }
                        // enfileira
                        queue.push_back(adj);
                    }
                }

                if (!queue.empty()) {
                    cout << "Queue: ";
                    for (Vertex* a : queue) {
                        cout << a->index << " ";
                    }
                    cout << "\n" << endl;
                } else {
                    cout << "Queue is empty." << endl;
                }

                vertex->color = Color::Black;
                cout << "*Back to ";
                show_vertex(vertex);
            }

            cout << "PATH TO ENDING NODE FOUNDED: ";
            for(int path: path_vector) {
                if (path_vector.back() != path){
                    cout << path << " <- ";
                } else {
                    cout << path << endl;
                }
            }

            cout << "BFS finished...\n" << endl;   
        }
        
        ~Graph() {
            for (Vertex* vertex : vertices) {
                delete vertex;
            }
        }
};

vector<Vertex*> ask_adjacents(Vertex &n, size_t available_vertices, vector<Vertex*> vertices)
{
    cout << "Quantos adjacentes o vértice " << n.index << " possui?\n-> ";
    int num_adjacents;
    cin >> num_adjacents;
    try
    {
        if (num_adjacents >= 0 || num_adjacents <= available_vertices - 1)
        {
            cout << "Agora forneça ao vértice, seus adjacentes (iterativamente): " << endl;
        } else {
            throw (num_adjacents);
        }
    } catch (int myNum) {
        cout << "Número maior ou menor do que o número de vértices disponíveis (" << available_vertices << ")\nNúmero inserido: " << myNum << endl;
    }
    
    vector<Vertex*> adjacents;

    for (size_t i = 0; i < num_adjacents; i++)
    {
        cout << i + 1 << "º adjacente: ";
        int choice;
        cin >> choice;
        try
        {
            if (choice >= 0 || choice <= available_vertices - 1)
            {
                adjacents.push_back(vertices[choice]);
                continue;
            } else {
                throw (num_adjacents);
            }
        } catch (int myNum) {
            cout << "Número maior ou menor do que o número de vértices disponíveis (" << available_vertices << ")\nNúmero inserido: " << myNum << endl;
        }
    }
    cout << endl;
    return adjacents;
}

int main() {
    Matrix matrix = {
    //   0  1  2  3  4  5
        {0, 1, 1, 1, 0, 0}, // 0
        {1, 0, 1, 0, 0, 1}, // 1
        {1, 1, 0, 0, 0, 0}, // 2
        {1, 0, 0, 0, 1, 0}, // 3
        {0, 0, 0, 1, 0, 1}, // 4
        {0, 1, 0, 0, 1, 0}  // 5
    };
    // Vertex a, b, c, d, e, f;
    // vector<Vertex*> vertices = { &a, &b, &c, &d, &e, &f};
    // int vertices_size = vertices.size();
    // for (size_t i = 0; i < vertices_size; i++)
    // {
    //     vertices[i]->index = i;
    //     vertices[i]->color = Color::White;
        
    //     vector<Vertex*> adjacents = ask_adjacents(*vertices[i], vertices_size, vertices);
    //     for (Vertex* adj : adjacents){
    //         vertices[i]->adjacents.push_back(adj);
    //     }
    // }

    Graph graph;
    // graph.maps_matrix();
    graph.insert_from_matrix(matrix);
    graph.show_matrix_representation();
    // graph.DFS();
    graph.BFS(5, 0);


    return 0;
}
