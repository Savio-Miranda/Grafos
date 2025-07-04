#ifndef UTILS_H
#define UTILS_H

#include <vector>
#include "Vertex.h"

typedef std::vector<std::vector<int>> Matrix;
typedef std::vector<Vertex*> Queue;

int partition(std::vector<Vertex*>& arr, int low, int high) {
    int pivot = arr[high]->index;
    int i = low - 1;

    for (int j = low; j < high; j++) {
        if (arr[j]->index <= pivot) {
            std::swap(arr[i + 1], arr[j]);
            i++;
        }
    }

    std::swap(arr[i + 1], arr[high]);
    return i + 1;
}

void quickSort(std::vector<Vertex*>& arr, int low, int high) {
    if (low < high) {
        int pi = partition(arr, low, high);
        quickSort(arr, low, pi - 1);
        quickSort(arr, pi + 1, high);
    }
}

#endif // UTILS_H
