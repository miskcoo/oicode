/*
 * BZOJ-3232 圈地游戏
 *  分数规划+网络流
 */
#include <cstdio>
#include <cstring>

template<int MaxVertex, int MaxEdge>
class dinic
{
	int cur[MaxVertex + 1];
	int head[MaxVertex + 1];
	int next[(MaxEdge + 3) * 2];
	int point[(MaxEdge + 3) * 2];
	double cap[(MaxEdge + 3) * 2];
	int que[(MaxEdge + 3) * 2];
	int flag[MaxVertex + 1];
	int S, T;
	
	int total;
	double answer;
public:
	bool bfs()
	{
		std::memset(flag, 0, sizeof(flag));
		int qh = 0, qt = 1;
		que[0] = S;
		flag[S] = 1;
		while(qh != qt)
		{
			int u = que[qh++];
			for(int k = head[u]; k; k = next[k])
			{
				int v = point[k];
				if(cap[k] > 1.0e-8 && flag[v] == 0)
				{
					flag[v] = flag[u] + 1;
					que[qt++] = v;
					if(v == T) return true;
				}
			}
		}
		return flag[T] != 0;
	}

	double min(double a, double b)
	{
		return a < b ? a : b;
	}

	bool equal(double a, double b)
	{
		 double v = a - b;
		 if(v < 0) return v > -1.0e-8;
		 return v < 1.0e-8;
	}

	double dfs(int now, double min_cap)
	{
		if(now == T)
		{
			answer += min_cap;
			return min_cap;
		}

		double used = 0.0;
		for(int& k = cur[now]; k; k = next[k])
		{
			int v = point[k];
			if(cap[k] > 1.0e-8 && flag[now] + 1 == flag[v])
			{
				double g = dfs(v, min(cap[k], min_cap - used));
				used += g;
				cap[k] -= g;
				cap[k ^ 1] += g;
				if(min_cap <= used || min_cap - used < 1.0e-8)
					return used;
			}
		}
		return used;
	}

	double solve(int s, int t)
	{
		S = s, T = t;
		while(bfs())
		{
			std::memcpy(cur, head, sizeof(head));
			dfs(S, 1.0e10);
		}
		return answer;
	}

	void init()
	{
		total = 2;
		answer = 0;
		std::memset(head, 0, sizeof(head));
	}

	void add_edge(int f, int t, double c, double z)
	{
		cap[total] = c;
		point[total] = t;
		next[total] = head[f];
		head[f] = total++;

		cap[total] = z;
		point[total] = f;
		next[total] = head[t];
		head[t] = total++;
	}
};

dinic<53 * 53 + 3, 53 * 53 * 4> nf;
int hcost[52][52];
int vcost[52][52];
int cost[52][52];

int get_pt(int a, int b)
{
	return a * 53 + b + 3;
}

int main()
{
	int N, M;
	int sum_v = 0;
	std::scanf("%d %d", &N, &M);
	for(int i = 1; i <= N; ++i)
		for(int j = 1; j <= M; ++j)
		{
			std::scanf("%d", cost[i] + j);
			sum_v += cost[i][j];
		}
	for(int i = 0; i <= N; ++i)
		for(int j = 1; j <= M; ++j)
			std::scanf("%d", hcost[i] + j);
	for(int i = 1; i <= N; ++i)
		for(int j = 0; j <= M; ++j)
			std::scanf("%d", vcost[i] + j);
	double l = 0, r = sum_v;
	while(r - l > 1.0e-6)
	{
		double m = (l + r) * 0.5;
		nf.init();
		for(int i = 0; i != N + 2; ++i)
		{
			for(int j = 0; j != M + 2; ++j)
			{
				if(i == 0 || j == 0 || i == N + 1 || j == M + 1)
					nf.add_edge(get_pt(i, j), 2, 1.0e10, 0);
				else nf.add_edge(1, get_pt(i, j), cost[i][j], 0);
			}
		}
		for(int i = 1; i <= N; ++i)
			for(int j = 0; j <= M; ++j)
				nf.add_edge(get_pt(i, j), get_pt(i, j + 1), 
					vcost[i][j] * m, vcost[i][j] * m);
		for(int i = 0; i <= N; ++i)
			for(int j = 1; j <= M; ++j)
				nf.add_edge(get_pt(i, j), get_pt(i + 1, j), 
					hcost[i][j] * m, hcost[i][j] * m);
		double ans = sum_v - nf.solve(1, 2);
		if(ans < 1.0e-8) r = m;
		else l = m;
	}
	std::printf("%.3lf", l);
	return 0;
}
