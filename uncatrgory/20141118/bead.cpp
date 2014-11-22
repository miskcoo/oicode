#include <cstdio>

const long long mod_v = 1000000007;
long long map[20][20];
long long dist[1 << 16];
long long f[1 << 16];

int main()
{
	std::freopen("bead.in", "r", stdin);
	std::freopen("bead.out", "w", stdout);
	int n;
	std::scanf("%d", &n);
	for(int i = 0; i != n; ++i)
	{
		for(int j = 0; j != n; ++j)
			std::scanf("%lld", map[i] + j);
	}

	for(int s = 0; s != 1 << n; ++s)
	{
		int temp[20];
		int num = 0;
		for(int i = 0; s >> i; ++i)
		{
			if(s & (1 << i))
				temp[num++] = i;
		}

		dist[s] = 1;
		for(int i = 0; i != num; ++i)
		{
			for(int j = i + 1; j != num; ++j)
				dist[s] = (dist[s] * (map[temp[i]][temp[j]] + 1)) % mod_v;
		}
	}

	f[0] = 1;
	for(int s = 1; s != 1 << n; ++s)
	{
		f[s] = dist[s];
		int top = 1 << n;
		while(!(top & s)) top >>= 1;
		for(int t = (s - 1) & s; t & top; t = (t - 1) & s)
			f[s] = (f[s] - f[t] * dist[s - t]) % mod_v;
	}

	long long ans = f[(1 << n) - 1];
	std::printf("%lld", (ans + mod_v) % mod_v);
	return 0;
}
