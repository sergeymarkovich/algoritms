#include "ArcGraph.h"

int ArcGraph::VerticesCount() const {
    return vertex;
}

void ArcGraph::AddEdge(int from, int to) {
    mPairs.push_back(std::make_pair(from, to));
}


std::vector<int> ArcGraph::GetNextVertices(int vertex) const {
    std::vector<int> NextVertices;
    for(auto &it : mPairs) {
        if (it.first == vertex) {
            NextVertices.push_back(it.second);
        }
    }
    return NextVertices;
}

std::vector<int> ArcGraph::GetPrevVertices(int vertex) const {
    std::vector<int> PrevVertices;
    for(auto &it : mPairs) {
        if (it.second == vertex) {
            PrevVertices.push_back(it.first);
        }
    }
    return PrevVertices;
}

ArcGraph::ArcGraph(IGraph& graph) : vertex(graph.VerticesCount()) {
    for(size_t i = 0; i < vertex; ++i) {
        std::vector<int> tmp = graph.GetNextVertices(i);
        for(auto &it : tmp) {
            mPairs.push_back(std::make_pair(i, it));
        }
    }
}