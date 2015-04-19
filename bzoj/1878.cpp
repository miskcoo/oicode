/* BZOJ-1878: [SDOI2009]HH的项链
 *  可持久化线段树 */
#include <cstdio>
 
const int MaxV = 1000001, MaxN = 50010, MaxS = 50010 * 17;
struct node_t
{
    int w;
    node_t *l, *r;
} node[MaxS];
int used, cnt[MaxV];
node_t *root[MaxN], *nil = node + used++;
 
node_t* modify(node_t *now, int head, int tail, int pos)
{
    node_t *n = node + used++;
    *n = *now, ++n->w;
    if(head == tail) return n;
    int m = (head + tail) >> 1;
    if(pos <= m) n->l = modify(n->l, head, m, pos);
    else n->r = modify(n->r, m + 1, tail, pos);
    return n;
}
 
int ask(node_t *l, node_t *r, int head, int tail, int pos)
{
    if(head == tail) return r->w - l->w;
    int m = (head + tail) >> 1;
    if(pos <= m) return ask(l->l, r->l, head, m, pos);
    return ask(l->r, r->r, m + 1, tail, pos) + (r->l->w - l->l->w);
}
 
int main()
{
    root[0] = nil->l = nil->r = nil;
    int n;
    std::scanf("%d", &n);
    for(int i = 1; i <= n; ++i)
    {
        int v;
        std::scanf("%d", &v);
        root[i] = modify(root[i - 1], 0, n, cnt[v]);
        cnt[v] = i;
    }
 
    int q;
    std::scanf("%d", &q);
    for(int i = 0; i != q; ++i)
    {
        int l, r;
        std::scanf("%d %d", &l, &r);
        std::printf("%d\n", ask(root[l - 1], root[r], 0, n, l - 1));
    }
    return 0;
}
