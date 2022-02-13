#include "MatrixGraph.h"
#include <iostream>


MatrixGraph::MatrixGraph(int vertexCount) : mMatrix(vertexCount) {
    for(size_t i = 0; i < vertexCount; ++i) {
        for(size_t j = 0; j < vertexCount; ++j) {
            mMatrix[i].push_back(false);
        }
    }
}

int MatrixGraph::VerticesCount() const {
    return mMatrix.size();
}

void MatrixGraph::AddEdge(int from, int to) {
    mMatrix[from][to] = true;
}

std::vector<int> MatrixGraph::GetNextVertices(int vertex) const {
    std::vector<int> NextVertices;

    for(size_t i = 0; i < VerticesCount(); ++i) {
        if (mMatrix[vertex][i] == true) {
            NextVertices.push_back(i);
        }
    }
    return NextVertices;
}

std::vector<int> MatrixGraph::GetPrevVertices(int vertex) const {
    std::vector<int> PrevVertices;

    for(size_t i = 0; i < VerticesCount(); ++i) {
        if (mMatrix[i][vertex] == true) {
            PrevVertices.push_back(i);
        }
    }
    return PrevVertices;
}

MatrixGraph::MatrixGraph(IGraph& graph) : mMatrix(graph.VerticesCount()) {
    for(auto &it : mMatrix) {
        it.resize(graph.VerticesCount());
    }
    for(size_t row = 0; row < VerticesCount(); ++row) {
        std::vector<int> next = graph.GetNextVertices(row);
        for(auto it: next) {
            mMatrix[row][it] = true;
        }
    }

}