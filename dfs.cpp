#include <iostream>
#include <vector>
#include <stack>
#include <omp.h>

using namespace std;

class Graph {
public:
    Graph(int vertices);
    void addEdge(int v, int w);
    void DFS(int startVertex);

private:
    int vertices;
    vector<vector<int> > adjacencyList;
};

Graph::Graph(int vertices) : vertices(vertices) {
    adjacencyList.resize(vertices);
}

void Graph::addEdge(int v, int w) {
    adjacencyList[v].push_back(w);
    adjacencyList[w].push_back(v); // Assuming an undirected graph
}

void Graph::DFS(int startVertex) {
    vector<bool> visited(vertices, false);
    stack<int> dfsStack;

    #pragma omp parallel
    {
        #pragma omp single
        dfsStack.push(startVertex);

        while (!dfsStack.empty()) {
            int currentVertex;
            #pragma omp critical
            {
                currentVertex = dfsStack.top();
                dfsStack.pop();
            }

            if (!visited[currentVertex]) {
                #pragma omp critical
                {
                    cout << "Visited: " << currentVertex << endl;
                    visited[currentVertex] = true;
                }

                // Use OpenMP for parallelizing the loop
                #pragma omp parallel for
                for (int i = 0; i < adjacencyList[currentVertex].size(); ++i) {
                    int adjacentVertex = adjacencyList[currentVertex][i];
                    if (!visited[adjacentVertex]) {
                        #pragma omp critical
                        {
                            dfsStack.push(adjacentVertex);
                        }
                    }
                }
            }
        }
    }
}

int main() {
    int vertices, edges;

    cout << "Enter the number of vertices: ";
    cin >> vertices;

    Graph graph(vertices);

    cout << "Enter the number of edges: ";
    cin >> edges;

    cout << "Enter the edges (format: vertex1 vertex2):" << endl;
    for (int i = 0; i < edges; ++i) {
        int v, w;
        cin >> v >> w;
        graph.addEdge(v, w);
    }

    int startVertex;
    cout << "Enter the starting vertex for DFS: ";
    cin >> startVertex;

    cout << "DFS starting from vertex " << startVertex << ":" << endl;
    graph.DFS(startVertex);

    return 0;
}
