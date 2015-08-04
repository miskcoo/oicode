/* BZOJ-4198: [Noi2015]荷马史诗
 *   堆 */
#include <cstdio>
#include <functional>
#include <vector>
#include <algorithm>
#include <queue>

const int MaxN = 200100;
long long val[MaxN];

typedef std::pair<long long, int> node_t;
std::priority_queue<node_t, std::vector<node_t>, std::greater<node_t> > heap;

int main()
{
	int n, k;
	std::scanf("%d %d", &n, &k);
	for(int i = 0; i != n; ++i)
		std::scanf("%lld", val + i);
	while((n - 1) % (k - 1)) ++n;

	for(int i = 0; i != n; ++i)
		heap.push(node_t(val[i], 0));

	long long ans = 0, height = 0;
	for(int i = 1; i < n; i += k - 1)
	{
		node_t x;
		for(int j = 0; j != k; ++j)
		{
			node_t a = heap.top(); heap.pop();
			x.first += a.first;
			x.second = std::max(x.second, a.second + 1);
		}

		heap.push(x);
		ans += x.first;
		height = x.second;
	}

	std::printf("%lld\n%lld\n", ans, height);
	return 0;
}
