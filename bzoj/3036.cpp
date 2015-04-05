/* BZOJ-3036: 绿豆蛙的归宿
 *  拓扑排序，概率DP */
#include <cstdio>

const int MaxN = 100010, MaxM = MaxN * 2;
int n, m, total;
int head[MaxN], point[MaxM], next[MaxM], weight[MaxM];
int deg[MaxN], que[MaxN], deg2[MaxN];
double f[MaxN];

void add_edge(int u, int v, int w)
{
	++deg[v], ++deg2[v];
	point[++total] = v;
	weight[total] = w;
	next[total] = head[u];
	head[u] = total;
}

double solve()
{
	int qhead = 0, qtail = 0;
	f[n] = 0.0;
	que[qtail++] = n;
	while(qhead != qtail)
	{
		int u = que[qhead++];
		for(int k = head[u]; k; k = next[k])
		{
			int v = point[k];
			f[v] += (f[u] + weight[k]) / double(deg2[v]);
			if(--deg[v] == 0)
				que[qtail++] = v;
		}
	}

	return f[1];
}

int main()
{
	std::scanf("%d %d", &n, &m);
	for(int i = 0; i != m; ++i)
	{
		int u, v, w;
		std::scanf("%d %d %d", &u, &v, &w);
		add_edge(v, u, w);
	}

	std::printf("%.2lf", solve());
	return 0;
}
