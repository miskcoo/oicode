/* BZOJ-4034: [HAOI2015]T2
 *   线段树  */
#include <cstdio>
 
const int MaxN = 400010;
int total, dfn_index;
int head[MaxN], point[MaxN], next[MaxN], weight[MaxN];
int dfn[MaxN], enter[MaxN], leave[MaxN];
long long range_add[MaxN], depth_add[MaxN];
long long dist[MaxN], depth[MaxN];
 
void add_edge(int u, int v)
{
    point[++total] = v;
    next[total] = head[u];
    head[u] = total;
}
 
void dfs(int u, int fa)
{
    dfn[++dfn_index] = u;
    enter[u] = dfn_index;
    for(int k = head[u]; k; k = next[k])
    {
        if(point[k] != fa)
        {
            dist[point[k]] += dist[u];
            depth[point[k]] = depth[u] + 1;
            dfs(point[k], u);
        }
    }
    leave[u] = dfn_index;
}
 
long long m1, m2;
void modify(int now, int head, int tail, int a, int b)
{
    if(head == a && tail == b)
    {
        range_add[now] += m1;
        depth_add[now] += m2;
        return;
    }
 
    int m = (head + tail) >> 1;
    int l = now << 1, r = l + 1;
    if(b <= m) modify(l, head, m, a, b);
    else if(m < a) modify(r, m + 1, tail, a, b);
    else {
        modify(l, head, m, a, m);
        modify(r, m + 1, tail, m + 1, b);
    }
}
 
long long ask(int now, int head, int tail, int pos)
{
    long long ans = range_add[now] + depth_add[now] * depth[dfn[pos]];
    if(head == tail) return ans;
    int m = (head + tail) >> 1;
    if(pos <= m) return ask(now << 1, head, m, pos) + ans;
    return ask((now << 1) + 1, m + 1, tail, pos) + ans;
}
 
int main()
{
    int n, m;
    std::scanf("%d %d", &n, &m);
    for(int i = 1; i <= n; ++i)
        std::scanf("%lld", dist + i);
    for(int i = 1; i != n; ++i)
    {
        int u, v;
        std::scanf("%d %d", &u, &v);
        add_edge(u, v);
        add_edge(v, u);
    }
 
    dfs(1, 0);
    for(int i = 0; i != m; ++i)
    {
        int op, x;
        long long a;
        std::scanf("%d %d", &op, &x);
        if(op == 3)
        {
            std::printf("%lld\n", ask(1, 1, n, enter[x]) + dist[x]);
        } else {
            std::scanf("%lld", &a);
            if(op == 1)
            {
                m1 = a;
                m2 = 0;
            } else {
                m1 = -(depth[x] - 1) * a;
                m2 = a;
            }
 
            modify(1, 1, n, enter[x], leave[x]);
        }
    }
    return 0;
}
