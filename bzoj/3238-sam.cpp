/* BZOJ-3238: [Ahoi2013]差异
 *  后缀自动机构造后缀树 */
#include <cstdio>
#include <cstring>

const int MaxN = 1000010, MaxAlpha = 26;
struct node_t
{
	int len;
	node_t *fa, *ch[MaxAlpha];
} node[MaxN];
char str[MaxN];
int total, used = 1;
node_t *sam_head = node, *sam_tail = sam_head;
int head[MaxN], next[MaxN], point[MaxN], weight[MaxN], size[MaxN];

void sam_extend(int x)
{
	node_t *p = sam_tail, *n = node + used++;
	size[used] = 1;
	sam_tail = n;
	n->len = p->len + 1;
	for(; p && !p->ch[x]; p = p->fa)
		p->ch[x] = n;
	if(!p)
	{
		n->fa = sam_head;
	} else {
		if(p->len + 1 == p->ch[x]->len)
		{
			n->fa = p->ch[x];
		} else {
			node_t *q = p->ch[x], *r = node + used++;
			*r = *q;
			r->len = p->len + 1;
			q->fa = n->fa = r;
			for(; p && p->ch[x] == q; p = p->fa)
				p->ch[x] = r;
		}
	}
}

void add_edge(int u, int v, int w)
{
	point[++total] = v;
	weight[total] = w;
	next[total] = head[u];
	head[u] = total;
}

long long ans = 0;
void dfs(int now, int d)
{
	for(int k = head[now]; k; k = next[k])
	{
		int v = point[k];
		dfs(v, weight[k] + d);
		ans -= 2ll * size[v] * size[now] * d;
		size[now] += size[v];
	}
}

int main()
{
	std::scanf("%s", str);
	int len = std::strlen(str);
	for(int i = len - 1; i >= 0; --i)
		sam_extend(str[i] - 'a');

	for(int i = 1; i != used; ++i)
		add_edge(node[i].fa - node + 1, i + 1, node[i].len - node[i].fa->len);
	ans += (long long)len * (len - 1) * (len + 1) / 2;
	dfs(1, 0);
	std::printf("%lld", ans);
	return 0;
}
