#include <cstdio>
#include <algorithm>
#include <cstring>

const int max_int = 44444444;
char A[100011], B[100011];
int dp[2][100011];

int solve(int b, int a, int k)
{
	if(b == 0) return 0;
	int la = std::strlen(A);
	int lb = std::strlen(B);

	int* f = dp[0], *g = dp[1];
	std::memset(dp, 44, sizeof(dp));
	g[0] = 0;
	for(int i = 1; i <= lb; ++i)
		g[i] = g[i - 1] + b;

	for(int i = 1; i <= la; ++i)
	{
		f[0] = b * i;
		int right = std::min(lb, i + k / b);
		for(int j = std::max(1, i - k / b); j <= right; ++j)
		{
			int v1 = std::min(f[j - 1], g[j]) + b;
			int v2 = g[j - 1] + a;
			if(A[i - 1] == B[j - 1])
				v2 -= a;
			f[j] = std::min(v1, v2);
		}

		std::swap(f, g);
	}

	return g[lb] > k ? -1 : g[lb];
}

int main()
{
	int T;
	std::scanf("%d", &T);
	while(T --> 0)
	{
		int a, b, k;
		std::scanf("%s", A);
		std::scanf("%s", B);
		std::scanf("%d %d %d", &a, &b, &k);
		std::printf("%d\n", solve(a, b, k));
	}
	return 0;
}

