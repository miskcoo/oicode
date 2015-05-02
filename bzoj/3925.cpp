/* BZOJ-3925: [Zjoi20150]地震后的幻想乡
 *   概率，子集DP */
#include <cstdio>
#include <algorithm>
#include <vector>

typedef __float128 value_t;
typedef std::vector<value_t> poly_t;

poly_t operator * (const poly_t& a, const poly_t& b)
{
	poly_t c(a.size() + b.size(), 0);
	for(size_t i = 0; i != a.size(); ++i)
		for(size_t j = 0; j != b.size(); ++j)
			c[i + j] += a[i] * b[j];
	return c;
}

poly_t operator - (const poly_t& a, const poly_t& b)
{
	poly_t c(std::max(a.size(), b.size()), 0);
	for(size_t i = 0; i != c.size(); ++i)
	{
		if(i < a.size()) c[i] += a[i];
		if(i < b.size()) c[i] -= b[i];
	}
	return c;
}

poly_t operator + (const poly_t& a, const poly_t& b)
{
	poly_t c(std::max(a.size(), b.size()), 0);
	for(size_t i = 0; i != c.size(); ++i)
	{
		if(i < a.size()) c[i] += a[i];
		if(i < b.size()) c[i] += b[i];
	}
	return c;
}

value_t integrate_eval(const poly_t& a)
{
	value_t ans = 0;
	for(size_t i = 0; i != a.size(); ++i)
		ans += a[i] / (i + 1);
	return ans;
}

const int MaxN = 10;
int edge[MaxN], bitcnt[1 << MaxN];
poly_t prob[1 << MaxN], pw[MaxN * MaxN];
poly_t one(1, 1), zero(1, 0);

int main()
{
	int n, m;
	std::scanf("%d %d", &n, &m);
	for(int i = 0; i != m; ++i)
	{
		int u, v;
		std::scanf("%d %d", &u, &v);
		--u, --v;
		edge[u] |= 1 << v;
		edge[v] |= 1 << u;
	}

	for(int i = 1; i != 1 << n; ++i)
		bitcnt[i] = bitcnt[i - (i & -i)] + 1;
	pw[0] = one, pw[1] = poly_t(2, 0);
	pw[1][0] = 1, pw[1][1] = -1;
	for(int i = 2; i < n * n; ++i)
		pw[i] = pw[i - 1] * pw[1];

	for(int s = 1; s != 1 << n; ++s)
	{
		if(bitcnt[s] == 1)
		{
			prob[s] = one;
		} else {
			int fixed;
			for(int i = 0; i != n; ++i)
				if((s >> i) & 1) fixed = i;
			poly_t ret = one;
			int remain = s - (1 << fixed);
			for(int ss = remain; ss >= 0; ss = (ss - 1) & remain)
			{
				int s0 = ss | (1 << fixed);
				if(s0 < s)
				{
					int between = 0;
					int s1 = s - s0;
					for(int i = 0; i != n; ++i)
					{
						if((s0 >> i) & 1)
							between += bitcnt[edge[i] & s1];
					}

					ret = ret - prob[s0] * pw[between];
				}

				if(ss == 0) break;
			}

			prob[s] = ret;
		}
	}

	value_t ans = integrate_eval(one - prob[(1 << n) - 1]);
	std::printf("%.6lf", (double)ans);
	return 0;
}
