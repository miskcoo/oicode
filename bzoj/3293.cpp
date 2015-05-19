/* BZOJ-3293: [Cqoi2011]分金币 */
#include <cstdio>
#include <algorithm>

const int MaxN = 1000001;
long long A[MaxN], C[MaxN], S[MaxN];

long long abs(long long v) { return v < 0 ? -v : v; }

int main()
{
	int n;
	std::scanf("%d", &n);
	for(int i = 1; i <= n; ++i)
	{
		std::scanf("%lld", A + i);
		S[i] = S[i - 1] + A[i];
	}

	long long mean = S[n] / n;
	for(int i = 1; i <= n; ++i)
		C[i] = mean * i - S[i];

	int mid = (n + 1) >> 1;
	std::nth_element(C + 1, C + mid, C + n + 1);

	long long ans = 0;
	for(int i = 1; i <= n; ++i)
		ans += abs(C[i] - C[mid]);

	std::printf("%lld", ans);

	return 0;
}
