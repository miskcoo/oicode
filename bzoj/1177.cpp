/* BZOJ-1177: [Apio2009]Oil
 *  DP */
#include <cstdio>

const int MaxN = 1501;
int sum[MaxN][MaxN];
int A[MaxN][MaxN], B[MaxN][MaxN], C[MaxN][MaxN], D[MaxN][MaxN];

int get_sum(int x1, int y1, int x2, int y2)
{
	return sum[x2][y2] + sum[x1 - 1][y1 - 1]
		- sum[x1 - 1][y2] - sum[x2][y1 - 1];
}

int max(int a, int b)
{
	return a < b ? b : a;
}

int max(int a, int b, int c)
{
	return max(a, max(b, c));
}

void solve_partial(int n, int m, int k)
{
	for(int i = k; i <= n; ++i)
	{
		for(int j = k; j <= m; ++j)
		{
			A[i][j] = max(A[i - 1][j], A[i][j - 1], 
					get_sum(i - k + 1, j - k + 1, i, j));
		}
	}

	for(int i = k; i <= n; ++i)
	{
		for(int j = m - k + 1; j; --j)
		{
			B[i][j] = max(B[i - 1][j], B[i][j + 1],
					get_sum(i - k + 1, j, i, j + k - 1));
		}
	}

	for(int i = n - k + 1; i; --i)
	{
		for(int j = k; j <= m; ++j)
		{
			C[i][j] = max(C[i + 1][j], C[i][j - 1],
					get_sum(i, j - k + 1, i + k - 1, j));
		}
	}

	for(int i = n - k + 1; i; --i)
	{
		for(int j = m - k + 1; j; --j)
		{
			D[i][j] = max(D[i + 1][j], D[i][j + 1],
					get_sum(i, j, i + k - 1, j + k - 1));
		}
	}
}

void solve_answer(int n, int m, int k)
{
	int ans = 0;
	for(int i = 1; i <= n; ++i)
	{
		for(int j = 1; j <= m; ++j)
		{
			ans = max(ans, A[i - 1][j - 1] + C[i][j - 1] + D[1][j]);
			ans = max(ans, C[1][j - 1] + B[i - 1][j] + D[i][j]);
			ans = max(ans, A[i - 1][j - 1] + B[i - 1][j] + D[i][1]);
			ans = max(ans, B[i - 1][1] + C[i][j - 1] + D[i][j]);
		}
	}

	for(int i = k; i + k <= n; ++i)
	{
		for(int j = 1; j + k - 1 <= m; ++j)
		{
			ans = max(ans, B[i][1] + D[i + k + 1][1]
					+ get_sum(i + 1, j, i + k, j + k - 1));
		}
	}

	for(int i = 1; i + k - 1 <= n; ++i)
	{
		for(int j = k; j + k <= m; ++j)
		{
			ans = max(ans, C[1][j] + D[1][j + k + 1]
					+ get_sum(i, j + 1, i + k - 1, j + k));
		}
	}

	std::printf("%d", ans);
}

int main()
{
	int n, m, k;
	std::scanf("%d %d %d", &n, &m, &k);
	for(int i = 1; i <= n; ++i)
	{
		for(int j = 1; j <= m; ++j)
		{
			std::scanf("%d", &sum[i][j]);
			sum[i][j] += sum[i - 1][j] + sum[i][j - 1] - sum[i - 1][j - 1];
		}
	}

	solve_partial(n, m, k);
	solve_answer(n, m, k);
	return 0;
}
