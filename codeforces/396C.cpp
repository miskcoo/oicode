/* Codeforces 396C. On Changing Tree
 *   线段树
 *   data structures, trees */
#include <cstdio>

typedef long long calc_t;
const int mod_v = 1000000007;
const int MaxN = 300001;

int total, modify_add_v, modify_inc_v, ans;
int add_v[MaxN << 2], inc_v[MaxN << 2];
int head[MaxN], point[MaxN], next[MaxN];
int dfn_index, enter[MaxN], leave[MaxN], map[MaxN], height[MaxN];

void add_edge(int u, int v)
{
	point[++total] = v;
	next[total] = head[u];
	head[u] = total;
}

void dfs(int u)
{
	enter[u] = ++dfn_index;
	map[dfn_index] = u;
	for(int k = head[u]; k; k = next[k])
	{
		height[point[k]] = height[u] + 1;
		dfs(point[k]);
	}

	leave[u] = dfn_index;
}

void modify(int now, int head, int tail, int a, int b)
{
	if(head == a && tail == b)
	{
		add_v[now] = (add_v[now] + modify_add_v) % mod_v;
		inc_v[now] = (inc_v[now] + modify_inc_v) % mod_v;
		return;
	}

	int m = (head + tail) >> 1;
	int l = now << 1, r = l + 1;
	if(b <= m) return modify(l, head, m, a, b);
	if(m < a) return modify(r, m + 1, tail, a, b);
	modify(l, head, m, a, m);
	modify(r, m + 1, tail, m + 1, b);
}

void ask(int now, int head, int tail, int pos)
{
	ans = (ans + (calc_t)height[map[pos]] * inc_v[now] + add_v[now]) % mod_v;
	if(head == tail)
		return;

	int m = (head + tail) >> 1;
	if(pos <= m) ask(now << 1, head, m, pos);
	else ask((now << 1) + 1, m + 1, tail, pos);
}

int main()
{
	int n, q;
	std::scanf("%d", &n);
	for(int i = 2; i <= n; ++i)
	{
		int fa;
		std::scanf("%d", &fa);
		add_edge(fa, i);
	}

	dfs(1);

	std::scanf("%d", &q);
	while(q --> 0)
	{
		int op, v, x, k;
		std::scanf("%d %d", &op, &v);
		if(op == 1)
		{
			std::scanf("%d %d", &x, &k);
			modify_inc_v = -k;
			modify_add_v = (x + (calc_t)height[v] * k) % mod_v;
			modify(1, 1, n, enter[v], leave[v]);
		} else {
			ans = 0;
			ask(1, 1, n, enter[v]);
			if(ans < 0) ans += mod_v;
			std::printf("%d\n", ans);
		}
	}
	return 0;
}
