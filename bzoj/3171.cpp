/*
 * BZOJ-3171 [Tjoi2013]循环格
 *  最小费用最大流，欧拉回路
 */
#include <cstdio>
#include <cstring>
#include <algorithm>

template<int VSize, int ESize>
class costflow
{
	int total;
	int head[VSize + 1];
	int next[ESize + 1];
	int point[ESize + 1];
	int from[ESize + 1];
	int cap[ESize + 1];
	int cost[ESize + 1];
	int S, T;

	int mark_value;
	int mark[VSize + 1];
	int que[VSize + 1];
	int path_from[VSize + 1];
	int dist[VSize + 1];
	bool spfa()
	{
		++mark_value;
		std::memset(dist, 0x33, sizeof(dist));
		int qhead = 0, qtail = 0;
		que[qtail++] = S;
		dist[S] = 0;
		mark[S] = mark_value;
		
		while(qhead != qtail)
		{
			int u = que[qhead++];
			if(qhead > VSize)
				qhead = 0;
			for(int k = head[u]; k; k = next[k])
			{
				int v = point[k];
				if(cap[k] == 0)
					continue;
				int t = dist[u] + cost[k];
				if(t < dist[v])
				{
					dist[v] = t;
					path_from[v] = k;
					if(mark[v] != mark_value)
					{
						mark[v] = mark_value;
						que[qtail++] = v;
						if(qtail > VSize)
							qtail = 0;
					}
				}
			}
			mark[u] = 0;
		}

		return dist[T] != 0x33333333;
	}
public:
	void init()
	{
		total = 2;
		mark_value = 0;
		std::memset(mark, 0, sizeof(mark));
		std::memset(head, 0, sizeof(head));
	}

	int solve(int s, int t)
	{
		S = s, T = t;
		int answer = 0;
		while(spfa())
		{
			int min_cap = ~0U >> 1;
			for(int u = T; u != S; u = from[path_from[u]])
			{
				int k = path_from[u];
				if(min_cap > cap[k])
					min_cap = cap[k];
			}

			for(int u = T; u != S; u = from[path_from[u]])
			{
				int k = path_from[u];
				cap[k] -= min_cap;
				cap[k ^ 1] += min_cap;
				answer += min_cap * cost[k];
			}
		}

		return answer;
	}

	void add_edge(int u, int v, int f, int c)
	{
		point[total] = v;
		from[total] = u;
		cap[total] = f;
		cost[total] = c;
		next[total] = head[u];
		head[u] = total++;

		point[total] = u;
		from[total] = v;
		cap[total] = 0;
		cost[total] = -c;
		next[total] = head[v];
		head[v] = total++;
	}
};

costflow<502, 15 * 15 * 6 * 3> cf;
char data[20][20];

int map_point(int x, int y)
{
	return x * 15 + y;
}
int main()
{
	int N, M;
	cf.init();
	std::scanf("%d %d", &N, &M);
	for(int i = 0; i != N; ++i)
		std::scanf("%s", data[i]);
	int s = 500, t = 501;
	for(int i = 0; i != N; ++i)
	{
		for(int j = 0; j != M; ++j)
		{
			static const int dxy[][2] = {{0, -1}, {1, 0}, {0, 1}, {-1, 0}};
			int v[4] = {1, 1, 1, 1};
			switch(data[i][j])
			{
			case 'L':
				v[3] = 0;
				break;
			case 'R':
				v[1] = 0;
				break;
			case 'U':
				v[0] = 0;
				break;
			case 'D':
				v[2] = 0;
				break;
			}

			for(int k = 0; k != 4; ++k)
			{
				int nx = i + dxy[k][1];
				int ny = j + dxy[k][0];
				if(nx < 0) nx = N - 1;
				if(ny < 0) ny = M - 1;
				if(nx >= N) nx = 0;
				if(ny >= M) ny = 0;
				cf.add_edge(250 + map_point(i, j), map_point(nx, ny), 1, v[k]);
			}

			cf.add_edge(s, 250 + map_point(i, j), 1, 0);
			cf.add_edge(map_point(i, j), t, 1, 0);
		}
	}

	std::printf("%d", cf.solve(s, t));
	return 0;
}
