

int ModyfiedBFS(const IGraph& graph, int from_vertex, int to_vertex) {
	std::queue<int> bfsQueue;
	bfsQueue.push(from_vertex);

    int VerticesCount = graph.VerticesCount();
    std::vector<int> way(VerticesCount, __INT32_MAX__);
    way[from_vertex] = 0;

    std::vector<int> count(VerticesCount, 0);
    count[from_vertex] = 1;

	while( bfsQueue.size() > 0 ) {
		int current = bfsQueue.front();
		bfsQueue.pop();
		std::vector<int> next = graph.GetNextVertices(current);
		for (auto nextVertex : next) {
            if (way[nextVertex] > way[current] + 1) {
                way[nextVertex] = way[current] + 1;
                bfsQueue.push(nextVertex);
            }  
            if (way[nextVertex] == way[current] + 1) {
                count[nextVertex] += count[current];
            }
        }
	}
    return count[to_vertex];
}

int main() {
    int vertices = 0;
    int edges = 0;
    std::cin >> vertices >> edges;

    ListGraph graph(vertices);

    int from = 0;
    int to = 0;
    for(int i = 0; i < edges; ++i) {
        std::cin >> from >> to;
        graph.AddEdge(from, to);
        graph.AddEdge(to, from);
    }
    std::cin >> from >> to;
    std::cout << ModyfiedBFS(graph, from, to);

    return 0;
}