/* BZOJ-1854: [Scoi2010]游戏 
 *  二分图匹配 */
#include <cstdio>

const int MaxN = 2010010;
int n, mark_v, total;
int head[MaxN], point[MaxN], next[MaxN];
int mat[MaxN], mark[MaxN];

void add_edge(int u, int v)
{
	point[++total] = v;
	next[total] = head[u];
	head[u] = total;
}

bool augment(int u)
{
	for(int k = head[u]; k; k = next[k])
	{
		int v = point[k];
		if(mark[v] == mark_v)
			continue;
		mark[v] = mark_v;
		if(!mat[v] || augment(mat[v]))
		{
			mat[v] = u;
			return true;
		}
	}

	return false;
}

int main()
{
	std::scanf("%d", &n);
	for(int i = 1; i <= n; ++i)
	{
		int a, b;
		std::scanf("%d %d", &a, &b);
		add_edge(a, i);
		add_edge(b, i);
	}

	int ans = 1;
	for(++mark_v; augment(ans); ++ans, ++mark_v);
	std::printf("%d", ans - 1);
	return 0;
}
