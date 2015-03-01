/* BZOJ-1042: [HAOI2008]硬币购物
 *  DP+容斥原理 */
#include <cstdio>

const int MaxN = 100001;
long long ans, f[MaxN];
int c[4], d[4];

void dfs(int k, int n, int sum)
{
	if(sum < 0) return;
	if(k == 4)
	{
		if(n & 1) ans -= f[sum];
		else ans += f[sum];
		return;
	}

	dfs(k + 1, n, sum);
	dfs(k + 1, n + 1, sum - (d[k] + 1) * c[k]);
}

int main()
{
	int tot;
	for(int i = 0; i != 4; ++i)
		std::scanf("%d", c + i);
	std::scanf("%d", &tot);
	f[0] = 1;
	for(int j = 0; j != 4; ++j)
	{
		for(int i = c[j]; i != MaxN; ++i)
			f[i] += f[i - c[j]];
	}

	while(tot --> 0)
	{
		int s;
		for(int i = 0; i != 4; ++i)
			std::scanf("%d", d + i);
		std::scanf("%d", &s);
		ans = 0;
		dfs(0, 0, s);
		std::printf("%lld\n", ans);
	}
	return 0;
}
