#include <iostream>
#include <vector>
#include <queue>
#include <omp.h>

using namespace std;

void bfs(vector<vector<int> >& graph, vector<bool>& visited, int start) {
    queue<int> q;
    q.push(start);
    visited[start] = true;

    while (!q.empty()) {
        int current = q.front();
        cout << "Visiting node: " << current << " in thread: " << omp_get_thread_num() << endl;
        q.pop();

        #pragma omp parallel
        {
            #pragma omp for
            for (int i = 0; i < graph[current].size(); ++i) {
                int neighbor = graph[current][i];
                if (!visited[neighbor]) {
                    #pragma omp critical
                    {
                        visited[neighbor] = true;
                        q.push(neighbor);
                    }
                }
            }
        }
    }
}

int main() {
    int num_nodes, num_edges;

    cout << "Enter the number of nodes: ";
    cin >> num_nodes;

    cout << "Enter the number of edges: ";
    cin >> num_edges;

    vector<vector<int> > graph(num_nodes);

    cout << "Enter the edges (format: source destination):" << endl;
    for (int i = 0; i < num_edges; ++i) {
        int source, destination;
        cin >> source >> destination;
        graph[source].push_back(destination);
        graph[destination].push_back(source); // Assuming an undirected graph
    }

    vector<bool> visited(num_nodes, false);

    #pragma omp parallel num_threads(4)
    {
        #pragma omp for
        for (int i = 0; i < num_nodes; ++i) {
            if (!visited[i]) {
                bfs(graph, visited, i);
            }
        }
    }

    return 0;
}
