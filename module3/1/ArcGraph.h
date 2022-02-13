#pragma once
#include "IGraph.h"


class ArcGraph : public IGraph {
public:
	ArcGraph(int vertexCount) : vertex(vertexCount) {};
	ArcGraph(IGraph& graph);

	virtual void AddEdge(int from, int to) override;
	virtual int VerticesCount() const override;
	virtual std::vector<int> GetNextVertices(int vertex) const override;
	virtual std::vector<int> GetPrevVertices(int vertex) const override;

private:
    std::vector<std::pair<int, int>> mPairs;
    int vertex;
};

