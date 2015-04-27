/* BZOJ-4011: [HNOI2015]落忆枫音
 *   拓扑序DP  */
#include <cstdio>

const int MaxN = 100010, MaxM = 200010;
const long long mod_v = 1000000007;
int total;
int head[MaxN], point[MaxM], next[MaxM];
int que[MaxN], deg[MaxN], deg2[MaxN];
long long inv[MaxM], f[MaxN];

void add_edge(int u, int v)
{
	++deg[v], ++deg2[v];
	point[++total] = v;
	next[total] = head[u];
	head[u] = total;
}

void init_inv(int m)
{
	inv[1] = 1;
	for(int i = 2; i <= m; ++i)
		inv[i] = -inv[mod_v % i] * (mod_v / i) % mod_v;
}

void topology_sort(int n)
{
	int qhead = 0, qtail = 0;
	for(int i = 1; i <= n; ++i)
		if(!deg[i]) que[qtail++] = i;
	while(qhead != qtail)
	{
		int u = que[qhead++];
		f[u] = f[u] * inv[deg2[u]] % mod_v;
		for(int k = head[u]; k; k = next[k])
		{
			int v = point[k];
			f[v] = (f[v] + f[u]) % mod_v;
			if(--deg[v] == 0)
				que[qtail++] = v;
		}
	}
}

int main()
{
	int n, m, x, y;
	std::scanf("%d %d %d %d", &n, &m, &x, &y);
	for(int i = 0; i != m; ++i)
	{
		int u, v;
		std::scanf("%d %d", &u, &v);
		add_edge(u, v);
	}

	init_inv(m);
	long long ans = 1;
	++deg2[y];
	for(int i = 2; i <= n; ++i)
		ans = ans * deg2[i] % mod_v;
	if(y != 1)
	{
		f[y] = ans;
		topology_sort(n);
		ans = ((ans - f[x]) % mod_v + mod_v) % mod_v;
	}
	std::printf("%lld", ans);
	return 0;
}
