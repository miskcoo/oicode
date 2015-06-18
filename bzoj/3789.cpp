/* BZOJ-3789: 扫雪车
 *   上下界最大流 */
#include <cstdio>
#include <cstring>
#include <algorithm>
 
const int MaxN = 110, MaxM = 40000, inf = ~0u >> 1;
int S, T, nomark, total = 1;
int head[MaxN], next[MaxM], point[MaxM], cap[MaxM];
int cur[MaxN], flag[MaxN], que[MaxN], mark[MaxM];
int flow[MaxN][MaxN], remain[MaxN][MaxN];
 
void add_edge0(int u, int v, int f, int m0 = 0)
{
    point[++total] = v;
    next[total] = head[u];
    cap[total] = f;
    mark[total] = m0;
    head[u] = total;
}
 
void add_edge(int u, int v, int f, int m0 = 0)
{
    add_edge0(u, v, f, m0);
    add_edge0(v, u, 0, m0);
}
 
bool bfs()
{
    int qhead = 0, qtail = 0;
    std::memset(flag, 0, sizeof(flag));
    que[qtail++] = T, flag[T] = 1;
    while(qhead != qtail)
    {
        int u = que[qhead++];
        for(int k = head[u]; k; k = next[k])
        {
            int v = point[k];
            if(nomark && mark[k]) continue;
            if(cap[k ^ 1] && !flag[v])
            {
                flag[v] = flag[u] + 1;
                que[qtail++] = v;
            }
        }
    }
 
    return flag[S];
}
 
int dfs(int u, int rest)
{
    if(u == T) return rest;
    int used = 0;
    for(int& k = cur[u]; k; k = next[k])
    {
        int v = point[k];
        if(nomark && mark[k]) continue;
        if(cap[k] && flag[v] + 1 == flag[u])
        {
            int g = std::min(rest - used, cap[k]);
            g = dfs(v, g);
            used += g;
            cap[k] -= g;
            cap[k ^ 1] += g;
            if(used == rest) return used;
        }
    }
 
    return used;
}
 
int dinic()
{
    int ans = 0;
    while(bfs())
    {
        std::memcpy(cur, head, sizeof(head));
        ans += dfs(S, inf);
    }
 
    return ans;
}
 
int main()
{
    //std::freopen("snow.in", "r", stdin);
    //std::freopen("snow.out", "w", stdout);
    int n, m, s0, t0, sum = 0;
    std::scanf("%d %d %d %d", &n, &m, &s0, &t0);
    S = n + 1, T = S + 1;
    for(int i = 0; i != m; ++i)
    {
        int x, y, w, t;
        std::scanf("%d %d %d %d", &x, &y, &w, &t);
        flow[x][y] += w;
        if(t)
        {
            sum += w;
            add_edge(x, T, w, 1);
            add_edge(S, y, w, 1);
        } else {
            add_edge(x, y, w);
        } 
    }
 
    int id = total + 1;
    add_edge(t0, s0, inf, 1);
    if(dinic() != sum)
    {
        std::puts("0");
        return 0;
    } 
 
    nomark = 1;
    S = s0, T = t0;
    int ans = cap[id ^ 1];
    ans += dinic();
 
    std::printf("%d\n", ans); 
    //output_way(n, ans);
    return 0;
}
