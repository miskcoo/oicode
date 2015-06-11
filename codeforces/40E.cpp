/* Codeforces 40E. Number Table
 *   组合计数
 *   combinatorics */
#include <cstdio>
#include <algorithm>

typedef long long calc_t;
const int MaxN = 1010;
int mod_v, pw[MaxN], cnt[MaxN], val[MaxN];

int main()
{
	int n, m, t, flag = 0;
	std::scanf("%d %d %d", &n, &m, &t);
	if(n < m) std::swap(n, m), flag = 1;
	for(int i = 0; i != t; ++i)
	{
		int x, y, v;
		std::scanf("%d %d %d", &x, &y, &v);
		if(flag) std::swap(x, y);
		++cnt[x];
		if(!val[x]) val[x] = v;
		else val[x] *= v;
	}

	std::scanf("%d", &mod_v);

	for(int i = 1; i <= n; ++i)
	{
		if(!cnt[i])
		{
			std::swap(cnt[i], cnt[n]);
			std::swap(val[i], val[n]);
			break;
		}
	}

	pw[0] = 1;
	for(int i = 1; i <= m; ++i)
		pw[i] = (pw[i - 1] << 1) % mod_v;
	int ans = 1;
	for(int i = 1; i != n; ++i)
	{
		if(cnt[i] == m && val[i] == 1) ans = 0;
		else if(m != cnt[i])
			ans = (calc_t)ans * pw[m - cnt[i] - 1] % mod_v;
	}

	if((n ^ m) & 1) std::puts("0");
	else std::printf("%d\n", ans);
	return 0;
}
