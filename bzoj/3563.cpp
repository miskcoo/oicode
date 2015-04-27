/* BZOJ-3563: DZY Loves Chinese
 *   随机化  */
#include <cstdio>
#include <algorithm>
#include <cstdlib>
#include <cstring>
 
const int MaxN = 100010, MaxM = 1000010, MaxK = 15;
int total;
int head[MaxN], next[MaxM], point[MaxM];
int edge_id[MaxM], xor_v[MaxN], mark[MaxN];
 
struct edge_t
{
    int id, u, v, w, mark;
} edge[MaxM];
 
void add_edge(int u, int v, int id)
{
    point[++total] = v;
    next[total] = head[u];
    edge_id[total] = id;
    head[u] = total;
}
 
void dfs(int u, int fa)
{
    int z = 0;
    mark[u] = 1;
    for(int k = head[u]; k; k = next[k])
    {
        int v = point[k];
        if(edge[edge_id[k]].mark) continue;
        if(v != fa && !mark[v])
        {
            dfs(v, u);
            z ^= xor_v[v];
            edge[edge_id[k]].w = xor_v[v];
        } else if(v != fa) {
            int w = std::rand() * std::rand();
            if(w < 0) w = -w;
            edge[edge_id[k]].w = w;
            xor_v[point[k]] ^= w;
            xor_v[u] ^= w;
        }
         
        edge[edge_id[k]].mark = 1;
    }
 
    mark[u] = 2;
    xor_v[u] ^= z;
}
 
int main()
{
    int n, m;
    std::scanf("%d %d", &n, &m);
    for(int i = 0; i != m; ++i)
    {
        int u, v;
        std::scanf("%d %d", &u, &v);
        edge[i].u = u;
        edge[i].v = v;
        edge[i].id = i;
 
        add_edge(u, v, i);
        add_edge(v, u, i);
    }
 
    dfs(1, 0);
 
    int q, count = 0;
    std::scanf("%d", &q);
    while(q --> 0)
    {
        int num, d[MaxK], tmp[32];
        std::memset(tmp, 0, sizeof(tmp));
        std::scanf("%d", &num);
        num ^= count;
        for(int i = 0; i != num; ++i)
        {
            int v;
            std::scanf("%d", &v);
            d[i] = edge[(v ^ count) - 1].w;
        }
 
        int j, k = 0;
        for(int i = 1 << 30; i; i >>= 1)
        {
            for(j = k; j != num; ++j)
                if(d[j] & i) break;
            if(j == num) continue;
            std::swap(d[j], d[k++]);
            for(j = k; j != num; ++j)
                if(d[j] & i) d[j] ^= d[k - 1];
        }
 
        if(!d[num - 1]) std::puts("Disconnected");
        else std::puts("Connected"), ++count;
    }
    return 0;
}
