#ifndef VERTEX_H
#define VERTEX_H

#include <vector>

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
    std::vector<Vertex*> adjacents;
    int discovered;
    int finalized;
    
    bool operator<(const Vertex& other) const {
        return index < other.index;
    }
};

#endif // VERTEX_H
