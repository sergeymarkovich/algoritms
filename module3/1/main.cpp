#include "ListGraph.h"
#include "MatrixGraph.h"
#include "ArcGraph.h"
#include "SetGraph.h"

#include <queue>
#include <iostream>

void BFS( const IGraph& graph, int vertex, void (*visit)(int) ) {
	std::vector<bool> visited( graph.VerticesCount(), false );
	std::queue<int> bfsQueue;
	bfsQueue.push( vertex );
	visited[vertex] = true;

	while( bfsQueue.size() > 0 ) {
		int current = bfsQueue.front();
		bfsQueue.pop();
		visit( current );
		std::vector<int> list = graph.GetNextVertices( current );
		for( int i = 0; i < list.size(); ++i ) {
            if( !visited[list[i]] ) {
				bfsQueue.push( list[i] );
				visited[list[i]] = true;
			}
        }
	}
}

void Test(IGraph& graph) {
    graph.AddEdge( 0, 1 );
    graph.AddEdge( 2, 6 );
    graph.AddEdge( 4, 6 );
    graph.AddEdge( 1, 5 );
    graph.AddEdge( 4, 5 );
	graph.AddEdge( 0, 2 );
	graph.AddEdge( 0, 3 );
	graph.AddEdge( 1, 3 );
	graph.AddEdge( 3, 4 );
}

int main() {
//0---->1----->5
//|\   /    /
//| \ /    /
//2  3--->4
// \       \
//  \------>6

	MatrixGraph graph(7);
    
    Test(graph);

    ListGraph graph2(graph);
    ArcGraph graph3(graph2);
    SetGraph graph4(graph3);

    MatrixGraph graph8(graph4);

    std::cout << "Checking for IGraph& constructor (BFS also checks VerticesCount and GetNextVertices!): \n";
	BFS( graph, 1, []( int vertex ) { std::cout << vertex << " "; } );
    std::cout << "\n";
    BFS( graph2, 1, []( int vertex ) { std::cout << vertex << " "; } );
    std::cout << "\n";
    BFS( graph3, 1, []( int vertex ) { std::cout << vertex << " "; } );
    std::cout << "\n";
    BFS( graph4, 1, []( int vertex ) { std::cout << vertex << " "; } );
    std::cout << "\n";
    BFS( graph8, 1, []( int vertex ) { std::cout << vertex << " "; } );
    std::cout << "\n";


    ListGraph graph5(7);
    ArcGraph graph6(7);
    SetGraph graph7(7);

    Test(graph5);
    Test(graph6);
    Test(graph7);


    std::cout << "Checking for addEdge: \n";
    BFS( graph5, 1, []( int vertex ) { std::cout << vertex << " "; } );
    std::cout << "\n";
    BFS( graph6, 1, []( int vertex ) { std::cout << vertex << " "; } );
    std::cout << "\n";
    BFS( graph7, 1, []( int vertex ) { std::cout << vertex << " "; } );
    std::cout << "\n";

    std::cout << "Checking getPreviousVertices:\n";
    for(auto it: graph.GetPrevVertices(6)) std::cout << it << " ";
    std::cout << "\n";
    for(auto it: graph2.GetPrevVertices(6)) std::cout << it << " ";
    std::cout << "\n";
    for(auto it: graph3.GetPrevVertices(6)) std::cout << it << " ";
    std::cout << "\n";
    for(auto it: graph4.GetPrevVertices(6)) std::cout << it << " ";
    std::cout << "\n";

	return 0;
}