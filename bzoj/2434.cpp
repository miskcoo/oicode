/* BZOJ-2434: [Noi2011]阿狸的打字机
 *   AC自动机，Fail树，DFS序，树状数组 */
#include <cstdio>
#include <cstring>

const int MaxN = 100010, MaxAlpha = 26;
struct node_t
{
	int id;
	node_t *fa, *fail;
	node_t *ch[MaxAlpha];
} node[MaxN];
char str[MaxN];
int used = 1, num, dfn_index, dfn[MaxN], leave[MaxN], ans[MaxN];
node_t *root = node + used++;
node_t *que[MaxN], *pos[MaxN];

struct graph_t
{
	int total;
	int head[MaxN], point[MaxN], next[MaxN], weight[MaxN];

	void reset()
	{
		total = 0;
		std::memset(head, 0, sizeof(head));
	}

	void add_edge(int u, int v, int w)
	{
		point[++total] = v;
		weight[total] = w;
		next[total] = head[u];
		head[u] = total;
	}
} gp;

class tree_array
{
	int ta[MaxN];
public:
	int ask(int x)
	{
		int v = 0;
		for(; x; x -= x & -x)
			v += ta[x];
		return v;
	}

	void modify(int x, int v)
	{
		for(; x <= used; x += x & -x)
			ta[x] += v;
	}
} ta;

void make_fail()
{
	int qhead = 0, qtail = 0;
	que[qtail++] = root;
	while(qhead != qtail)
	{
		node_t *n = que[qhead++];
		for(int i = 0; i != MaxAlpha; ++i)
		{
			node_t *z = n->ch[i];
			if(z)
			{
				node_t *p = n->fail;
				while(p && !p->ch[i]) p = p->fail;
				z->fail = p ? p->ch[i] : root;
				gp.add_edge(z->fail - node, z - node, 0);
				que[qtail++] = z;
			}
		}
	}
}

void build()
{
	node_t *now = root;
	for(int i = 0; str[i]; ++i)
	{
		if(str[i] == 'B')
		{
			now = now->fa;
		} else if(str[i] == 'P') {
			now->id = num;
			pos[num++] = now;
		} else {
			int x = str[i] - 'a';
			if(!now->ch[x]) 
			{
				node_t *z = node + used++;
				z->fa = now;
				now->ch[x] = z;
			}

			now = now->ch[x];
		}
	}
}

void dfs(int u)
{
	dfn[u] = ++dfn_index;
	for(int k = gp.head[u]; k; k = gp.next[k])
		dfs(gp.point[k]);
	leave[u] = dfn_index;
}

void solve()
{
	node_t *now = root;
	for(int i = 0; str[i]; ++i)
	{
		if(str[i] == 'B')
		{
			ta.modify(dfn[now - node], -1);
			now = now->fa;
		} else if(str[i] == 'P') {
			int u = now->id + 1;
			for(int k = gp.head[u]; k; k = gp.next[k])
			{
				int v = pos[gp.point[k] - 1] - node;
				ans[gp.weight[k]] = ta.ask(leave[v]) - ta.ask(dfn[v] - 1);
			}
		} else {
			int x = str[i] - 'a';
			now = now->ch[x];
			ta.modify(dfn[now - node], 1);
		}
	}
}

int main()
{
	std::scanf("%s", str);
	build();
	make_fail();
	dfs(root - node);

	int q;
	std::scanf("%d", &q);
	gp.reset();
	for(int i = 0; i != q; ++i)
	{
		int x, y;
		std::scanf("%d %d", &x, &y);
		gp.add_edge(y, x, i);
	}

	solve();
	for(int i = 0; i != q; ++i)
		std::printf("%d\n", ans[i]);
	return 0;
}
