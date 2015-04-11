/* BZOJ-3757: 苹果树
 *  树分块，树上莫队 */
#include <cstdio>
#include <cmath>
#include <algorithm>
 
const int MaxN = 50001, MaxM = 100001, MaxL = 17;
int n, q, total, block_size, block_num, stop, dfs_index;
int head[MaxN], point[MaxM], next[MaxM], stack[MaxN];
int depth[MaxN], dist[MaxL][MaxN], block[MaxN], dfn[MaxN];
int color_num, count[MaxN], color[MaxN], mark[MaxN], ans[MaxM];
 
struct ques_t
{
    int id;
    int u, v, a, b;
 
    bool operator < (const ques_t& r) const
    {
        if(block[u] == block[r.u])
            return dfn[v] < dfn[r.v];
        return block[u] < block[r.u];
    }
} ques[MaxM];
 
void add_edge(int u, int v)
{
    point[++total] = v;
    next[total] = head[u];
    head[u] = total;
}
 
int dfs(int u, int fa, int d)
{
    int size = 1;
    depth[u] = d;
    dist[0][u] = fa;
    stack[stop++] = u;
    dfn[u] = ++dfs_index;
 
    for(int l = 1; l != MaxL; ++l)
        dist[l][u] = dist[l - 1][dist[l - 1][u]];
 
    for(int k = head[u]; k; k = next[k])
    {
        int v = point[k];
        if(v == fa) continue;
        size += dfs(v, u, d + 1);
        if(size > block_size)
        {
            for(++block_num; stack[stop - 1] != u; --size)
                block[stack[--stop]] = block_num;
        }
    }
 
    return size;
}
 
int get_lca(int u, int v)
{
    if(depth[u] < depth[v])
        std::swap(u, v);
    int d = depth[u] - depth[v];
    for(int l = 0; d; ++l, d >>= 1)
        if(d & 1) u = dist[l][u];
 
    for(int p = MaxL - 1; u != v; p ? --p : 0)
    {
        if(dist[p][u] != dist[p][v] || p == 0)
        {
            u = dist[p][u];
            v = dist[p][v];
        }
    }
 
    return u;
}
 
void toggle(int u)
{
    if(!mark[u]) 
    {
        if(!count[color[u]]++)
            ++color_num;
    } else {
        if(!--count[color[u]])
            --color_num;
    }
 
    mark[u] ^= 1;
}
 
void move(int u, int v)
{
    while(u != v)
    {
        if(depth[u] > depth[v]) 
        {
            toggle(u);
            u = dist[0][u];
        } else {
            toggle(v);
            v = dist[0][v];
        }
    }
}
 
int main()
{
    std::scanf("%d %d", &n, &q);
    for(int i = 1; i <= n; ++i)
        std::scanf("%d", color + i);
    for(int i = 0; i != n; ++i)
    {
        int u, v;
        std::scanf("%d %d", &u, &v);
        if(!u || !v) continue;
        add_edge(u, v);
        add_edge(v, u);
    }
 
    block_size = (int)std::sqrt(double(n)) + 1;
    dfs(1, 0, 1);
    for(++block_num; stop; --stop)
        block[stack[stop - 1]] = block_num;
 
    for(int i = 0; i != q; ++i)
    {
        int u, v, a, b;
        std::scanf("%d %d %d %d", &u, &v, &a, &b);
        ques[i].id = i;
        ques[i].u = u, ques[i].v = v;
        ques[i].a = a, ques[i].b = b;
    }
 
    std::sort(ques, ques + q);
 
    int pu = 1, pv = 1;
    for(int i = 0; i != q; ++i)
    {
        move(pu, ques[i].u);
        move(pv, ques[i].v);
        int lca = get_lca(ques[i].u, ques[i].v);
        toggle(lca);
        ans[ques[i].id] = color_num;
        if(ques[i].a != ques[i].b && count[ques[i].a] && count[ques[i].b])
            --ans[ques[i].id];
        toggle(lca);
 
        pu = ques[i].u;
        pv = ques[i].v;
    }
 
    for(int i = 0; i != q; ++i)
        std::printf("%d\n", ans[i]);
    return 0;
}
