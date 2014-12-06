#include <cstdio>
#include <cstring>
#include <algorithm>

const int MaxN = 1010;
double matrix[MaxN][MaxN];
double ans[MaxN], value[MaxN];

void gauss(int N)
{
	for(int i = 0; i != N; ++i)
	{
		int j = i + 1;
		double d = matrix[j][i] / matrix[i][i];
		matrix[j][N] = matrix[i][N] * d - matrix[j][N];
		matrix[j][N - 1] = matrix[i][N - 1] * d - matrix[j][N - 1];
		if(N - 1 != i + 1)
			matrix[j][i + 1] = matrix[i][i + 1] * d - matrix[j][i + 1];
		matrix[j][i] = 0.0;
	}

	for(int i = N - 1; i >= 0; --i)
	{
		double count = matrix[i][N];
		if(i != N - 1)
			count -= matrix[i][N - 1] * ans[N - 1];
		ans[i] = count / matrix[i][i];
		matrix[i][N] = matrix[i][N - 1] = matrix[i][i] = 0.0;
		matrix[i][i] = matrix[i][N] = matrix[i][N - 1] = 0;
	}
}

int main()
{
	std::freopen("a.in", "r", stdin);
	std::freopen("a.out", "w", stdout);
	int n, m;
	std::scanf("%d %d", &n, &m);

	value[0] = 4.0 / 9.0;
	value[1] = 5.0 / 9.0;

	for(int i = 3; i <= n; ++i)
	{
		for(int p = 0; p != i; ++p)
		{
			matrix[p][p] = 1.0;
			matrix[p][p ? p - 1 : i - 1] = -0.5;
			matrix[p][i] = p ? value[p - 1] / 3.0 : 1.0 / 6.0;
		}

		gauss(i);
		std::memcpy(value, ans, sizeof(double) * i);
	}

	std::printf("%.9lf", (double)value[m - 1]);
	return 0;
}
