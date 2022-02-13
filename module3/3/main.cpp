#include <vector>
#include <cassert>
#include <queue>
#include <iostream>
#include <cmath>  // for infty
#include <set>

struct IGraph {
    virtual ~IGraph() {}
	
    virtual void AddEdge(int from, int to, int weight) = 0;

	virtual int VerticesCount() const  = 0;

    virtual std::vector<std::pair<int, int>> GetNextVertices(int vertex) const = 0;
    virtual std::vector<std::pair<int, int>> GetPrevVertices(int vertex) const = 0;
};

class ListGraph : public IGraph {
public:
	ListGraph(int vertexCount) : mLists(vertexCount) {};

	// IGraph
	virtual void AddEdge( int from, int to, int weight ) override;
	virtual int VerticesCount() const override;
	virtual std::vector<std::pair<int, int>> GetNextVertices( int vertex ) const override;
	virtual std::vector<std::pair<int, int>> GetPrevVertices( int vertex ) const override;

private:
	std::vector<std::vector<std::pair<int, int>>> mLists;
};

void ListGraph::AddEdge(int from, int to, int weight) {
    mLists[to].emplace_back(from, weight);
}

int ListGraph::VerticesCount() const {
	return mLists.size();
}

std::vector<std::pair<int, int>> ListGraph::GetNextVertices(int vertex) const {
	return mLists[vertex];
}

std::vector<std::pair<int, int>> ListGraph::GetPrevVertices(int vertex) const {
    std::vector<std::pair<int, int>> result;
	for (int from = 0; from < mLists.size(); ++from)
		for (int i = 0; i < mLists[from].size(); ++i)
			if (mLists[from][i].first == vertex)
				result.emplace_back(from, mLists[from][i].second);
	return result;
}

int MinWayLength(const ListGraph& graph, int from, int to) {
    int verticesCount = graph.VerticesCount();
    std::vector<long> way(verticesCount, INT64_MAX);
    std::set<std::pair<int, int>> PriorityQueue;
    way[from] = 0;
    PriorityQueue.emplace(0, from);

    while(!PriorityQueue.empty()) {
        auto vertex_from_queue = *PriorityQueue.begin();
        auto nextVertices = graph.GetNextVertices(vertex_from_queue.second);

        for (auto &nextVertex : nextVertices) {
            if (way[nextVertex.first] > way[vertex_from_queue.second] + nextVertex.second) {
                std::pair<int, int> weightNextVertex(way[nextVertex.first], nextVertex.first);
                auto vertexInQueue = PriorityQueue.find(weightNextVertex);

                if (vertexInQueue != PriorityQueue.end()) {
                    PriorityQueue.erase(vertexInQueue);
                }

                way[nextVertex.first] = way[vertex_from_queue.second] + nextVertex.second;
                PriorityQueue.insert(std::pair<int, int>(way[nextVertex.first], nextVertex.first));
            }
        }
        PriorityQueue.erase(PriorityQueue.find(vertex_from_queue));
    }
    return way[to];
}

int main() {
    int verticesCount = 0;
    int edgesCount = 0;
    std::cin >> verticesCount >> edgesCount;
    ListGraph graph(verticesCount);
    
    int from = 0;
    int to = 0;
    int weight = 0;
    for (int i = 0; i < edgesCount; i++) {
        std::cin >> from >> to >> weight;
        graph.AddEdge(from, to, weight);
        graph.AddEdge(to, from, weight);
    }

    std::cin >> from >> to;
    std::cout << MinWayLength(graph, from, to);
    return 0;
}