/*
 * BZOJ-1257 [CQOI2007]余数之和sum
 *  数论
 */
#include <cstdio>

int main()
{
	int N, K;
	std::scanf("%d %d", &N, &K);
	long long sum = (long long)N * K;
	if(N > K) N = K;
	int l = 1;

	while(l <= N)
	{
		int v = K / l;
		int r = K / v;
		if(r > N) r = N;
		sum -= v * (r - l + 1) * (long long)(r + l) / 2;
		l = r + 1;
	}

	std::printf("%lld\n", sum);
	return 0;
}
