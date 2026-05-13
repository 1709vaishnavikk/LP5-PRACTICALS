#include <iostream>
#include <vector>
#include <queue>
#include <stack>
#include <omp.h>
#include <time.h>

using namespace std;


// ===== BFS =====

// Sequential BFS
void bfsSequential(vector<vector<int>>& graph, int start, int n) {

    vector<bool> visited(n, false);

    queue<int> q;

    q.push(start);

    visited[start] = true;

    int count = 0;

    cout << "Sequential BFS Traversal : ";

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

    int count = 0;

    cout << "Parallel BFS Traversal : ";

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



// ===== DFS =====

// Sequential DFS
void dfsSequential(vector<vector<int>>& graph, int start, int n) {

    vector<bool> visited(n, false);

    stack<int> s;

    s.push(start);

    visited[start] = true;

    int count = 0;

    cout << "Sequential DFS Traversal : ";

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

    int count = 0;

    cout << "Parallel DFS Traversal : ";

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



    // ===== BFS =====

    start_time = clock();

    bfsSequential(graph, start, n);

    end_time = clock();

    double bfs_seq =
    ((double)(end_time - start_time) * 1000)
    / CLOCKS_PER_SEC;

    cout << "Sequential BFS Time : "
         << bfs_seq << " ms\n";


    start_time = clock();

    bfsParallel(graph, start, n);

    end_time = clock();

    double bfs_par =
    ((double)(end_time - start_time) * 1000)
    / CLOCKS_PER_SEC;

    cout << "Parallel BFS Time : "
         << bfs_par << " ms\n";

    cout << "BFS Speedup : "
         << bfs_seq / bfs_par << "\n\n";



    // ===== DFS =====

    start_time = clock();

    dfsSequential(graph, start, n);

    end_time = clock();

    double dfs_seq =
    ((double)(end_time - start_time) * 1000)
    / CLOCKS_PER_SEC;

    cout << "Sequential DFS Time : "
         << dfs_seq << " ms\n";


    start_time = clock();

    dfsParallel(graph, start, n);

    end_time = clock();

    double dfs_par =
    ((double)(end_time - start_time) * 1000)
    / CLOCKS_PER_SEC;

    cout << "Parallel DFS Time : "
         << dfs_par << " ms\n";

    cout << "DFS Speedup : "
         << dfs_seq / dfs_par << endl;

    return 0;
}