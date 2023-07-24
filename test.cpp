#include <iostream>
#include <vector>
using namespace std;

int n = 4;
vector<vector<int>> connections = {{0, 1}, {0, 2}, {1, 2}};

vector<vector<int>> edges;
vector<int> used;

void dfs(int u) {
    used[u] = true;
    for (int v : edges[u]) {
        if (!used[v]) {
            dfs(v);
        }
    }
}
int makeConnected(int n, vector<vector<int>>& connections) {
    if (connections.size() < n - 1) {
        return -1;
    }
    edges.resize(n);
    for (const auto& conn : connections) {
        edges[conn[0]].push_back(conn[1]);
        edges[conn[1]].push_back(conn[0]);
    }
    used.resize(n);
    int ans = 0;
    for (int i = 0; i < n; ++i) {
        if (!used[i]) {
            dfs(i);
            ++ans;
        }
    }
    return ans - 1;
}
int main() {
    int result = makeConnected(n, connections);
    cout << result;
    return 0;
}
