#pragma once
#include "IGraph.h"

class MatrixGraph : public IGraph {
public:
	MatrixGraph(int vertexCount);
	MatrixGraph(IGraph& graph);


	virtual void AddEdge( int from, int to ) override;
	virtual int VerticesCount() const override;
	virtual std::vector<int> GetNextVertices(int vertex) const override;
	virtual std::vector<int> GetPrevVertices(int vertex) const override;

private:
	std::vector<std::vector<bool>> mMatrix;
};