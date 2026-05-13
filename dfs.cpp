#include <iostream>
#include <vector>
#include <stack>
#include <omp.h>
#include <time.h>

using namespace std;


// Sequential DFS
void dfsSequential(vector<vector<int>>& graph, int start, int n) {

    vector<bool> visited(n, false);

    stack<int> s;

    s.push(start);

    visited[start] = true;

    cout << "Sequential DFS : ";

    while(!s.empty()) {

        int node = s.top();

        s.pop();

        cout << node << " ";

        for(auto neighbor : graph[node]) {

            if(!visited[neighbor]) {

                visited[neighbor] = true;

                s.push(neighbor);
            }
        }
    }

    cout << endl;
}


// Parallel DFS
void dfsParallel(vector<vector<int>>& graph, int start, int n) {

    vector<bool> visited(n, false);

    stack<int> s;

    s.push(start);

    visited[start] = true;

    cout << "Parallel DFS : ";

    while(!s.empty()) {

        int node = s.top();

        s.pop();

        cout << node << " ";

        #pragma omp parallel for
        for(int i = 0; i < graph[node].size(); i++) {

            int neighbor = graph[node][i];

            #pragma omp critical
            {
                if(!visited[neighbor]) {

                    visited[neighbor] = true;

                    s.push(neighbor);
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


    // Sequential DFS
    start_time = clock();

    dfsSequential(graph, start, n);

    end_time = clock();

    double seq_time =
    ((double)(end_time - start_time) * 1000)
    / CLOCKS_PER_SEC;

    cout << "Sequential DFS Time : "
         << seq_time << " ms\n\n";


    // Parallel DFS
    start_time = clock();

    dfsParallel(graph, start, n);

    end_time = clock();

    double par_time =
    ((double)(end_time - start_time) * 1000)
    / CLOCKS_PER_SEC;

    cout << "Parallel DFS Time : "
         << par_time << " ms\n\n";

    cout << "Speedup : "
         << seq_time / par_time << endl;

    return 0;
}