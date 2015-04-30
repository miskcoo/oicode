/* BZOJ-1984: 月下“毛景树”
 *   树链剖分  */
#include <cstdio>
#include <algorithm>

const int MaxN = 100010, MaxM = MaxN << 1;
int n, total = 1, dfn_index;
int head[MaxN], point[MaxM], next[MaxM], weight[MaxM];
int val[MaxN << 2], lazy_add[MaxN << 2], lazy_set[MaxN << 2];
int size[MaxN], dfn[MaxN], top[MaxN], pval[MaxN], edge_val[MaxN];
int heavy_son[MaxN], depth[MaxN], fa[MaxN], enter[MaxN];

void add_edge(int u, int v, int w)
{
	point[++total] = v;
	weight[total] = w;
	next[total] = head[u];
	head[u] = total;
}

void update_lazy(int now, int add, int set)
{
	if(set != 0)
	{
		lazy_set[now] = set;
		lazy_add[now] = add;
		val[now] = set + add;
	} else {
		lazy_add[now] += add;
		val[now] += add;
	}
}

void pushdown(int now)
{
	if(lazy_set[now] || lazy_add[now])
	{
		update_lazy(now << 1, lazy_add[now], lazy_set[now]);
		update_lazy((now << 1) + 1, lazy_add[now], lazy_set[now]);
		lazy_add[now] = lazy_set[now] = 0;
	}
}

void build(int now, int head, int tail)
{
	if(head == tail)
	{
		val[now] = pval[dfn[head]];
		return;
	}

	int m = (head + tail) >> 1;
	build(now << 1, head, m);
	build((now << 1) + 1, m + 1, tail);
	val[now] = std::max(val[now << 1], val[(now << 1) + 1]);
}

int seg_add, seg_set;
void modify(int now, int head, int tail, int a, int b)
{
	if(head == a && tail == b)
		return update_lazy(now, seg_add, seg_set);
	pushdown(now);
	int m = (head + tail) >> 1;
	int l = now << 1, r = l + 1;
	if(b <= m) modify(l, head, m, a, b);
	else if(m < a) modify(r, m + 1, tail, a, b);
	else {
		modify(l, head, m, a, m);
		modify(r, m + 1, tail, m + 1, b);
	}

	val[now] = std::max(val[l], val[r]);
}

int ask(int now, int head, int tail, int a, int b)
{
	if(head == a && tail == b) return val[now];
	pushdown(now);
	int m = (head + tail) >> 1;
	int l = now << 1, r = l + 1;
	if(b <= m) return ask(l, head, m, a, b);
	else if(m < a) return ask(r, m + 1, tail, a, b);
	return std::max(ask(l, head, m, a, m), ask(r, m + 1, tail, m + 1, b));
}

void dfs_size(int u)
{
	int heavy_size = 0;
	size[u] = 1;
	for(int k = head[u]; k; k = next[k])
	{
		int v = point[k];
		if(v != fa[u]) 
		{
			fa[v] = u;
			depth[v] = depth[u] + 1;
			pval[v] = weight[k];
			edge_val[k >> 1] = v;
			dfs_size(v);
			size[u] += size[v];
			if(heavy_size < size[v])
			{
				heavy_size = size[v];
				heavy_son[u] = v;
			}
		}
	}
}

void dfs(int u)
{
	dfn[++dfn_index] = u;
	enter[u] = dfn_index;

	if(!top[u]) top[u] = top[fa[u]];
	if(heavy_son[u])
	{
		top[heavy_son[u]] = top[u];
		dfs(heavy_son[u]);
	}

	for(int k = head[u]; k; k = next[k])
	{
		int v = point[k];
		if(v != fa[u] && heavy_son[u] != v)
		{
			top[v] = v;
			dfs(v);
		}
	}
}

void tree_modify(int u, int v)
{
	while(top[u] != top[v])
	{
		if(depth[top[u]] < depth[top[v]])
			std::swap(u, v);
		modify(1, 1, n, enter[top[u]], enter[u]);
		u = fa[top[u]];
	}

	if(enter[u] > enter[v]) std::swap(u, v);
	if(u != v) modify(1, 1, n, enter[u] + 1, enter[v]);
}

int tree_ask(int u, int v)
{
	int ans = 0;
	while(top[u] != top[v])
	{
		if(depth[top[u]] < depth[top[v]])
			std::swap(u, v);
		int z = ask(1, 1, n, enter[top[u]], enter[u]);
		if(z > ans) ans = z;
		u = fa[top[u]];
	}

	if(u == v) return ans;
	if(enter[u] > enter[v]) std::swap(u, v);
	int z = ask(1, 1, n, enter[u] + 1, enter[v]);
	return std::max(z, ans);
}

int main()
{
	std::scanf("%d", &n);
	for(int i = 1; i != n; ++i)
	{
		int u, v, w;
		std::scanf("%d %d %d", &u, &v, &w);
		add_edge(u, v, w);
		add_edge(v, u, w);
	}

	dfs_size(1);
	top[1] = 1;
	dfs(1);
	build(1, 1, n);

	char op[10];
	while(std::scanf("%s", op))
	{
		if(*op == 'S') break;
		else if(op[1] == 'h')
		{
			int u, v;
			std::scanf("%d %d", &u, &v);
			seg_set = v, seg_add = 0;
			u = edge_val[u];
			modify(1, 1, n, enter[u], enter[u]);
		} else {
			int u, v;
			std::scanf("%d %d", &u, &v);
			seg_set = seg_add = 0;
			if(*op == 'C')
			{
				std::scanf("%d", &seg_set);
				tree_modify(u, v);
			} else if(*op == 'A') {
				std::scanf("%d", &seg_add);
				tree_modify(u, v);
			} else {
				std::printf("%d\n", tree_ask(u, v));
			}
		}
	}
	return 0;
}
