/* BZOJ-3550: [ONTAK2010]Vacation
 *   线性规划（单纯形法）*/
#include <cstdio>
#include <algorithm>

const int MaxN = 1210, MaxM = 610;
const double eps = 1.0e-6, inf = 1.0e20;

bool cmp(double x) { return x > -eps && x < eps; }

int n, m, k;
double ans, A[MaxN][MaxM], C[MaxN], F[MaxM];

void pivot(int l, int e)
{
	C[l] /= A[l][e];
	for(int i = 0; i != n; ++i)
		if(i != e) A[l][i] /= A[l][e];
	A[l][e] = 1 / A[l][e];

	for(int i = 0; i != m; ++i)
	{
		if(i != l && !cmp(A[i][e]))
		{
			C[i] -= A[i][e] * C[l];
			for(int j = 0; j != n; ++j)
				if(j != e) A[i][j] -= A[i][e] * A[l][j];
			A[i][e] *= -A[l][e];
		}
	}

	ans += F[e] * C[l];
	for(int i = 0; i != n; ++i)
		if(i != e) F[i] -= F[e] * A[l][i];
	F[e] *= -A[l][e];
}

void simplex()
{
	for(;;)
	{
		int i, j, k;
		double lim;
		for(lim = 0, i = 0; i != n; ++i)
			if(F[i] > lim) lim = F[i], k = i;
		if(lim < eps) return;
		lim = inf;
		for(i = 0; i != m; ++i)
		{
			if(A[i][k] > eps && lim > C[i] / A[i][k])
				j = i, lim = C[i] / A[i][k];
		}

		if(lim > 0.5 * inf) break;
		pivot(j, k);
	}
} 

int main()
{
	std::scanf("%d %d", &n, &k);
	n *= 3;
	for(int i = 0; i != n; ++i)
		std::scanf("%lf", F + i);

	for(int i = 0; i + n / 3 <= n; ++i, ++m)
	{
		C[m] = k;
		for(int j = 0; j != n / 3; ++j)
			A[m][i + j] = 1;
	} 

	for(int i = 0; i != n; ++i, ++m)
		C[m] = 1, A[m][i] = 1;
	simplex(); 
	std::printf("%.0lf", ans);
	return 0;
}
