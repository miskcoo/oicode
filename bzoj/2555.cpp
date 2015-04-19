/* BZOJ-2555: SubString
 *   后缀自动机+LCT */
#include <cstdio>
#include <cstring>
#include <algorithm>
 
const int MaxN = 1200100, MaxAlpha = 26;
char str[MaxN];
 
int mask = 0;
void get_string(int mask)
{
    std::scanf("%s", str);
    int len = std::strlen(str);
    for(int i = 0; str[i]; ++i)
    {
        mask = (mask * 131 + i) % len;
        char t = str[i];
        str[i] = str[mask];
        str[mask] = t;
    }
}
 
class link_cut_tree
{
public:
    int fa[MaxN], son[MaxN][2];
    int w[MaxN], lazy[MaxN], rev[MaxN];
 
    bool not_root(int u)
    {
        int f = fa[u];
        return son[f][0] == u || son[f][1] == u;
    }
 
    void add(int u, int x)
    {
        if(u) lazy[u] += x, w[u] += x;
    }
 
    void pushdown(int u)
    {
        int &l = son[u][0], &r = son[u][1];
        if(rev[u])
        {
            rev[l] ^= 1;
            rev[r] ^= 1;
            rev[u] = 0;
            std::swap(l, r);
        } 
 
        if(lazy[u])
        {
            add(l, lazy[u]);
            add(r, lazy[u]);
            lazy[u] = 0;
        }
    }
 
    void rotate(int u)
    {
        int f = fa[u], p = son[f][0] == u;
        son[f][!p] = son[u][p];
        if(son[u][p]) fa[son[u][p]] = f;
        if(not_root(f)) 
            son[fa[f]][son[fa[f]][0] != f] = u;
        fa[u] = fa[f];
        fa[son[u][p] = f] = u;
    }
 
    void clear_mark(int u)
    {
        if(not_root(u))
            clear_mark(fa[u]);
        pushdown(u);
    }
 
    void splay(int u)
    {
        for(clear_mark(u); not_root(u); rotate(u))
        {
            int f = fa[u];
            if(not_root(f))
                rotate((son[f][0] == u) ^ (son[fa[f]][0] == f) ? u : f);
        }
    }
 
    void access(int u)
    {
        int prev = 0;
        while(u)
        {
            splay(u);
            son[u][1] = prev;
            prev = u;
            u = fa[u];
        }
    }
 
    void make_root(int u)
    {
        access(u);
        splay(u);
        rev[u] ^= 1;
    }
 
    void link(int u, int v)
    {
        make_root(u);
        fa[u] = v;
        modify(v, w[u]);
    }
 
    void cut(int u, int v)
    {
        make_root(u);
        access(v);
        splay(v);
        son[v][0] = fa[u] = 0;
        modify(v, -w[u]);
    }
 
    void modify(int v, int w)
    {
        make_root(1);
        access(v);
        splay(v);
        add(v, w);
    }
} lct;
 
class suffix_automaton
{
    struct node_t
    {
        int id, len;
        node_t *fa, *ch[MaxAlpha];
    } node[MaxN];
 
    int used;
    node_t *head, *tail;
public:
    suffix_automaton() : used(1), head(node), tail(node) 
    {
        head->id = 1;
    }
 
    void extend(int x)
    {
        node_t *p = tail, *n = node + used++;
        tail = n, n->id = used;
        n->len = p->len + 1;
        lct.w[n->id] = 1;
        for(; p && !p->ch[x]; p = p->fa)
            p->ch[x] = n;
        if(!p)
        {
            n->fa = head;
            lct.link(n->id, head->id);
        } else {
            if(p->len + 1 == p->ch[x]->len)
            {
                n->fa = p->ch[x];
                lct.link(n->id, p->ch[x]->id);
            } else {
                node_t *q = p->ch[x], *r = node + used++;
                *r = *q;
                r->id = used, r->len = p->len + 1;
                lct.link(r->id, r->fa->id);
                lct.link(n->id, r->id);
                lct.cut(q->id, q->fa->id);
                lct.link(q->id, r->id);
                q->fa = n->fa = r;
                for(; p && p->ch[x] == q; p = p->fa)
                    p->ch[x] = r;
            }
        }
    }
 
    void build()
    {
        std::scanf("%s", str);
        for(int i = 0; str[i]; ++i)
            extend(str[i] - 'A');
    }
 
    void add()
    {
        get_string(mask);
        for(int i = 0; str[i]; ++i)
            extend(str[i] - 'A');
    }
 
    int ask()
    {
        get_string(mask);
        node_t *now = head;
        for(int i = 0; str[i]; ++i)
        {
            int x = str[i] - 'A';
            if(!now->ch[x]) return 0;
            now = now->ch[x];
        }
 
        lct.splay(now->id);
        return lct.w[now->id];
    }
} sam;
 
int main()
{
    int q;
    std::scanf("%d", &q);
    sam.build();
    for(int i = 0; i != q; ++i)
    {
        char op[6];
        std::scanf("%s", op);
        if(*op == 'Q')
        {
            int ans = sam.ask();
            mask ^= ans;
            std::printf("%d\n", ans);
        } else {
            sam.add();
        }
    }
}
