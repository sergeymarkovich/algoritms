#include <iostream>
#include <vector>
#include <cassert>
#include <functional>
#include <queue>
#include <set>

struct IGraph {
    virtual ~IGraph() {}
    // Добавление ребра от from к to.
    virtual void AddEdge(int from, int to) = 0;

    virtual int VerticesCount() const  = 0;

    virtual std::vector<int> GetNextVertices(int vertex) const = 0;
    virtual std::vector<int> GetPrevVertices(int vertex) const = 0;
};

class MatrixGraph : public IGraph {
public:
    MatrixGraph(int size): adjacencyMatrix(size, std::vector<int>(size, 0)) {
    }

    MatrixGraph(const IGraph &graph) : adjacencyMatrix(graph.VerticesCount(), std::vector<int>(graph.VerticesCount(), 0))
    {
        for (int i = 0; i < adjacencyMatrix.size(); i++) {
            for (auto vertex : graph.GetNextVertices(i)) {
                adjacencyMatrix[i][vertex] = 1;
            }
        }
    }
    ~MatrixGraph() {
    }


    void AddEdge(int from, int to) override {
        adjacencyMatrix[from][to] = 1;
    }

    int VerticesCount() const override {
        return adjacencyMatrix.size();
    };
    std::vector<int> GetNextVertices(int vertex) const override {
        std::vector<int> tmp;
        for (int i = 0; i < adjacencyMatrix.size(); i++) {
            if (adjacencyMatrix[vertex][i] == 1) {
                tmp.push_back(i);
            }
        }
        return tmp;
    }
    std::vector<int> GetPrevVertices(int vertex) const override {
        std::vector<int> tmp;
        for (int i = 0; i < adjacencyMatrix.size(); i++) {
            if (adjacencyMatrix[i][vertex] == 1) {
                tmp.push_back(i);
            }
        }
        return tmp;
    }

private:
    std::vector<std::vector<int>> adjacencyMatrix;

};

class SetGraph: public IGraph {
public:
    SetGraph(int size): treeList(size) {}
    SetGraph(const IGraph &graph) : treeList(graph.VerticesCount()) {
        for (int i = 0; i < treeList.size(); i++) {
            for (auto vertex : graph.GetNextVertices(i)) {
                treeList[i].insert(vertex);
            }
        }
    }
    ~SetGraph() {}

    void AddEdge(int from, int to) override {
        treeList[from].insert(to);
    }
    int VerticesCount() const override {
        return treeList.size();
    }

    std::vector<int> GetNextVertices(int vertex) const override {
        std::vector<int> tmp;
        for (auto element : treeList[vertex]) {
            tmp.push_back(element);
        }
        return tmp;
    }
    std::vector<int> GetPrevVertices(int vertex) const override {
        std::vector<int> tmp;
        for (int row = 0; row < treeList.size(); row++) {
            for (auto ver : treeList[row]) {
                if (ver == vertex) {
                    tmp.push_back(row);
                }
            }
        }
        return tmp;
    }

private:
    std::vector<std::set<int>> treeList;
};

class ArcGraph: public IGraph {
public:
    ArcGraph(int size) {
    }

    ArcGraph(IGraph & graph) : pairList() {
        for (int i = 0; i < graph.VerticesCount(); i++) {
            for (auto element : graph.GetNextVertices(i)) {
                pairList.emplace_back(i, element);
            }
        }
    }
    ~ArcGraph() {
    }
    void AddEdge(int from, int to) override {
        pairList.emplace_back(from, to);
    }

    int VerticesCount() const  override {
        std::set<int> uniqueVerteces;
        for (auto i = pairList.begin(); i != pairList.end(); i++) {
            if (uniqueVerteces.find(i->first) == uniqueVerteces.end()) {
                uniqueVerteces.insert(i->first);
            } else if (uniqueVerteces.find(i->second) == uniqueVerteces.end())
                uniqueVerteces.insert(i->second);
        }
        return uniqueVerteces.size();
    }

    std::vector<int> GetNextVertices(int vertex) const override {
        std::vector<int> tmp;
        for (auto pair : pairList) {
            if (pair.first == vertex) {
                tmp.push_back(pair.second);
            }
        }
        return tmp;
    }
    std::vector<int> GetPrevVertices(int vertex) const override {
        std::vector<int> tmp;
        for (auto pair : pairList) {
            if (pair.second == vertex) {
                tmp.push_back(pair.first);
            }
        }
        return tmp;
    }

private:
    std::vector<std::pair<int, int>> pairList;
};

class ListGraph: public IGraph
{
public:
    ListGraph(int size) : adjacencyLists(size)
    {
    }

    ListGraph(const IGraph &graph) : adjacencyLists(graph.VerticesCount())
    {
        for (int i = 0; i < graph.VerticesCount(); i++)
        {
            adjacencyLists[i] = graph.GetNextVertices(i);
        }
    }

    ~ListGraph()
    {
    }

    void AddEdge(int from, int to) override
    {
        assert(0 <= from && from < adjacencyLists.size());
        assert(0 <= to && to < adjacencyLists.size());
        adjacencyLists[from].push_back(to);
    }

