#include <iostream>
#include <vector>
#include <queue>
#include <omp.h>
#include <time.h>

using namespace std;


// Sequential BFS
void bfsSequential(vector<vector<int>>& graph, int start, int n) {

    vector<bool> visited(n, false);

    queue<int> q;

    q.push(start);

    visited[start] = true;

    cout << "Sequential BFS : ";

    while(!q.empty()) {

        int node = q.front();

        q.pop();

        cout << node << " ";

        for(auto neighbor : graph[node]) {

            if(!visited[neighbor]) {

                visited[neighbor] = true;

                q.push(neighbor);
            }
        }
    }

    cout << endl;
}


// Parallel BFS
void bfsParallel(vector<vector<int>>& graph, int start, int n) {

    vector<bool> visited(n, false);

    queue<int> q;

    q.push(start);

    visited[start] = true;

    cout << "Parallel BFS : ";

    while(!q.empty()) {

        int node = q.front();

        q.pop();

        cout << node << " ";

        #pragma omp parallel for
        for(int i = 0; i < graph[node].size(); i++) {

            int neighbor = graph[node][i];

            #pragma omp critical
            {
                if(!visited[neighbor]) {

                    visited[neighbor] = true;

                    q.push(neighbor);
                }
            }
        }
    }

    cout << endl;
}


int main() {

    // ===== RANDOM LARGE GRAPH =====

    int n = 500;

    int edges = 2000;

    vector<vector<int>> graph(n);

    for(int i = 0; i < edges; i++) {

        int u = rand() % n;

        int v = rand() % n;

        graph[u].push_back(v);

        graph[v].push_back(u);
    }

    int start = 0;



    /*
    ===== MANUAL INPUT =====

    int n, edges;

    cout << "Enter number of nodes: ";
    cin >> n;

    vector<vector<int>> graph(n);

    cout << "Enter number of edges: ";
    cin >> edges;

    cout << "Enter edges (u v):\n";

    for(int i = 0; i < edges; i++) {

        int u, v;

        cin >> u >> v;

        graph[u].push_back(v);

        graph[v].push_back(u);
    }

    int start;

    cout << "Enter start node: ";
    cin >> start;

    */


    clock_t start_time, end_time;


    // Sequential BFS
    start_time = clock();

    bfsSequential(graph, start, n);

    end_time = clock();

    double seq_time =
    ((double)(end_time - start_time) * 1000)
    / CLOCKS_PER_SEC;

    cout << "Sequential BFS Time : "
         << seq_time << " ms\n\n";


    // Parallel BFS
    start_time = clock();

    bfsParallel(graph, start, n);

    end_time = clock();

    double par_time =
    ((double)(end_time - start_time) * 1000)
    / CLOCKS_PER_SEC;

    cout << "Parallel BFS Time : "
         << par_time << " ms\n\n";

    cout << "Speedup : "
         << seq_time / par_time << endl;

    return 0;
}