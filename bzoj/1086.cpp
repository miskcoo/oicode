/* BZOJ-1086: [SCOI2005]王室联邦
 *  DFS，树分块 */
#include <cstdio>
 
const int MaxN = 1001, MaxM = 2001;
int n, b, total, stop, count;
int head[MaxN], next[MaxM], point[MaxM], st[MaxN];
int belong[MaxN], map[MaxN];
 
void add_edge(int u, int v)
{
    point[++total] = v;
    next[total] = head[u];
    head[u] = total;
}
 
int dfs(int u, int fa)
{
    int size = 1;
    st[stop++] = u;
    for(int k = head[u]; k; k = next[k])
    {
        int v = point[k];
        if(v == fa) continue;
        size += dfs(v, u);
        if(size > b)
        {
            map[++count] = u;
            while(st[stop - 1] != u) 
            {
                belong[st[--stop]] = count;
                --size;
            }
        }
    }
 
    return size;
}
 
int main()
{
    std::scanf("%d %d", &n, &b);
    for(int i = 1; i != n; ++i)
    {
        int u, v;
        std::scanf("%d %d", &u, &v);
        add_edge(u, v);
        add_edge(v, u);
    }
 
    dfs(1, 0);
    if(n < b)
    {
        std::puts("0");
    } else {
        std::printf("%d\n", count);
        for(int i = 1; i <= n; ++i)
            std::printf("%d ", belong[i] ? belong[i] : count);
        std::printf("\n");
        for(int i = 1; i <= count; ++i)
            std::printf("%d ", map[i]);
    }
    return 0;
}
