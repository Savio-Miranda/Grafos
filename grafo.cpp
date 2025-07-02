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

struct Node
{
    int index;
    Color color;
    Node* ancester;
    vector<Node*> adjacents;
    int discovered;
    int finalized;
};

typedef vector<vector<int>> Matrix;
typedef vector<Node*> Queue;

class Graph
{
    private:
        vector<Node*> nodes;
        Matrix matrix;
    
    public:
        Graph(vector<Node*> nodes)
        {
            this->nodes = nodes;
            int nodes_size = nodes.size();
            matrix = init_matrix(nodes_size);
        }

        string color_to_string(Color c)
        {
            switch (c)
            {
                case Color::White:
                    return "White";
                
                case Color::Gray:
                    return "Gray";
                
                case Color::Black:
                    return "Black";
                default:
                    return "White";
            }
        }

        Matrix init_matrix(int nodes_size)
        {
            vector<vector<int>> matrix(nodes_size, vector<int>(nodes_size, 0));
            return matrix;
        }

        void maps_matrix()
        {
            for(Node *node: nodes)
            {
                for(Node* adjt: node->adjacents)
                {
                    matrix[node->index][adjt->index] = 1;
                    matrix[adjt->index][node->index] = 1;
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
                for(int node: line)
                {
                    cout << node << " ";
                }
                cout << "\n";
                count++;
            }
            cout << endl;
        }

        // void show_nodes_representation()
        // {
        //     cout << "--------------";
        //     for(Node* node : nodes)
        //     {
        //         cout << "Index: " << node->index << endl;
        //         cout << "Color: " << color_to_string(node->color) << endl;
        //         if (node->ancester == nullptr){
        //             cout << "Ancester: " << "Null" << endl;
        //         } else {
        //             cout << "Ancester: " << (node->ancester)->index << endl;
        //         }
        //         cout << "Discovered: " << node->discovered << endl;
        //         cout << "Finalized: " << node->finalized << endl;
        //         cout << "--------------";
        //     }
        // }

        void DFS_Visit(Node* node, int &time)
        {
            node->color = Color::Gray;
            time++;
            node->discovered = time;
            for(Node* adj: node->adjacents)
            {
                if (adj->color == Color::White)
                {
                    adj->ancester = node;
                    DFS_Visit(adj, time);
                }
            }
            node->color = Color::Black;
            node->finalized = ++time;
        }

        void DFS()
        {
            for (Node* node: nodes)
            {
                node->color = Color::White;
                node->ancester = NULL;
            }
            
            int time = 0;
            
            for (Node* node : nodes)
            {
                if (node->color == Color::White)
                    DFS_Visit(node, time);
            }
        }

        void BFS(Node* n)
        {
            cout << "Starting BFS..." << endl;
            
            
            cout << "Setting every adjacent..." << endl;
            for (Node* adj : n->adjacents)
            {
                adj->color = Color::White;
                adj->discovered = INT_MAX;
                adj->ancester = nullptr;
            }
            
            
            cout << "Initial node: (" << n->index << ") | ";
            n->color = Color::Gray;
            cout << "Color: " << color_to_string(n->color) << " | ";
            n->discovered = 0;
            cout << "Discovered: " << n->discovered << " | ";
            n->ancester = nullptr;
            cout << "Ancester: Null |" << endl;
            cout << "----------------------------------------------------------------------" << endl;

            cout << "Populating queue with initial node..." << endl;
            Queue queue = {n};

            while (queue.size() != 0)
            {
                Node* node = queue.front();
                queue.erase(queue.begin());
                for (Node* adj : node->adjacents)
                {
                    if (adj->color == Color::White)
                    {
                        cout << "Current node: (" << adj->index << ") | ";
                        adj->color = Color::Gray;
                        cout << "Color: " << color_to_string(adj->color) << " | ";
                        adj->discovered = node->discovered + 1;
                        cout << "Discovered: " << adj->discovered << " | ";
                        adj->ancester = node;
                        cout << "Ancester: " << (adj->ancester)->index << " |" << endl;
                        cout << "----------------------------------------------------------------------" << endl;
                        queue.push_back(adj);
                    }
                }

                if (!queue.empty()) {
                    cout << "Nodes in queue: ";
                    for (Node* a : queue) {
                        cout << a->index << " ";
                    }
                    cout << "\n" << endl;
                } else {
                    cout << "Queue is empty." << endl;
                }

                node->color = Color::Black;
                cout << "node: (" << node->index << ") | ";
                cout << "Color: " << color_to_string(node->color) << " | ";
                cout << "Discovered: " << node->discovered << " | ";
                if (node->ancester != nullptr){
                    cout << "Ancester: " << (node->ancester)->index << " |" << endl;
                } else {
                    cout << "Ancester: Null |" << endl;
                }
                cout << "----------------------------------------------------------------------" << endl;
            }
        }

};

vector<Node*> ask_adjacents(Node &n, size_t available_nodes, vector<Node*> nodes)
{
    cout << "Quantos adjacentes o vértice " << n.index << " possui?\n-> ";
    int num_adjacents;
    cin >> num_adjacents;
    try
    {
        if (num_adjacents >= 0 || num_adjacents <= available_nodes - 1)
        {
            cout << "Agora forneça ao vértice, seus adjacentes (iterativamente): " << endl;
        } else {
            throw (num_adjacents);
        }
    } catch (int myNum) {
        cout << "Número maior ou menor do que o número de vértices disponíveis (" << available_nodes << ")\nNúmero inserido: " << myNum << endl;
    }
    
    vector<Node*> adjacents;

    for (size_t i = 0; i < num_adjacents; i++)
    {
        cout << i + 1 << "º adjacente: ";
        int choice;
        cin >> choice;
        try
        {
            if (choice >= 0 || choice <= available_nodes - 1)
            {
                adjacents.push_back(nodes[choice]);
                continue;
            } else {
                throw (num_adjacents);
            }
        } catch (int myNum) {
            cout << "Número maior ou menor do que o número de vértices disponíveis (" << available_nodes << ")\nNúmero inserido: " << myNum << endl;
        }
    }
    cout << endl;
    return adjacents;
}

int main() {
    Node a, b, c, d, e, f;
    vector<Node*> nodes = { &a, &b, &c, &d, &e, &f};
    int nodes_size = nodes.size();
    for (size_t i = 0; i < nodes_size; i++)
    {
        nodes[i]->index = i;
        nodes[i]->color = Color::White;
        
        vector<Node*> adjacents = ask_adjacents(*nodes[i], nodes_size, nodes);
        for (Node* adj : adjacents){
            nodes[i]->adjacents.push_back(adj);
        }
    }

    Graph graph(nodes);
    // graph.maps_matrix();
    graph.show_matrix_representation();
    // graph.DFS();
    graph.BFS(&a);
    // graph.show_nodes_representation();


    return 0;
}


    // a.adjacents = { &b, &c };
    // b.adjacents = { &a };
    // c.adjacents = { &a };
    // d.adjacents = { &e };
    // e.adjacents = { &d };