/* BZOJ-2153: 设计铁路
 *  斜率优化 */
#include <algorithm>
#include <cstdio>

struct block_t
{
	int dist;
	long long num;
	friend bool operator < (const block_t& a, const block_t& b)
	{
		return a.dist < b.dist;
	}
} block[40001];

long long f[40001];
long long remain[40001];
long long s[40001], t[40001];
long long g[40001];
int que[40001];

double calc_slope(int j, int i)
{
	return double(g[i] - g[j]) / (block[i].dist - block[j].dist);
}

int main()
{
	int n, m;
	std::scanf("%d %d", &n, &m);
	for(int i = 1; i <= n; ++i)
		std::scanf("%d %lld", &block[i].dist, &block[i].num);
	std::sort(block + 1, block + n + 1);

	int l = 1, r = 2;
	while(r <= n)
	{
		if(block[l].dist == block[r].dist)
		{
			block[l].num += block[r++].num;
		} else {
			block[++l] = block[r++];
		}
	}

	n = l;

	int qhead = 0, qtail = 0;
	que[qtail] = 0;

	for(int i = 2; i <= n; ++i)
	{
		s[i] = s[i - 1] + block[i - 1].num * block[i - 1].dist;
		t[i] = t[i - 1] + block[i - 1].num;
	}

	for(int i = 1; i <= n; ++i)
	{
		while(qhead < qtail && calc_slope(que[qhead], que[qhead + 1]) < t[i])
			++qhead;
		int d = que[qhead];
		f[i] = f[d] + (s[i] - s[d]) - block[d].dist * (t[i] - t[d]) + m;
		g[i] = f[i] - s[i] + block[i].dist * t[i];

		while(qhead < qtail && calc_slope(que[qtail - 1], que[qtail]) > calc_slope(que[qtail], i))
			--qtail;
		que[++qtail] = i;
	}

	remain[n] = 0;
	long long base = block[n].num;
	long long count = 0;
	for(int i = n - 1; i >= 0; --i)
	{
		count += base * (block[i + 1].dist - block[i].dist);
		base += block[i].num;
		remain[i] = count;
	}

	long long ans = ~0llu >> 1;
	for(int i = 0; i <= n; ++i)
		ans = std::min(ans, f[i] + remain[i]);
	std::printf("%lld", ans);
	return 0;
}
