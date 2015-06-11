/* Codeforces 51E. Pentagon
 *   组合计数，DP，图论
 *   combinatorics, graphs, matrices */
#include <cstdio>
#include <iostream>

const int MaxN = 700;
int n, m;
int deg[MaxN], A[MaxN][MaxN], B[MaxN][MaxN], C[MaxN][MaxN];

void solve(int A[][MaxN], int B[][MaxN], int C[][MaxN])
{
	for(int i = 0; i != n; ++i)
	{
		for(int j = 0; j <= i; ++j)
		{
			for(int k = 0; k < n; ++k)
				C[i][j] += A[i][k] * B[k][j];
			C[j][i] = C[i][j];
		}
	}
}

int main()
{
	std::scanf("%d %d", &n, &m);
	for(int i = 0; i != m; ++i)
	{
		int u, v;
		std::scanf("%d %d", &u, &v);
		++deg[--v], ++deg[--u];
		A[u][v] = A[v][u] = 1;
	}

	solve(A, A, B);
	solve(A, B, C);

	long long ans = 0;
	for(int i = 0; i != n; ++i)
		for(int j = 0; j != n; ++j)
			ans += (long long)B[i][j] * C[i][j];
	ans /= 10;

	for(int i = 0; i != n; ++i)
	{
		for(int j = 0; j != i; ++j)
		{
			if(!A[i][j]) continue;
			for(int k = 0; k != j; ++k)
			{
				if(A[i][k] && A[j][k])
					ans -= deg[i] + deg[j] + deg[k] - 3;
			}
		}
	}

	std::cout << ans << std::endl;
	return 0;
}
