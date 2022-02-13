#pragma once
#include "IGraph.h"
#include <set>

class SetGraph : public IGraph {
public:
	SetGraph(int vertexCount);
	SetGraph(IGraph& graph);


	virtual void AddEdge( int from, int to ) override;
	virtual int VerticesCount() const override;
	virtual std::vector<int> GetNextVertices(int vertex) const override;
	virtual std::vector<int> GetPrevVertices(int vertex) const override;

private:
	std::vector<std::set<int>> mTreeVector;
};