/* BZOJ-1233: [Usaco2009Open]干草堆tower 
 *  DP+单调队列 */
#include <cstdio>

const int MaxN = 100001;
int sum[MaxN];
int f[MaxN];
int g[MaxN], que[MaxN];
int main()
{
	int n;
	std::scanf("%d", &n);
	for(int i = 1; i <= n; ++i)
	{
		std::scanf("%d", sum + i);
		sum[i] += sum[i - 1];
	}

	int h = 0, t = 0;
	que[t] = n + 1;
	for(int i = n; i; --i)
	{
		while(h != t && sum[que[h + 1] - 1] - sum[i - 1] >= f[que[h + 1]])
			++h;
		f[i] = sum[que[h] - 1] - sum[i - 1];
		g[i] = g[que[h]] + 1;
		while(h != t && sum[i - 1] - f[i] >= sum[que[t] - 1] - f[que[t]])
			--t;
		que[++t] = i;
	}

	std::printf("%d", g[1]);
	return 0;
}

