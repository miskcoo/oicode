/* BZOJ-2780: [Spoj]8093 Sevenk Love Oimaster
 *   广义后缀自动机+树状数组   */
#include <cstdio>
#include <algorithm>

const int MaxN = 200010, MaxQ = 360000, MaxAlpha = 26;
struct node_t
{
	int len;
	node_t *fa, *ch[MaxAlpha];
} node[MaxN];

struct graph_t
{
	int total;
	int head[MaxN], point[MaxN * 10], next[MaxN * 10];
	
	void add_edge(int u, int v)
	{
		point[++total] = v;
		next[total] = head[u];
		head[u] = total;
	}
} g, s;

int used = 1;
char str[MaxQ];
node_t *sam_head = node + used++, *sam_tail = sam_head;

void sam_extend(int x)
{
	node_t *p = sam_tail;
	if(p->ch[x])
	{
		if(p->len + 1 == p->ch[x]->len)
		{
			sam_tail = p->ch[x];
		} else {
			node_t *q = p->ch[x], *r = node + used++;
			*r = *q; q->fa = r;
			r->len = p->len + 1;
			for(; p && p->ch[x] == q; p = p->fa)
				p->ch[x] = r;
			sam_tail = r;
		}
	} else {
		node_t *n = node + used++;
		n->len = p->len + 1;
		sam_tail = n;
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
				n->fa = q->fa = r;
				r->len = p->len + 1;
				for(; p && p->ch[x] == q; p = p->fa)
					p->ch[x] = r;
			}
		}
	}
}

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
		for(; x; x -= x & -x)
			v += ta[x];
		return v;
	}
} ta;

struct ques_t
{
	int id, l, r, ans;
	bool operator < (const ques_t& q) const
	{
		return r < q.r;
	}
} ques[MaxQ];

int dfn_index, cnt[MaxN], ans[MaxN];
int enter[MaxN], leave[MaxN], dfn[MaxN];
void dfs(int u, int fa)
{
	dfn[++dfn_index] = u;
	enter[u] = dfn_index;
	for(int k = g.head[u]; k; k = g.next[k])
		if(g.point[k] != fa) dfs(g.point[k], u);
	leave[u] = dfn_index;
}

int main()
{
	int n, q;
	std::scanf("%d %d", &n, &q);
	for(int i = 0; i != n; ++i)
	{
		std::scanf("%s", str);
		sam_tail = sam_head;
		for(int j = 0; str[j]; ++j)
		{
			sam_extend(str[j] - 'a');
			s.add_edge(sam_tail - node, i);
		}
	}

	for(int i = 2; i != used; ++i)
		g.add_edge(node[i].fa - node, i);
	dfs(1, 0);

	for(int i = 0; i != q; ++i)
	{
		std::scanf("%s", str);
		node_t *now = sam_head;
		bool matched = true;
		for(int j = 0; str[j]; ++j)
		{
			int x = str[j] - 'a';
			if(!now->ch[x])
			{
				matched = false;
				break;
			} else {
				now = now->ch[x];
			}
		}

		ques[i].id = i;
		if(!matched)
		{
			ques[i].ans = 0;
			ques[i].l = -1;
		} else {
			int u = now - node;
			ques[i].l = enter[u];
			ques[i].r = leave[u];
		}
	}

	std::sort(ques, ques + q);

	int now = 0;
	while(now != q && ques[now].l == -1) ++now;
	ta.size = dfn_index;
	for(int i = 1; i <= dfn_index && now != q; ++i)
	{
		for(int k = s.head[dfn[i]]; k; k = s.next[k])
		{
			int v = s.point[k];
			if(cnt[v]) ta.modify(cnt[v], -1);
			ta.modify(cnt[v] = i, 1);
		}

		for(; now != q && ques[now].r == i; ++now)
			ques[now].ans = ta.ask(ques[now].r) - ta.ask(ques[now].l - 1);
	}

	for(int i = 0; i != q; ++i)
		ans[ques[i].id] = ques[i].ans;
	for(int i = 0; i != q; ++i)
		std::printf("%d\n", ans[i]);
	return 0;
}
