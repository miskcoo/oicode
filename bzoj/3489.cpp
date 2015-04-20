/* BZOJ-3489: A simple rmq problem
 *   主席树套线段树 */
#include <cstdio>
#include <cstring>
#include <algorithm>
 
template<typename Type>
class memory_pool
{
    int used, remain;
    Type* data[1 << 16];
public:
    memory_pool() : used(0), remain(0) {}
    ~memory_pool() 
    {
        for(int i = 0; i != used; ++i)
            delete[] data[i];
    }
 
    Type* get()
    {
        if(remain == 0) 
            data[used++] = new Type[remain = 1 << 16];
        return data[used - 1] + --remain;
    }
};
 
struct seg_t
{
    int v;
    seg_t *l, *r;
};
 
memory_pool<seg_t> seg_mem;
seg_t *seg_nil;
 
seg_t* seg_modify(seg_t *now, int head, int tail, int pos, int v)
{
    seg_t *n = seg_mem.get();
    *n = *now;
    if(n->v < v) n->v = v;
    if(head == tail) return n;
    int m = (head + tail) >> 1;
    if(pos <= m) n->l = seg_modify(n->l, head, m, pos, v);
    else n->r = seg_modify(n->r, m + 1, tail, pos, v);
    return n;
}
 
int seg_ask(seg_t *now, int head, int tail, int a, int b)
{
    if(now == seg_nil) return 0;
    if(head == a && tail == b) return now->v;
    int m = (head + tail) >> 1;
    if(b <= m) return seg_ask(now->l, head, m, a, b);
    if(m < a) return seg_ask(now->r, m + 1, tail, a, b);
    return std::max(seg_ask(now->l, head, m, a, m),
            seg_ask(now->r, m + 1, tail, m + 1, b));
}
 
struct node_t
{
    seg_t *seg;
    node_t *l, *r;
};
 
int n, m;
memory_pool<node_t> mem;
node_t *nil;
 
int ask(node_t *now, int head, int tail, int pos, int nx, int ny)
{
    if(head == tail) return seg_ask(now->seg, 1, n, nx, ny);
    int m = (head + tail) >> 1;
    if(m < pos) return ask(now->r, m + 1, tail, pos, nx, ny);
    return std::max(ask(now->l, head, m, pos, nx, ny), 
			seg_ask(now->r->seg, 1, n, nx, ny));
}
 
node_t* modify(node_t *now, int head, int tail, int pos, int npos, int v)
{
    node_t *z = mem.get();
    *z = *now;
    z->seg = seg_modify(z->seg, 1, n, npos, v);
    if(head == tail) return z;
    int m = (head + tail) >> 1;
    if(pos <= m) z->l = modify(z->l, head, m, pos, npos, v);
    else z->r = modify(z->r, m + 1, tail, pos, npos, v);
    return z;
}
 
void init()
{
    static seg_t seg_base;
    seg_nil = &seg_base;
    seg_nil->l = seg_nil->r = seg_nil;
    seg_nil->v = 0;
 
    static node_t node_base;
    nil = &node_base;
    nil->l = nil->r = nil;
    nil->seg = seg_nil;
}
 
const int MaxN = 100010;
int cnt[MaxN], data[MaxN];
struct prev_t
{
    int prev, next, pos;
    bool operator < (const prev_t& v) const
    {
        return prev < v.prev;
    }
} prev[MaxN];
node_t *root[MaxN];
 
int main()
{
    init();
    std::scanf("%d %d", &n, &m);
    for(int i = 1; i <= n; ++i)
    {
        std::scanf("%d", data + i);
        prev[i].prev = cnt[data[i]];
        prev[i].pos = i;
        cnt[data[i]] = i;
    }
 
    for(int i = 0; i <= n; ++i)
        cnt[i] = n + 1;
    for(int i = n; i; --i)
    {
        prev[i].next = cnt[data[i]];
        cnt[data[i]] = i;
    }
 
    std::sort(prev + 1, prev + n + 1);
    root[0] = nil;
    for(int i = 0, j = 1; i <= n + 1; ++i)
    {
        if(i) root[i] = root[i - 1];
        while(j <= n && prev[j].prev == i)
        {
            root[i] = modify(root[i], 0, n + 1, 
                prev[j].next, prev[j].pos, data[prev[j].pos]);
            ++j;
        }
    }
 
    int lastans = 0;
    for(int i = 0; i != m; ++i)
    {
        int x, y;
        std::scanf("%d %d", &x, &y);
        x = (x + lastans) % n + 1;
        y = (y + lastans) % n + 1;
        int l = std::min(x, y), r = std::max(x, y);
        lastans = ask(root[l - 1], 0, n + 1, r + 1, l, r);
        std::printf("%d\n", lastans);
    }
    return 0;
}
