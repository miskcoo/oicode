/* BZOJ-1642: [Usaco2007 Nov]Milking Time 挤奶时间
 *   DP+树状数组 */
#include <cstdio>
#include <algorithm>

const int MaxN = 1010, MaxT = 1000010;
struct range_t
{
	int start, end, product;
	bool operator < (const range_t& r) const
	{
		return start < r.start;
	}
} range[MaxN];
int n, m, r, ta[MaxT];

void modify(int x, int v)
{
	for(; x <= n; x += x & -x)
		ta[x] = std::max(ta[x], v);
}

int ask(int x)
{
	int ans = 0;
	for(; x; x -= x & -x)
		ans = std::max(ans, ta[x]);
	return ans;
}

int main()
{
	std::scanf("%d %d %d", &n, &m, &r);
	for(int i = 0; i != m; ++i)
		std::scanf("%d %d %d", &range[i].start, &range[i].end, &range[i].product);
	std::sort(range, range + m);
	int ans = 0, now_ans = 0;
	for(int i = 0; i != m; ++i)
	{
		int t = range[i].start - r;
		now_ans = range[i].product;
		if(t > 0) now_ans = std::max(now_ans, ask(t) + range[i].product);
		modify(range[i].end, now_ans);
		ans = std::max(ans, now_ans);
	}

	std::printf("%d", ans);
	return 0;
}
