/* BZOJ-4080: [Wf2014]Sensor Network
 *   随机化 */
#include <cstdio>
#include <algorithm>
#include <cstdlib>
#include <bitset>

const int MaxN = 100;
typedef std::bitset<MaxN> bit_t;
int X[MaxN], Y[MaxN], order[MaxN];
bit_t edge[MaxN];

long long sqr(long long x) { return x * x; }
long long dist(int i, int j)
{
	return sqr(X[i] - X[j]) + sqr(Y[i] - Y[j]);
}

int main()
{
	int n, d;
	std::scanf("%d %d", &n, &d);
	for(int i = 0; i != n; ++i)
	{
		std::scanf("%d %d", X + i, Y + i);
		for(int j = 0; j != i; ++j)
		{
			if(dist(i, j) <= sqr(d))
				edge[i][j] = 1, edge[j][i] = 1;
		}
	}

	bit_t ans;
	for(int i = 0; i != n; ++i) order[i] = i;
	for(int i = 0; i != 1000; ++i)
	{
		std::random_shuffle(order, order + n);
		bit_t mark, now;
		mark = ~mark;
		for(int j = 0; j != n; ++j)
		{
			if(mark[order[j]])
			{
				now[order[j]] = 1;
				mark &= edge[order[j]];
			}
		}

		if(now.count() > ans.count()) ans = now;
	}

	std::printf("%d\n", ans.count());
	for(int i = 0; i != n; ++i)
		if(ans[i]) std::printf("%d ", i + 1);
	return 0;
}
