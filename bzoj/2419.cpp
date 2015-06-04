/* BZOJ-2419: 电阻
 *   高斯消元+Kirchhoff电流定律 */
#include <cstdio>
#include <cstring>
#include <algorithm>

const double eps = 1.0e-8;
const int MaxN = 110;
bool cmp(double x) { return x < eps && x > -eps; }

double ans[MaxN], A[MaxN][MaxN];

void gauss(int n)
{
	for(int i = 1; i <= n; ++i)
	{
		for(int j = i; j <= n; ++j)
		{
			if(!cmp(A[j][i]))
			{
				if(i != j)
				{
					for(int k = 1; k <= n + 1; ++k)
						std::swap(A[i][k], A[j][k]);
				}

				break;
			}
		}

		for(int j = i + 1; j <= n; ++j)
		{
			double z = A[j][i] / A[i][i];
			for(int k = i; k <= n + 1; ++k)
				A[j][k] -= A[i][k] * z;
		}
	}

	for(int i = n; i; --i)
	{
		ans[i] = A[i][n + 1];
		for(int j = i + 1; j <= n; ++j)
			ans[i] -= A[i][j] * ans[j];
		ans[i] /= A[i][i];
	}
}

int main()
{
	int n, m;
	while(std::scanf("%d %d", &n, &m) != EOF)
	{
		std::memset(A, 0, sizeof(A));
		for(int i = 0; i != m; ++i)
		{
			int u, v;
			double r;
			std::scanf("%d %d %lf", &u, &v, &r);
			r = 1.0 / r;
			A[u][v] -= r; A[u][u] += r;
			A[v][u] -= r; A[v][v] += r;
		}

		A[1][n + 1] = 1;
		for(int i = 0; i <= n; ++i)
			A[n][i] = 0;
		A[n][n] = 1;

		gauss(n);
		std::printf("%.2lf\n", ans[1]);
	}
	return 0;
}
