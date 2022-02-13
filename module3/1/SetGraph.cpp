#include "SetGraph.h"

SetGraph::SetGraph(int vertexCount) : mTreeVector(vertexCount) {};

SetGraph::SetGraph(IGraph& graph) : mTreeVector(graph.VerticesCount()) {
    for(size_t i = 0; i < graph.VerticesCount(); ++i) {
        for(auto &it : graph.GetNextVertices(i)) {
            mTreeVector[i].insert(it);
        }
    }
}

void SetGraph::AddEdge(int from, int to) {
    mTreeVector[from].insert(to);
}

int SetGraph::VerticesCount() const {
    return mTreeVector.size();
}

std::vector<int> SetGraph::GetNextVertices(int vertex) const {
    std::vector<int> NextVertices;
    for(auto &it : mTreeVector[vertex]) {
        NextVertices.push_back(it);
    }
    return NextVertices;
}

std::vector<int> SetGraph::GetPrevVertices(int vertex) const {
    std::vector<int> PrevVertices;
    for(size_t i = 0; i < VerticesCount(); ++i) {
        if(mTreeVector[i].find(vertex) != mTreeVector[i].end()) {
            PrevVertices.push_back(i);
        }
    }
    return PrevVertices;
}