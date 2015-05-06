/* BZOJ-1620: [Usaco2008 Nov]Time Management 时间管理
 *   排序+贪心 */
#include <cstdio>
#include <algorithm>

const int MaxN = 1000;
struct work_t
{
	int during, end;
	bool operator < (const work_t& r) const
	{
		return end > r.end;
	}
} works[MaxN];

int main()
{
	int n;
	std::scanf("%d", &n);
	for(int i = 0; i != n; ++i)
		std::scanf("%d %d", &works[i].during, &works[i].end);
	std::sort(works, works + n);
	int now = ~0u >> 1;
	for(int i = 0; i != n; ++i)
	{
		if(works[i].end <= now)
			now = works[i].end - works[i].during;
		else now -= works[i].during;
	}

	if(now <= 0) std::puts("-1");
	else std::printf("%d", now);
	return 0;
}
