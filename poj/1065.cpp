/* POJ-1065 Wooden Sticks
 *   Dilworth 定理 */
#include <cstdio>
#include <algorithm>

const int MaxN = 5000;
struct stick_t
{
	int length, weight;
	bool operator < (const stick_t& r) const
	{
		if(length == r.length)
			return weight < r.weight;
		return length < r.length;
	}
} sticks[MaxN];
int A[MaxN];

void solve()
{
	int n;
	std::scanf("%d", &n);
	for(int i = 0; i != n; ++i)
		std::scanf("%d %d", &sticks[i].length, &sticks[i].weight);
	std::sort(sticks, sticks + n);
	int ans = 0;
	for(int i = 0; i != n; ++i)
	{
		bool flag = false;
		for(int j = 0; j != ans; ++j)
		{
			if(sticks[i].weight >= A[j])
			{
				flag = true;
				A[j] = sticks[i].weight;
				break;
			}
		}

		if(!flag) A[ans++] = sticks[i].weight;
	}

	std::printf("%d\n", ans);
}

int main()
{
	int cases;
	std::scanf("%d", &cases);
	while(cases --> 0) solve();
	return 0;
}
