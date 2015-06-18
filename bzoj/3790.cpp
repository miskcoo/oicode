/* BZOJ-3790: 神奇项链
 *   回文自动机+贪心 */
#include <cstdio>
#include <cstring>
#include <algorithm>
 
const int MaxAlpha = 26, MaxN= 50010;
 
struct node_t
{
    int len;
    node_t *fa, *ch[MaxAlpha];
} node[MaxN];
node_t *odd, *even, *tail;
int used;
 
node_t* get_node()
{
    node_t *n = node + used++;
    std::memset(n->ch, 0, sizeof(n->ch));
    n->fa = 0, n->len = 0;
    return n;
}
 
void pam_init()
{
    used = 0;
    even = get_node();
    odd = get_node();
    even->len = 0, odd->len = -1;
    even->fa = odd->fa = odd;
    tail = odd;
}
 
int pam_extend(const char* str, int x)
{
    node_t *p = tail;
 
    int ch = str[x] - 'a';
    while(str[x - p->len - 1] != str[x]) p = p->fa;
    if(!p->ch[ch])
    {
        node_t *n = get_node();
        n->len = p->len + 2;
        node_t *q = p->fa;
        while(str[x - q->len - 1] != str[x]) q = q->fa;
        n->fa = q->ch[ch];
        if(!n->fa) n->fa = even;
        p->ch[ch] = n;
    }
 
    tail = p->ch[ch];
    return p->ch[ch]->len;
}
 
int total;
int head[MaxN], next[MaxN], point[MaxN];
char str[MaxN];
 
void add_edge(int u, int v)
{
//  printf("(%d, %d)\n", u, v);
    point[++total] = v;
    next[total] = head[u];
    head[u] = total;
}
 
int main()
{
//  std::freopen("necklace.in", "r", stdin);
//  std::freopen("necklace.out", "w", stdout);
    while(std::scanf("%s", str) != EOF)
    {
        total = 0;
        std::memset(head, 0, sizeof(head));
        pam_init();
 
        for(int i = 0; str[i]; ++i)
            add_edge(i - pam_extend(str, i) + 2, i + 1);
 
        int rm = 0, rmost = 1, cnt = 0;
        int len = std::strlen(str);
        for(int i = 1; i <= len; ++i)
        {
            for(int k = head[i]; k; k = next[k])
                rm = std::max(rm, point[k]);
            if(i == rmost) 
                rmost = rm + 1, ++cnt;
        }
 
        std::printf("%d\n", cnt - 1);
    }
    return 0;
}
