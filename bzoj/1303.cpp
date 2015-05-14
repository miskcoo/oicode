/* BZOJ-1303: [CQOI2009]中位数图 */
#include <cstdio>

const int MaxN = 100010;
int val[MaxN], cnt[MaxN << 1];

int main()
{
	int n, b, p;
	std::scanf("%d %d", &n, &b);
	for(int i = 1; i <= n; ++i)
	{
		std::scanf("%d", val + i);
		if(val[i] == b) p = i;
		else if(val[i] > b) val[i] = 1;
		else val[i] = -1;
	}

	int now = 0;
	for(int i = p + 1; i <= n; ++i)
	{
		now += val[i];
		++cnt[now + n];
	}

	now = 0; ++cnt[n];
	long long ans = 0;
	for(int i = p - 1; i; --i)
	{
		now += val[i];
		ans += cnt[n - now];
	}

	std::printf("%lld", ans + cnt[n]);
	return 0;
}
