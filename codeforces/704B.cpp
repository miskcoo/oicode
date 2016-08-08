/* Codeforces 704B. Ant Man
 *   dp, graphs */
#include <cstdio>
#include <set>
#include <algorithm>
#include <cstring>

const int MaxN = 5010;
const long long inf = ~0ull >> 3;
int n, s, e;
int input[5][MaxN];
int *A, *B, *C, *D, *X;
long long dp[2][MaxN];

long long calc()
{
	using std::min;

	auto f = dp[0], g = dp[1];

	for(int i = 0; i <= n + 1; ++i)
		g[i] = inf;
	g[0] = 0;

	int mask = 0;
	for(int i = 1; i <= n; ++i)
	{
		for(int j = 0; j <= i; ++j)
		{
			f[j] = inf;
			if(i == s)
			{
				mask |= 1;
				f[j] = min(g[j + 1] + C[i], g[j] + D[i]);
			} else if(i == e) {
				mask |= 2;
				f[j] = min(g[j + 1] + A[i], g[j] + B[i]);
			} else {
				if(j || (mask & 1)) f[j] = min(f[j], g[j] + A[i] + D[i]);
				if(j || (mask & 2)) f[j] = min(f[j], g[j] + B[i] + C[i]);
				if(j) f[j] = min(f[j], g[j - 1] + D[i] + B[i]);
				if(j || mask) f[j] = min(f[j], g[j + 1] + C[i] + A[i]);
			}
		}

		for(int j = i + 1; j <= n + 1; ++j)
			f[j] = inf;

		std::swap(f, g);
	}

	if(n == s)
	{
		return f[0] + C[n];
	} else if(n == e) {
		return f[0] + A[n];
	} else {
		return f[0] + A[n] + C[n];
	}
}

int main()
{
	std::scanf("%d %d %d", &n, &s, &e);

	for(int i = 0; i != 5; ++i)
		for(int j = 1; j <= n; ++j)
			std::scanf("%d", input[i] + j);

	X = input[0];
	A = input[1]; B = input[2];
	C = input[3]; D = input[4];

	for(int i = 1; i <= n; ++i)
	{
		A[i] += X[i], B[i] -= X[i];
		C[i] += X[i], D[i] -= X[i];
	}

	std::printf("%I64d", calc());

	return 0;
}
