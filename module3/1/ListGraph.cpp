#include "ListGraph.h"
#include <cassert>

ListGraph::ListGraph(int vertexCount) :
	mLists(vertexCount)
{
}

ListGraph::ListGraph(IGraph& graph) :
	mLists(graph.VerticesCount())
{
	for(int i = 0; i < mLists.size(); ++i)
		mLists[i] = graph.GetNextVertices(i);
}

void ListGraph::AddEdge(int from, int to) {
	assert(from >= 0 && from < mLists.size());
	assert(to >= 0 && to < mLists.size());

	mLists[from].push_back(to);
}

int ListGraph::VerticesCount() const {
	return mLists.size();
}

std::vector<int> ListGraph::GetNextVertices(int vertex) const {
	assert(vertex >= 0 && vertex < mLists.size());
	return mLists[vertex];
}

std::vector<int> ListGraph::GetPrevVertices(int vertex) const {
	assert( vertex >= 0 && vertex < mLists.size() );
	std::vector<int> result;
	for (int from = 0; from < mLists.size(); ++from)
		for (int i = 0; i < mLists[from].size(); ++i)
			if (mLists[from][i] == vertex)
				result.push_back(from);
	return result;
}