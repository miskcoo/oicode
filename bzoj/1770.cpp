/* BZOJ-1770: [Usaco2009 Nov]lights 燈
 *  高斯消元+搜索 */
#include <cstdio>
#include <algorithm>

const int MaxN = 37;
int N, M;
int light[MaxN][MaxN];
int eqn[MaxN];
int ans[MaxN];
int now_ans;

void dfs(int n, int answer)
{
	if(now_ans <= answer)
		return;
	if(n < 0)
	{
		now_ans = std::min(now_ans, answer);
		return;
	}

	if(light[eqn[n]][n] == 0)
	{
		ans[n] = 0;
		dfs(n - 1, answer);
		ans[n] = 1;
		dfs(n - 1, answer + 1);
	} else {
		int x = light[eqn[n]][N];
		for(int j = n + 1; j < N; ++j)
		{
			if(light[eqn[n]][j])
				x ^= ans[j];
		}
		ans[n] = x;
		dfs(n - 1, answer + (x == 1));
	}
}

void gauss_elimination()
{
	for(int i = 0; i != N; ++i)
		eqn[i] = i;
	for(int i = 0; i != N; ++i)
	{
		// find non-zero variable
		int var = -1;
		for(int j = i; j != N; ++j)
		{
			if(light[eqn[j]][i])
			{
				var = j;
				break;
			}
		}

		if(var == -1)
			continue;

		std::swap(eqn[i], eqn[var]);

		// eliminate
		for(int j = i + 1; j < N; ++j)
		{
			if(!light[eqn[j]][i])
				continue;
			for(int k = i; k <= N; ++k)
				light[eqn[j]][k] ^= light[eqn[i]][k];
		}
	}

	for(int i = N - 1; i >= 0; --i)
	{
		int x = light[eqn[i]][N];
		for(int j = i + 1; j < N; ++j)
		{
			if(light[eqn[i]][j])
				x ^= ans[j];
		}
		ans[i] = x;
		if(x) ++now_ans;
	}

	dfs(N - 1, 0);
}

int main()
{
	std::scanf("%d %d", &N, &M);
	for(int i = 0; i != M; ++i)
	{
		int u, v;
		std::scanf("%d %d", &u, &v);
		--u, --v;
		light[u][v] = light[v][u] = 1;
	}

	for(int i = 0; i != N; ++i)
		light[i][i] = light[i][N] = 1;

	gauss_elimination();
	std::printf("%d", now_ans);
	return 0;
}
