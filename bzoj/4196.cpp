/* BZOJ-4196: [Noi2015]软件包管理器 */
#include <cstdio>
#include <cstring>

const int MaxN = 100100;

int n, total, dfn_index;
int head[MaxN], next[MaxN], point[MaxN], fa[MaxN], height[MaxN];
int enter[MaxN], leave[MaxN], top[MaxN], size[MaxN], max_son[MaxN];

void add_edge(int u, int v)
{
	point[++total] = v;
	next[total] = head[u];
	head[u] = total;
}

int dfs_size(int u)
{
	size[u] = 1;
	for(int k = head[u]; k; k = next[k])
	{
		height[point[k]] = height[u] + 1;
		size[u] += dfs_size(point[k]);
	}

	return size[u];
}

void dfs_link(int u, bool flag)
{
	enter[u] = ++dfn_index;
	int k;
	if(flag) top[u] = top[fa[u]];
	else top[u] = u;

	max_son[u] = head[u];
	for(k = head[u]; k; k = next[k])
		if(size[max_son[u]] < size[point[k]])
			max_son[u] = point[k];
	if(max_son[u]) dfs_link(max_son[u], true);

	for(k = head[u]; k; k = next[k])
		if(point[k] != max_son[u])
			dfs_link(point[k], false);
	leave[u] = dfn_index;
}

int sum[MaxN * 4], lazy[MaxN * 4];

void pushdown(int now, int head, int tail)
{
	if(lazy[now] == -1) return;
	int m = (head + tail) >> 1;
	int l = now << 1, r = l + 1;

	lazy[l] = lazy[r] = lazy[now];
	sum[l] = lazy[now] * (m - head + 1);
	sum[r] = lazy[now] * (tail - m);

	lazy[now] = -1;
}

int modify_val;
void modify(int now, int head, int tail, int a, int b)
{
	if(head == a && tail == b)
	{
		lazy[now] = modify_val;
		sum[now] = (b - a + 1) * modify_val;
		return;
	}

	pushdown(now, head, tail);
	int m = (head + tail) >> 1;
	int l = now << 1, r = l + 1;

	if(b <= m) 
	{
		modify(l, head, m, a, b);
	} else if(m < a) {
		modify(r, m + 1, tail, a, b);
	} else {
		modify(l, head, m, a, m);
		modify(r, m + 1, tail, m + 1, b);
	}

	sum[now] = sum[l] + sum[r];
}

int ask(int now, int head, int tail, int a, int b)
{
	if(head == a && tail == b)
		return sum[now];

	pushdown(now, head, tail);
	int m = (head + tail) >> 1;
	int l = now << 1, r = l + 1;

	if(b <= m) 
	{
		return ask(l, head, m, a, b);
	} else if(m < a) {
		return ask(r, m + 1, tail, a, b);
	} else {
		return ask(l, head, m, a, m) + ask(r, m + 1, tail, m + 1, b);
	}
}

int link_calc(int u)
{
	int ans = height[u];
	modify_val = 1;

	do {
		ans -= ask(1, 1, n, enter[top[u]], enter[u]);
		modify(1, 1, n, enter[top[u]], enter[u]);
		u = fa[top[u]];
	} while(u != -1);
	return ans;
}

int main()
{
	std::scanf("%d", &n);
	for(int i = 1; i != n; ++i)
	{
		std::scanf("%d", fa + i);
		add_edge(fa[i], i);
	}

	fa[0] = -1;
	height[0] = 1;
	dfs_size(0);
	dfs_link(0, false);

	std::memset(lazy, -1, sizeof(lazy));

	int q;
	std::scanf("%d", &q);
	while(q --> 0)
	{
		char op[20];
		int u, ans = 0;
		std::scanf("%s %d", op, &u);
		if(*op == 'i')
		{
			ans = link_calc(u);
		} else {
			ans = ask(1, 1, n, enter[u], leave[u]);
			modify_val = 0;
			modify(1, 1, n, enter[u], leave[u]);
		}

		std::printf("%d\n", ans);
	}

	return 0;
}

