/* Codeforces 243D. Cubes
 *   线段树
 *   data structures, dp, geometry, two pointers */
#include <cstdio>
#include <cmath>
#include <cstring>
#include <algorithm>

const int MaxN = 1001;
int n, vx, vy;
int val[4 * MaxN * MaxN], lazy[4 * MaxN * MaxN];
long long hash[MaxN * MaxN];
int map[MaxN][MaxN];

int ask_min(int now, int head, int tail, int a, int b)
{
	if(head == a && tail == b)
		return val[now];
	int m = (head + tail) >> 1;
	int l = now << 1, r = l + 1;
	if(b <= m) return std::max(lazy[now], ask_min(l, head, m, a, b));
	if(m < a) return std::max(lazy[now], ask_min(r, m + 1, tail, a, b));
	return std::max(lazy[now], std::min(ask_min(l, head, m, a, m), ask_min(r, m + 1, tail, m + 1, b)));
}

void modify(int now, int head, int tail, int a, int b, int v)
{
	if(head == a && tail == b)
	{
		lazy[now] = std::max(v, lazy[now]);
		val[now] = std::max(lazy[now], val[now]);
		return;
	}

	int m = (head + tail) >> 1;
	int l = now << 1, r = l + 1;
	if(b <= m) modify(l, head, m, a, b, v);
	else if(m < a) modify(r, m + 1, tail, a, b, v);
	else modify(l, head, m, a, m, v), modify(r, m + 1, tail, m + 1, b, v);
	val[now] = std::max(lazy[now], std::min(val[l], val[r]));
}

long long calc(long long x, long long y)
{
	return vx * y - vy * x;
}

int main()
{
	std::scanf("%d %d %d", &n, &vx, &vy);
	for(int i = 1; i <= n; ++i)
		for(int j = 1; j <= n; ++j)
			std::scanf("%d", map[i] + j);

	if(vx < 0)
	{
		vx = -vx;
		for(int i = 1; i < n - i + 1; ++i)
			for(int j = 1; j <= n; ++j)
				std::swap(map[i][j], map[n - i + 1][j]);
	}

	if(vy < 0)
	{
		vy = -vy;
		for(int i = 1; i <= n; ++i)
			for(int j = 1; j < n - j + 1; ++j)
				std::swap(map[i][j], map[i][n - j + 1]);
	}

	int tot = 0;
	for(int i = 0; i <= n; ++i)
		for(int j = 0; j <= n; ++j)
			hash[tot++] = calc(i, j);
	std::sort(hash, hash + tot);
	tot = std::unique(hash, hash + tot) - hash;

	long long ans = 0;
	for(int i = 1; i <= n; ++i)
	{
		for(int j = 1; j <= n; ++j)
		{
			int l = std::lower_bound(hash, hash + tot, calc(i, j - 1)) - hash + 1;
			int r = std::lower_bound(hash, hash + tot, calc(i - 1, j)) - hash;
			if(l > r) std::swap(l, r);
			int h = ask_min(1, 1, tot, l, r);
			ans += std::max(map[i][j] - h, 0);
			modify(1, 1, tot, l, r, map[i][j]);
		}
	}

	std::printf("%I64d\n", ans);

	return 0;
}
