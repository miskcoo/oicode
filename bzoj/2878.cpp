/* BZOJ-2878: [Noi2012]迷失游乐园
 *  环套树+概率DP */
#include <cstdio>
#include <cstring>

const int MaxN = 100001, MaxE = 200001;
int total;
int N, M;
int head[MaxN], point[MaxE], weight[MaxE], next[MaxE];
double up[MaxN], down[MaxN];
int deg[MaxN], que[MaxN], deg2[MaxN];
int circle[MaxN], circle_num;

void add_edge(int u, int v, int w)
{
	weight[++total] = w;
	point[total] = v;
	next[total] = head[u];
	head[u] = total;
	++deg[u];
}

void tree_down(int u, int fa)
{
	int son = 0;
	for(int k = head[u]; k; k = next[k])
	{
		int v = point[k];
		if(v == fa) continue;
		tree_down(v, u);
		down[u] += weight[k] + down[v];
		++son;
	}

	if(son) down[u] /= son;
	else down[u] = 0.0;
}

void tree_up(int u, int fa)
{
	int son = 0;
	double sum = 0.0;
	for(int k = head[u]; k; k = next[k])
	{
		int v = point[k];
		if(v == fa) continue;
		sum += weight[k] + down[v];
		++son;
	}

	if(fa)
	{
		++son;
		sum += up[u];
	} else if(son == 1) {
		++son;
	}

	for(int k = head[u]; k; k = next[k])
	{
		int v = point[k];
		if(v == fa) continue;
		up[v] = (sum - down[v] - weight[k]) / (son - 1.0) + weight[k];
		tree_up(v, u);
	}
}

void circle_down(int u, int fa)
{
	int son = 0;
	for(int k = head[u]; k; k = next[k])
	{
		int v = point[k];
		if(v == fa || deg[v]) continue;
		circle_down(v, u);
		down[u] += weight[k] + down[v];
		++son;
	}

	if(son) down[u] /= son;
	else down[u] = 0.0;
}

void circle_up(int u, int fa)
{
	int son = 0;
	double sum = 0.0;
	for(int k = head[u]; k; k = next[k])
	{
		int v = point[k];
		if(v == fa || deg[v])
			continue;
		sum += weight[k] + down[v];
		++son;
	}

	if(!fa)
	{
		son += 2;
		sum += up[u] * 2.0;
	} else {
		++son;
		sum += up[u];
	}

	for(int k = head[u]; k; k = next[k])
	{
		int v = point[k];
		if(v == fa || deg[v])
			continue;
		up[v] = (sum - weight[k] - down[v]) / (son - 1.0) + weight[k];
		circle_up(v, u);
	}
}

void solve_circle(int u)
{
	double sum = 0.0;
	for(int k = head[u]; k; k = next[k])
	{
		if(!deg[point[k]]) continue;
		double p = 0.5, len = weight[k];
		int nxt = point[k], from = u;
		while(nxt)
		{
			if(deg2[nxt] > 2)
			{
				sum += (down[nxt] + len) * p 
					* (deg2[nxt] - 2.0) / (deg2[nxt] - 1.0);
				p /= deg2[nxt] - 1.0;
			}

			int temp = 0;
			for(int t = head[nxt]; t; t = next[t])
			{
				int v = point[t];
				if(!deg[v] || v == from || v == u)
					continue;
				temp = v;
				len += weight[t];
				break;
			}

			from = nxt;
			nxt = temp;
		}

		if(deg2[from] == 2)
		{
			sum += len * p;
		} else {
			sum += (len + down[from]) * p;
		}
	}

	up[u] = sum;
}

void topology_sort()
{
	int qhead = 0, qtail = 0;
	for(int i = 1; i <= N; ++i)
	{
		if(deg[i] == 1)
		{
			que[qtail++] = i;
			deg[i] = 0;
		}
	}

	while(qhead != qtail)
	{
		int u = que[qhead++];
		for(int k = head[u]; k; k = next[k])
		{
			int v = point[k];
			if(!deg[v]) 
				continue;
			if(--deg[v] == 1)
			{
				que[qtail++] = v;
				deg[v] = 0;
			}
		}
	}
}

void find_circle()
{
	int first = 1;
	for(; first <= N; ++first)
	{
		if(deg[first]) 
			break;
	}

	circle_num = 1;
	circle[0] = first;
	int u = first;
	do {
		int next_v = 0, pre = 0;
		if(circle_num >= 2) pre = circle[circle_num - 2];
		for(int k = head[u]; k; k = next[k])
		{
			int v = point[k];
			if(v == pre || !deg[v])
				continue;
			next_v = v;
			break;
		}

		if(next_v == 0)
		{
			for(int k = head[u]; k; k = next[k])
			{
				int v = point[k];
				if(!deg[v]) continue;
				next_v = v;
				break;
			}
		}

		u = circle[circle_num++] = next_v;
	} while(u != first);

	--circle_num;
}

int main()
{
	std::scanf("%d %d", &N, &M);
	for(int i = 0; i != M; ++i)
	{
		int u, v, w;
		std::scanf("%d %d %d", &u, &v, &w);
		add_edge(u, v, w);
		add_edge(v, u, w);
	}

	if(N == M + 1)
	{
		tree_down(1, 0);
		tree_up(1, 0);
		double ans = down[1];
		for(int u = 2; u <= N; ++u)
			ans += (up[u] + down[u] * (deg[u] - 1.0)) / deg[u];
		std::printf("%lf", ans / N);
	} else {
		std::memcpy(deg2, deg, sizeof(deg));
		topology_sort();
		find_circle();
		for(int i = 0; i != circle_num; ++i)
		{
			circle_down(circle[i], 0);
			deg[circle[i]] = deg2[circle[i]];
		}

		for(int i = 0; i != circle_num; ++i)
			solve_circle(circle[i]);

		for(int i = 0; i != circle_num; ++i)
			circle_up(circle[i], 0);

		double ans = 0.0;
		for(int u = 1; u <= N; ++u)
		{
			int w = deg[u] ? 2 : 1;
			ans += (up[u] * w + down[u] * (deg2[u] - w)) / deg2[u];
		}

		std::printf("%lf", ans / N);
	}
	return 0;
}
