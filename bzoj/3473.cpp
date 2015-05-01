/* BZOJ-3473: 字符串
 *   广义后缀自动机+树状数组 */
#include <cstdio>
#include <cstring>
#include <algorithm>

const int MaxN = 200010, MaxAlpha = 26;
struct node_t
{
	int len, w;
	node_t *fa, *ch[MaxAlpha];
} node[MaxN];
char str[MaxN];
int used = 1, dfn_index;
node_t *sam_head = node + used++;
int enter[MaxN], leave[MaxN], cnt[MaxN], dfn[MaxN];

node_t* sam_extend(node_t *tail, int x)
{
	node_t *n = tail->ch[x];
	if(n)
	{
		if(tail->len + 1 == n->len)
			return n;
		node_t *r = node + used++;
		*r = *n; n->fa = r;
		r->len = tail->len + 1;
		for(node_t *p = tail; p && p->ch[x] == n; p = p->fa)
			p->ch[x] = r;
		return r;
	} else {
		node_t *p = tail, *n = node + used++;
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
				*r = *q; r->len = p->len + 1;
				q->fa = n->fa = r;
				for(; p && p->ch[x] == q; p = p->fa)
					p->ch[x] = r;
			}
		}

		return n;
	}

	return 0;
}

struct ques_t
{
	int id;
	bool operator < (const ques_t& q) const
	{
		return leave[id] < leave[q.id];
	}
} ques[MaxN];

struct graph_t
{
	int total;
	int head[MaxN], point[MaxN << 1], next[MaxN << 1];

	void add_edge(int u, int v)
	{
		point[++total] = v;
		next[total] = head[u];
		head[u] = total;
	}
} s, g, gp;

struct tree_array
{
	int size, ta[MaxN];

	void modify(int x, int v)
	{
		for(; x <= size; x += x & -x)
			ta[x] += v;
	}

	int ask(int x)
	{
		int v = 0;
		if(!x) return v;
		for(; x; x -= x & -x)
			v += ta[x];
		return v;
	}
} ta;

void dfs(int u)
{
	dfn[++dfn_index] = u;
	enter[u] = dfn_index;
	for(int k = gp.head[u]; k; k = gp.next[k])
		dfs(gp.point[k]);
	leave[u] = dfn_index;
}

void dfs2(int u)
{
	if(node[u].fa) node[u].w += node[u].fa->w;
	for(int k = gp.head[u]; k; k = gp.next[k])
		dfs2(gp.point[k]);
}

int main()
{
	int n, times;
	std::scanf("%d %d", &n, &times);
	for(int i = 1; i <= n; ++i)
	{
		node_t *now = sam_head;
		std::scanf("%s", str);
		int len = std::strlen(str);
		for(int j = len - 1; j >= 0; --j)
		{
			now = sam_extend(now, str[j] - 'a');
			s.add_edge(now - node, i);
			g.add_edge(i, now - node);
		}
	}

	for(int i = 2; i != used; ++i)
		gp.add_edge(node[i].fa - node, i);
	dfs(1);

	for(int i = 1; i != used; ++i)
		ques[i].id = i;
	ta.size = used;
	std::sort(ques + 1, ques + used);
	for(int i = 1, q = 1; i != used; ++i)
	{
		for(int k = s.head[dfn[i]]; k; k = s.next[k])
		{
			int v = s.point[k];
			if(cnt[v]) ta.modify(cnt[v], -1);
			ta.modify(cnt[v] = i, 1);
		}

		for(; q != used && leave[ques[q].id] == i; ++q)
		{
			int id = ques[q].id;
			node_t *n = node + id;
			int v = ta.ask(leave[id]) - ta.ask(enter[id] - 1);
			if(n->fa) n->w = (v >= times) ? n->len - n->fa->len : 0;
			else n->w = 0;
		}
	}

	dfs2(1);
	for(int i = 1; i <= n; ++i)
	{
		long long ans = 0;
		for(int k = g.head[i]; k; k = g.next[k])
			ans += node[g.point[k]].w;
		std::printf("%lld ", ans);
	}
	return 0;
}
