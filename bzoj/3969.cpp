/* BZOJ-3969: [WF2013]Low Power
 *  二分+贪心 */
#include <cstdio>
#include <algorithm>

const int MaxN = 1000000;
int n, k, tot;
int val[MaxN];

bool check(int v)
{
	int count = 0;
	for(int i = 1; i < tot && count != n; ++i)
	{
		if(val[i] - val[i - 1] <= v)
		{
			if(tot - i + 1 >= 2 * k * (n - count))
				++i, ++count;
			else return false;
		}
	}

	return count == n;
}

int main()
{
	std::scanf("%d %d", &n, &k);
	tot = 2 * n * k;
	for(int i = 0; i != tot; ++i)
		std::scanf("%d", val + i);
	std::sort(val, val + tot);
	int l = 0, r = 1000000000;
	while(l < r)
	{
		int m = (l + r) >> 1;
		if(check(m)) r = m;
		else l = m + 1;
	}

	std::printf("%d", l);
	return 0;
}
