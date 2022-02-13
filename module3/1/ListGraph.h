#pragma once
#include "IGraph.h"

class ListGraph : public IGraph {
public:
	ListGraph( int vertexCount );
	ListGraph( IGraph& graph );

	// IGraph
	virtual void AddEdge( int from, int to ) override;
	virtual int VerticesCount() const override;
	virtual std::vector<int> GetNextVertices( int vertex ) const override;
	virtual std::vector<int> GetPrevVertices( int vertex ) const override;

private:
	std::vector<std::vector<int>> mLists;
};