/* BZOJ-3812: 主旋律
 *   子集DP+容斥原理 */
#include <cstdio>

const int MaxN = 15;
const long long mod_v = 1000000007;
long long pw[MaxN * MaxN];
int bitcnt[1 << MaxN], edge_in[1 << MaxN], edge_out[1 << MaxN];
long long f[1 << MaxN], g[1 << MaxN], h[1 << MaxN], p[1 << MaxN];

int main()
{
	int n, m;
	std::scanf("%d %d", &n, &m);
	for(int i = 0; i != m; ++i)
	{
		int u, v;
		std::scanf("%d %d", &u, &v);
		u = 1 << (u - 1), v = 1 << (v - 1);
		edge_out[u] |= v;
		edge_in[v] |= u;
	}

	pw[0] = 1;
	for(int i = 1; i != n * n; ++i)
		pw[i] = (pw[i - 1] << 1) % mod_v;

	bitcnt[0] = 0;
	for(int i = 1; i != 1 << n; ++i)
		bitcnt[i] = bitcnt[i - (i & -i)] + 1;

	for(int s = 1; s != 1 << n; ++s)
	{
		int one = s & -s, outside = s ^ one;
		for(int i = outside; i; i = (i - 1) & outside)
			g[s] = (g[s] - f[s ^ i] * g[i]) % mod_v;

		h[s] = h[outside] 
			+ bitcnt[edge_in[one] & outside]
			+ bitcnt[edge_out[one] & outside];

		f[s] = pw[h[s]];
		for(int i = s; i; i = (i - 1) & s)
		{
			if(i != s)
			{
				int one = (i ^ s) & -(i ^ s);
				p[i] = p[i ^ one] 
					+ bitcnt[edge_out[one] & i]
					- bitcnt[edge_in[one] & (i ^ s)];
			} else p[i] = 0;
			f[s] = (f[s] - pw[h[s ^ i] + p[i]] * g[i]) % mod_v;
		}

		g[s] = (g[s] + f[s]) % mod_v;
	}

	long long ans = (f[(1 << n) - 1] + mod_v) % mod_v;
	std::printf("%lld", ans);

	return 0;
}
