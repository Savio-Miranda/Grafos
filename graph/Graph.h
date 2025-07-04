#ifndef GRAPH_H
#define GRAPH_H

#include <iostream>
#include <vector>
#include <limits.h>
#include "Vertex.h"
#include "utils.h"

using namespace std;

class Graph {
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

        void order_DFS_list(int starting_vertex){
            quickSort(vertices, 0, vertices.size() - 1);
            vertices.insert(vertices.begin(), vertices[starting_vertex]);
            for(Vertex* v : vertices){
                if (starting_vertex == v->index){
                    vertices.erase(vertices.begin() + starting_vertex + 1);
                    break;
                }
            }
        }
        
        vector<int> DFS_backtrack(Vertex* vertex, vector<int> circle_path){
            if (vertex->ancester == nullptr)
                return circle_path;
            
            circle_path.push_back(vertex->ancester->index);
            return DFS_backtrack(vertex->ancester, circle_path); // RETURN ADICIONADO
        }
        
        void DFS_Visit(Vertex* vertex, int &time)
        {
            vertex->color = Color::Gray;
            time++;
            vertex->discovered = time;
            
            cout << "Visiting ";
            show_vertex(vertex);
            
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
            cout << "*Back to ";
            show_vertex(vertex);
        }

        void DFS(int starting_vertex)
        {
            Vertex* n = vertices[starting_vertex];
            cout << "Starting DFS..." << endl;
            cout << "Setting every adjacent..." << endl;
            for (Vertex* vertex: vertices)
            {
                vertex->color = Color::White;
                vertex->ancester = NULL;
            }
            
            int time = 0;
            
            order_DFS_list(starting_vertex);

            for (Vertex* vertex : vertices)
            {
                if (vertex->color == Color::White)
                    DFS_Visit(vertex, time);
            }
            cout << "DFS finished...\n" << endl;
        }


        void DFS_visit_cycle(Vertex* vertex, int &time, vector<Vertex*> &cycle_path)
        {
            vertex->color = Color::Gray;
            time++;
            vertex->discovered = time;
            cycle_path.push_back(vertex);

            for(Vertex* adj: vertex->adjacents)
            {   
                if (adj->color == Color::White)
                {
                    adj->ancester = vertex;
                    DFS_visit_cycle(adj, time, cycle_path);
                }
                if (vertex->ancester == nullptr){
                    return;
                }
                if (adj->color == Color::Gray && adj->index != (vertex->ancester)->index){
                    return;
                }

            }
            return;
        }

        void cut_vector(std::vector<Vertex*> &cycle_path) {
            while (!cycle_path.empty()) {
                Vertex* current = cycle_path.front();
                Vertex* target = cycle_path.back();

                bool has_adjacent_to_last = false;
                for (Vertex* neighbor : current->adjacents) {
                    if (neighbor == target) {
                        has_adjacent_to_last = true;
                        break;
                    }
                }

                if (has_adjacent_to_last) {
                    break; // achou um vértice com adjacente correto, para de cortar
                } else {
                    cycle_path.erase(cycle_path.begin()); // remove o primeiro elemento
                }
            }
        }

        void DFS_Cycle(int starting_vertex)
        {
            Vertex* n = vertices[starting_vertex];
            cout << "Starting DFS..." << endl;
            cout << "Setting every adjacent..." << endl;
            for (Vertex* vertex: vertices)
            {
                vertex->color = Color::White;
                vertex->ancester = NULL;
            }
            
            vector<Vertex*> cycle_path;
            int time = 0;

            order_DFS_list(starting_vertex);

            for (Vertex* vertex : vertices)
            {
                if (vertex->color == Color::White){
                    DFS_visit_cycle(vertex, time, cycle_path);
                    if (!cycle_path.empty()){
                        cout << "Cycle founded: ";
                        cut_vector(cycle_path);
                        cycle_path.push_back(cycle_path[0]);
                        for (Vertex* c : cycle_path){
                            cout << c->index << " ";
                        }
                        cout << endl;
                        cout << "DFS finished..." << endl;
                        return;
                    }
                }   
            }
            
            cout << "No cycle founded..." << endl;
            cout << "DFS finished..." << endl;
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

            cout << "PATH SIZE: " << path_vector.size() << endl;
            cout << "PATH: ";
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
};

#endif // GRAPH_H