    int VerticesCount() const override
    {
        return (int)adjacencyLists.size();
    }

    std::vector<int> GetNextVertices(int vertex) const override
    {
        assert(0 <= vertex && vertex < adjacencyLists.size());
        return adjacencyLists[vertex];
    }

    std::vector<int> GetPrevVertices(int vertex) const override
    {
        assert(0 <= vertex && vertex < adjacencyLists.size());
        std::vector<int> prevVertices;

        for (int from = 0; from < adjacencyLists.size(); from++)
        {
            for (int to: adjacencyLists[from])
            {
                if (to == vertex)
                {
                    prevVertices.push_back(from);
                }
            }
        }

        return prevVertices;
    }

private:
    std::vector<std::vector<int>> adjacencyLists;
};

void BFS(const IGraph &graph, int vertex, std::vector<bool> &visited, std::function<void(int)> &func)
{
    std::queue<int> qu;
    qu.push(vertex);
    visited[vertex] = true;

    while (!qu.empty())
    {
        int currentVertex = qu.front();
        qu.pop();

        func(currentVertex);

        for (int nextVertex: graph.GetNextVertices(currentVertex))
        {
            if (!visited[nextVertex])
            {
                visited[nextVertex] = true;
                qu.push(nextVertex);
            }
        }
    }
}

void mainBFS(const IGraph &graph, std::function<void(int)> func)
{
    std::vector<bool> visited(graph.VerticesCount(), false);

    for (int i = 0; i < graph.VerticesCount(); i++)
    {
        if (!visited[i])
            BFS(graph, i, visited, func);
    }
}

void DFS(const IGraph &graph, int vertex, std::vector<bool> &visited, std::function<void(int)> &func)
{
    visited[vertex] = true;
    func(vertex);

    for (int nextVertex: graph.GetNextVertices(vertex))
    {
        if (!visited[nextVertex])
            DFS(graph, nextVertex, visited, func);
    }

}

void mainDFS(const IGraph &graph, std::function<void(int)> func)
{
    std::vector<bool> visited(graph.VerticesCount(), false);

    for (int i = 0; i < graph.VerticesCount(); i++)
    {
        if (!visited[i])
            DFS(graph, i, visited, func);
    }
}

void topologicalSortInternal(const IGraph &graph, int vertex, std::vector<bool> &visited, std::deque<int> &sorted)
{
    visited[vertex] = true;

    for (int nextVertex: graph.GetNextVertices(vertex))
    {
        if (!visited[nextVertex])
            topologicalSortInternal(graph, nextVertex, visited, sorted);
    }

    sorted.push_front(vertex);
}

std::deque<int> topologicalSort(const IGraph &graph)
{
    std::deque<int> sorted;
    std::vector<bool> visited(graph.VerticesCount(), false);

    for (int i = 0; i < graph.VerticesCount(); i++)
    {
        if (!visited[i])
            topologicalSortInternal(graph, i, visited, sorted);
    }

    return sorted;
}


int main(int argc, const char * argv[]) {
    ListGraph graph(7);
    graph.AddEdge(0, 1);
    graph.AddEdge(0, 5);
    graph.AddEdge(1, 2);
    graph.AddEdge(1, 3);
    graph.AddEdge(1, 5);
    graph.AddEdge(1, 6);
    graph.AddEdge(3, 2);
    graph.AddEdge(3, 4);
    graph.AddEdge(3, 6);
    graph.AddEdge(5, 4);
    graph.AddEdge(5, 6);
    graph.AddEdge(6, 4);
    ArcGraph arcGraph(graph);
    SetGraph setGraph(arcGraph);
    MatrixGraph mGraph(setGraph);

    mainBFS(graph, [](int vertex){ std::cout << vertex << " ";});
    std::cout << std::endl;
    mainBFS(mGraph, [](int vertex){ std::cout << vertex << " ";});
    std::cout << std::endl;
    mainBFS(setGraph, [](int vertex){ std::cout << vertex << " ";});
    std::cout << std::endl;
    mainBFS(arcGraph, [](int vertex){ std::cout << vertex << " ";});
    std::cout << std::endl;

    mainDFS(graph, [](int vertex){ std::cout << vertex << " ";});
    std::cout << std::endl;
    mainDFS(mGraph, [](int vertex){ std::cout << vertex << " ";});
    std::cout << std::endl;
    mainDFS(setGraph, [](int vertex){ std::cout << vertex << " ";});
    std::cout << std::endl;
    mainDFS(arcGraph, [](int vertex){ std::cout << vertex << " ";});
    std::cout << std::endl;

    for (int vertex: topologicalSort(graph))
    {
        std::cout << vertex << " ";
    }
    std::cout << std::endl;

    for (int vertex: topologicalSort(mGraph))
    {
        std::cout << vertex << " ";
    }
    std::cout << std::endl;

    for (int vertex: topologicalSort(setGraph))
    {
        std::cout << vertex << " ";
    }
    std::cout << std::endl;
    for (int vertex: topologicalSort(arcGraph))
    {
        std::cout << vertex << " ";
    }
    std::cout << std::endl;

    return 0;
}
