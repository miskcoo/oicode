/* BZOJ-3744: Gty的妹子序列
 *  分块+树状数组 */
#include <cstdio>
#include <cstring>
#include <cmath>
#include <algorithm>

const int MaxN = 50010, MaxB = 300;
typedef long long int64;
int block_size;
int64 f[MaxB][MaxB], g[MaxB][MaxN];
int64 data[MaxN];
struct data_t
{
	int pos;
	int64 v;
	bool operator < (const data_t& r) const
	{
		return v < r.v;
	}
} tmp[MaxN];

struct tree_array
{
	int size, ta[MaxN];

	void reset()
	{
		std::memset(ta, 0, (size + 1) * sizeof(int));
	}

	void modify(int x, int v)
	{
		for(; x; x -= x & -x)
			ta[x] += v;
	}

	int ask(int x)
	{
		int v = 0;
		for(; x <= size; x += x & -x)
			v += ta[x];
		return v;
	}
} ta;

int64 get_sum(int l, int r, int v)
{
	if(!l) return g[r][v];
	return g[r][v] - g[l - 1][v];
}

int64 partial_solve(int l, int r)
{
	int64 ans = 0;
	for(int i = l; i <= r; ++i)
	{
		if(data[i] < ta.size)
			ans += ta.ask(data[i] + 1);
		ta.modify(data[i], 1);
	}
	return ans;
}

int64 solve(int l, int r)
{
	ta.reset();
	int a = (l + block_size - 1) / block_size;
	int b = r / block_size;
	if(a >= b) return partial_solve(l, r);
	int64 ans = f[a][b - 1];
	ans += partial_solve(l, a * block_size - 1);
	ans += partial_solve(b * block_size, r);
	for(int i = l; i != a * block_size; ++i)
		ans += get_sum(a, b - 1, data[i] - 1);

	for(int i = b * block_size; i <= r; ++i)
	{
		ans += get_sum(a, b - 1, ta.size);
		ans -= get_sum(a, b - 1, data[i]);
	}

	return ans;
}

int main()
{
	int n;
	std::scanf("%d", &n);
	for(int i = 0; i != n; ++i)
	{
		tmp[i].pos = i;
		std::scanf("%lld", &tmp[i].v);
	}
	std::sort(tmp, tmp + n);
	int prev = -1, cnt = 0;
	for(int i = 0; i != n; ++i)
	{
		if(prev != tmp[i].v)
		{
			++cnt;
			prev = tmp[i].v;
		}
		data[tmp[i].pos] = cnt;
	}

	ta.size = cnt;
	block_size = (int)std::sqrt(double(n)) + 1;
	for(int i = 0, j = 0; j < n; ++i, j += block_size)
	{
		ta.reset();
		for(int k = j, w = i; k < n; ++w, k += block_size)
		{
			if(w) f[i][w] += f[i][w - 1];
			for(int p = k, t = 0; t < block_size && p < n; ++p, ++t)
			{
				if(data[p] != cnt)
					f[i][w] += ta.ask(data[p] + 1);
				ta.modify(data[p], 1);
			}
		}

		for(int k = j, w = 0; w < block_size && k < n; ++w, ++k)
			++g[i][data[k]];
		for(int k = 1; k <= cnt; ++k)
			g[i][k] += g[i][k - 1];
		for(int k = 1; k <= cnt; ++k)
			if(i) g[i][k] += g[i - 1][k];
	}

	int m;
	std::scanf("%d", &m);
	int64 lastans = 0;
	for(int i = 0; i != m; ++i)
	{
		int l, r;
		std::scanf("%d %d", &l, &r);
		l ^= lastans, r ^= lastans;
		lastans = solve(l - 1, r - 1);
		std::printf("%lld\n", lastans);
	}
	return 0;
}
