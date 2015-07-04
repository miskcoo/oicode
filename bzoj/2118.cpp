/* BZOJ-2118: 墨墨的等式
 *   dijkstra */
#include <cstdio>
#include <queue>
#include <algorithm>
#include <functional>

const int MaxN = 12, MaxW = 500000;
int n, dist[MaxW], mark[MaxW];
long long L, R, A[MaxN];

typedef std::pair<int, int> dijkstra_t;
std::priority_queue<
	dijkstra_t,
	std::vector<dijkstra_t>,
	std::greater<dijkstra_t> 
> heap;

void dijkstra()
{
	std::fill(dist, dist + A[0], ~0u >> 1);
	dist[0] = 0;
	heap.push(dijkstra_t(0, 0));
	while(!heap.empty())
	{
		int u = heap.top().second;
		heap.pop();
		if(mark[u]) continue;
		mark[u] = 1;

		for(int i = 0; i != n; ++i)
		{
			int v = (u + A[i]) % A[0];
			int w = dist[u] + (u + A[i]) / A[0];
			if(w < dist[v])
			{
				dist[v] = w;
				if(!mark[v])
					heap.push(dijkstra_t(w, v));
			}
		}
	}
}

long long calc(long long x)
{
	long long ans = 0;
	for(int i = 0; i != A[0]; ++i)
		ans += std::max(0LL, (x - i) / A[0] + 1 - dist[i]);
	return ans;
}

int main()
{
	std::scanf("%d %lld %lld", &n, &L, &R);
	for(int i = 0; i != n; ++i)
		std::scanf("%lld", A + i);
	std::sort(A, A + n);
	dijkstra();
	std::printf("%lld\n", calc(R) - calc(L - 1));
	return 0;
}
