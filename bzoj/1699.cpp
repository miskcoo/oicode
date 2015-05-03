/* BZOJ-1699: [Usaco2007 Jan]Balanced Lineup排队
 *   ST表  */
#include <cstdio>
#include <algorithm>
 
const int MaxL = 16, MaxN = 50010;
int data[MaxN], log2[MaxN];
int st_max[MaxL][MaxN], st_min[MaxL][MaxN];
int ans_max, ans_min;
 
void init_st(int n)
{
    for(int i = 1; i <= n; ++i)
        st_max[0][i] = data[i];
    for(int l = 1; (1 << l) < n; ++l)
        for(int i = 1; i <= n; ++i)
            st_max[l][i] = std::max(st_max[l - 1][i], st_max[l - 1][i + (1 << (l - 1))]);
 
    for(int i = 1; i <= n; ++i)
        st_min[0][i] = data[i];
    for(int l = 1; (1 << l) < n; ++l)
        for(int i = 1; i <= n; ++i)
            st_min[l][i] = std::min(st_min[l - 1][i], st_min[l - 1][i + (1 << (l - 1))]);
 
    log2[1] = 0;
    for(int i = 2; i <= n; ++i)
    {
        if(i - (i & -i))
            log2[i] = log2[i - 1];
        else log2[i] = log2[i - 1] + 1;
    }
}
 
void ask(int a, int b)
{
    int d = b - a + 1;
    int l = log2[d];
    ans_max = std::max(st_max[l][a], st_max[l][b - (1 << l) + 1]);
    ans_min = std::min(st_min[l][a], st_min[l][b - (1 << l) + 1]);
}
 
int main()
{
    int n, q;
    std::scanf("%d %d", &n, &q);
    for(int i = 1; i <= n; ++i)
        std::scanf("%d", data + i);
    init_st(n);
    while(q --> 0)
    {
        int a, b;
        std::scanf("%d %d", &a, &b);
        ask(a, b);
        std::printf("%d\n", ans_max - ans_min);
    }
    return 0;
}
