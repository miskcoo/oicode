/* BZOJ-4010: [HNOI2015]菜肴制作
 *  拓扑排序+堆 */
#include <queue>
#include <cstdio>
#include <vector>
#include <functional>
#include <cstring>

const int MaxN = 100010, MaxM = 100010;
int total;
int head[MaxN], point[MaxM], next[MaxM];
int deg[MaxN], que[MaxN], ans[MaxN];
std::priority_queue<int> heap;

void add_edge(int u, int v)
{
	++deg[v];
	point[++total] = v;
	next[total] = head[u];
	head[u] = total;
}

void reset()
{
	total = 0;
	std::memset(head, 0, sizeof(head));
	std::memset(deg, 0, sizeof(deg));
}

void solve()
{
	reset();
	int n, m;
	std::scanf("%d %d", &n, &m);
	for(int i = 0; i != m; ++i)
	{
		int u, v;
		std::scanf("%d %d", &u, &v);
		add_edge(v, u);
	}

	int now = 0;
	for(int i = 1; i <= n; ++i)
		if(!deg[i]) heap.push(i);
	while(!heap.empty())
	{
		int u = heap.top();
		ans[now++] = u;
		heap.pop();
		for(int k = head[u]; k; k = next[k])
		{
			int v = point[k];
			if(--deg[v] == 0)
				heap.push(v);
		}
	}

	if(n == now)
	{
		for(int i = n - 1; i >= 0; --i)
			std::printf("%d ", ans[i]);
		std::puts("");
	} else std::puts("Impossible!");
}

int main()
{
	int t;
	std::scanf("%d", &t);
	while(t --> 0) solve();
	return 0;
}
