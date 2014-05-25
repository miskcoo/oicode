/*
 * BZOJ-1927 [Sdoi2010]星际竞速
 *  网络流-费用流
 */
#include <cstdio>
#include <algorithm>
#include <cstring>

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

	int s, t;

	int dist[VSize + 1];
	int mark[VSize + 1];
	int que[VSize + 1];
	int pre[VSize + 1];
	int mark_value;
public:
	void init()
	{
		total = 2;
		mark_value = 0;
		std::memset(mark, 0, sizeof(mark));
		std::memset(head, 0, sizeof(head));
	}

	void add_edge(int u, int v, int flow, int cost)
	{
		point[total] = v;
		from[total] = u;
		next[total] = head[u];
		cap[total] = flow;
		this->cost[total] = cost;
		head[u] = total++;

		point[total] = u;
		from[total] = v;
		next[total] = head[v];
		cap[total] = 0;
		this->cost[total] = -cost;
		head[v] = total++;
	}

	bool spfa()
	{
		++mark_value;
		std::memset(dist, 0x66, sizeof(dist));
		int qhead = 0, qtail = 0;
		mark[s] = mark_value;
		que[qtail++] = s;
		dist[s] = 0;

		while(qhead != qtail)
		{
			int u = que[qhead++];
			if(qhead == VSize + 1)
				qhead = 0;
			for(int k = head[u]; k; k = next[k])
			{
				int v = point[k];
				int t = dist[u] + cost[k];
				if(cap[k] && t < dist[v])
				{
					dist[v] = t;
					pre[v] = k;
					if(mark[v] != mark_value)
					{
						mark[v] = mark_value;
						que[qtail++] = v;
						if(qtail == VSize + 1)
							qtail = 0;
					}
				}
			}

			mark[u] = 0;
		}

		return dist[t] != 0x66666666;
	}

	int get_cost(int s, int t)
	{
		int answer = 0;
		this->s = s, this->t = t;
		while(spfa())
		{
			int count = 0;
			int min_cap = ~0U >> 1;
			for(int u = t; u != s; u = from[pre[u]])
				min_cap = std::min(min_cap, cap[pre[u]]);
			for(int u = t; u != s; u = from[pre[u]])
			{
				int k = pre[u];
				cap[k] -= min_cap;
				cap[k ^ 1] += min_cap;
				count += cost[k];
			}

			if(count >= 0) break;
			answer += min_cap * count;
		}

		return answer;
	}
};

costflow<1603, 35000> cf;

int main()
{
	int N, M;
	cf.init();
	std::scanf("%d %d", &N, &M);
	int s = 1603, t = 1602;
	int ans = 0;
	for(int i = 1; i <= N; ++i)
	{
		int v;
		std::scanf("%d", &v);
		ans += v;
		cf.add_edge(s, i * 2, 1, 0);
		cf.add_edge(i * 2 - 1, t, 1, -v);
	}

	for(int i = 0; i != M; ++i)
	{
		int u, v, w;
		std::scanf("%d %d %d", &u, &v, &w);
		if(u > v) std::swap(u, v);
		cf.add_edge(u * 2, v * 2 - 1, 1, w);
	}

	std::printf("%d", ans + cf.get_cost(s, t));
	return 0;
}
