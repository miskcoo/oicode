/* BZOJ-1791 [Ioi2008]Island 岛屿 
     环套树 */
#include <cstdio>
#include <algorithm>

const int MaxV = 1000001, MaxE = 2000001;
int head[MaxV], next[MaxE], point[MaxE], total;
int block[MaxV], block_num;
int que[MaxV * 2], deg[MaxV], circle[MaxV * 2];
long long dist[MaxV], block_dist[MaxV], weight[MaxE];
long long sum[MaxV * 2];

void add_edge(int u, int v, int w)
{
	point[++total] = v;
	weight[total] = w;
	next[total] = head[u];
	head[u] = total;
	++deg[v];
}

void make_block(int u)
{
	int qhead = 0, qtail = 0;
	que[qtail++] = u;
	++block_num;

	while(qhead != qtail)
	{
		int u = que[qhead++];
		block[u] = block_num;

		for(int k = head[u]; k; k = next[k])
		{
			int v = point[k];
			if(!block[v])
				que[qtail++] = v;
		}
	}
}

void topology_sort(int N)
{
	int qhead = 0, qtail = 0;
	for(int u = 1; u <= N; ++u)
	{
		if(deg[u] == 1)
			que[qtail++] = u;
	}

	while(qhead != qtail)
	{
		int u = que[qhead++];
		int bk = block[u];
		for(int k = head[u]; k; k = next[k])
		{
			int v = point[k];
			if(deg[v] == 1) continue;
			long long d = dist[u] + weight[k];
			block_dist[bk] = std::max(block_dist[bk], d + dist[v]);
			dist[v] = std::max(dist[v], d);

			if(--deg[v] == 1)
				que[qtail++] = v;
		}
	}
}

long long solve_circle(int u)
{
	int num = 0, now = u;

	// find circle
	circle[num++] = u;
	int tmp;
	while(num == 1 || now != u)
	{
		bool found = false;
		for(int k = head[now]; k; k = next[k])
		{
			int v = point[k];
			if(num > 1 && v == circle[num - 2])
			{
				tmp = k;
				continue;
			}

			if(deg[v] == 2)
			{
				sum[num] = sum[num - 1] + weight[k];
				now = circle[num++] = v;
				--deg[v];
				found = true;
				break;
			}
		}

		if(!found)
		{
			sum[num] = sum[num - 1] + weight[tmp];
			now = circle[num++] = point[tmp];
			--deg[point[tmp]];
		}
	}

	// copy circle
	int len = num - 1;
	for(int i = 1; i != len; ++i)
	{
		circle[len + i] = circle[i];
		sum[len + i] = sum[len] + sum[i];
	}

	// solve
	long long ans = 0;
	int qhead = 0, qtail = 0;
	for(int i = 0, size = len * 2; i != size; ++i)
	{
		if(qhead != qtail)
		{
			while(que[qhead] + len <= i)
				++qhead;
			int q = que[qhead];
			long long d = dist[circle[q]] - sum[q];
			ans = std::max(ans, sum[i] + dist[circle[i]] + d);
		}

		long long now = dist[circle[i]] - sum[i];
		while(qhead != qtail)
		{
			int q = que[qtail - 1];
			long long d = dist[circle[q]] - sum[q];
			if(now >= d) --qtail;
			else break;
		}

		que[qtail++] = i;
	}

	return std::max(ans, block_dist[block[u]]);
}

int main()
{
	int N;
	std::scanf("%d", &N);
	for(int u = 1; u <= N; ++u)
	{
		int w, v;
		std::scanf("%d %d", &v, &w);
		add_edge(u, v, w);
		add_edge(v, u, w);
	}

	for(int u = 1; u <= N; ++u)
	{
		if(!block[u])
			make_block(u);
	}

	topology_sort(N);

	long long ans = 0;
	for(int u = 1; u <= N; ++u)
	{
		if(deg[u] == 2)
			ans += solve_circle(u);
	}

	std::printf("%lld", ans);
	return 0;
}
