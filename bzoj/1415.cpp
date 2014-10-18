/* BZOJ-1415: [Noi2005]聪聪和可可
 *  概率DP */
#include <cstdio>
#include <cstring>

const int MaxN = 1001, MaxE = 2001;
int total, mark_v, N, M;
int head[MaxN], point[MaxE], next[MaxE];
int p[MaxN][MaxN], que[MaxN], dist[MaxN], mark[MaxN];
double dp[MaxN][MaxN];

void spfa(int x)
{
	std::memset(dist, 33, sizeof(dist));
	dist[x] = 0, mark[x] = ++mark_v;
	int qhead = 0, qtail = 0;
	que[qtail++] = x;
	while(qhead != qtail)
	{
		int u = que[qhead++];
		if(qhead == MaxN) qhead = 0;
		for(int k = head[u]; k; k = next[k])
		{
			int v = point[k];
			int d = dist[u] + 1;
			bool enqueue = false;
			if(d == dist[v] && p[x][u] < p[x][v])
			{
				p[x][v] = p[x][u];
				enqueue = true;
			} else if(d < dist[v]) {
				dist[v] = d;
				p[x][v] = p[x][u];
				if(!p[x][v]) p[x][v] = v;
				enqueue = true;
			}

			if(enqueue && mark[v] != mark_v)
			{
				que[qtail++] = v;
				mark[v] = mark_v;
				if(qtail == MaxN)
					qtail = 0;
			}
		}

		mark[u] = 0;
	}

	p[x][x] = x;
}

void add_edge(int u, int v)
{
	point[++total] = v;
	next[total] = head[u];
	head[u] = total;
}

double f(int u, int v)
{
	if(dp[u][v] > 0)
		return dp[u][v];
	else if(u == v)
		return 0.0;
	int deg = 0;
	double sum = 0.0;
	int pos = p[p[u][v]][v];
	if(pos == v) return 1.0;
	for(int k = head[v]; k; k = next[k], ++deg)
		sum += f(pos, point[k]);
	sum += f(pos, v);
	return dp[u][v] = sum / (deg + 1.0) + 1.0;
}

int main()
{
	int a, b;
	std::scanf("%d %d", &N, &M);
	std::scanf("%d %d", &a, &b);
	for(int i = 0; i != M; ++i)
	{
		int u, v;
		std::scanf("%d %d", &u, &v);
		add_edge(u, v);
		add_edge(v, u);
	}

	for(int i = 1; i <= N; ++i)
		spfa(i);
	std::printf("%.3lf", f(a, b));
	return 0;
}
