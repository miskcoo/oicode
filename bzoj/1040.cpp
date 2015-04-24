/* BZOJ-1040: [ZJOI2008]骑士
 *   环套树DP */
#include <cstdio>
#include <algorithm>

const int MaxN = 1000010;
int total;
int fa[MaxN], deg[MaxN], weight[MaxN], que[MaxN];
long long f[MaxN][2], dp[MaxN][2];

void topology_sort(int n)
{
	int qhead = 0, qtail = 0;
	for(int i = 1; i <= n; ++i)
	{
		if(deg[i] == 0)
			que[qtail++] = i;
	}

	while(qhead != qtail)
	{
		int u = que[qhead++], p = fa[u];
		f[p][1] += f[u][0];
		f[p][0] += std::max(f[u][0], f[u][1]);
		if(--deg[p] == 0)
			que[qtail++] = p;
	}
}

long long solve_circle(int u)
{
	int *circ = que, tot = 0;
	do {
		circ[tot++] = u;
		--deg[u];
		u = fa[u];
	} while(u != circ[0]);
	circ[tot] = circ[0];

	dp[0][1] = 0;
	dp[0][0] = f[u][0];
	for(int i = 0; i != tot; ++i)
	{
		int v = circ[i + 1];
		dp[i + 1][0] = f[v][0] + std::max(dp[i][0], dp[i][1]);
		dp[i + 1][1] = f[v][1] + dp[i][0];
	}

	long long ans = std::max(dp[tot - 1][0], dp[tot - 1][1]);
	dp[0][0] = f[u][0];
	dp[0][1] = f[u][1];
	for(int i = 0; i != tot; ++i)
	{
		int v = circ[i + 1];
		dp[i + 1][0] = f[v][0] + std::max(dp[i][0], dp[i][1]);
		dp[i + 1][1] = f[v][1] + dp[i][0];
	}

	return std::max(ans, dp[tot - 1][0]);
}

int main()
{
	int n;
	std::scanf("%d", &n);
	for(int i = 1; i <= n; ++i)
	{
		int u;
		std::scanf("%d %d", weight + i, &u);
		f[i][0] = 0;
		f[i][1] = weight[i];
		++deg[u], fa[i] = u;
	}

	long long ans = 0;
	topology_sort(n);
	for(int i = 1; i <= n; ++i)
		if(deg[i]) ans += solve_circle(i);
	std::printf("%lld", ans);
	return 0;
}
