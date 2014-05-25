/* 
 * BZOJ-1061 志愿者招募
 *  费用流-代数构图
 */
#include <cstdio>
#include <cstring>

const long long max_int = 0xffffffffffffLL;
template<typename Type, int MaxVertex, int MaxEdge>
class cost_flow
{
	int last[MaxVertex + 1];
	int next[(MaxEdge + 3) * 2];
	int point[(MaxEdge + 3) * 2];
	int from[(MaxEdge + 3) * 2];
	Type cap[(MaxEdge + 3) * 2];
	Type cost[(MaxEdge + 3) * 2];

	int total;
	int pre[(MaxEdge + 3) * 2];
	Type dist[MaxEdge + 1];
	int S, T;

	bool mark[MaxVertex + 1];
	int que[MaxVertex + 1];

	bool spfa()
	{
		int head = 0, tail = 1;
		for(int i = 0; i <= MaxVertex; ++i)
		{
			dist[i] = max_int;
			mark[i] = false;
		}

		dist[S] = 0;
		que[0] = S;
		mark[T] = true;

		while(head != tail)
		{
			int v = que[head++];
			if(head + 1 == MaxVertex) head = 0;
			for(int k = last[v]; k; k = next[k])
			{
				int i = point[k];
				if(cap[k] && dist[i] > dist[v] + cost[k])
				{
					dist[i] = dist[v] + cost[k];
					pre[i] = k;
					if(mark[i] == false)
					{
						mark[i] = true;
						que[tail++] = i;
						if(tail + 1 == MaxVertex) tail = 0;
					}
				}
			}
			mark[v] = false;
		}
		return dist[T] != max_int;
	}

	Type solve()
	{
		Type answer = 0;
		Type min_cap = max_int;
		for(int i = T; i != S; i = from[pre[i]])
		{
			if(cap[pre[i]] < min_cap)
				min_cap = cap[pre[i]];
		}
		for(int i = T; i != S; i = from[pre[i]])
		{
			cap[pre[i]] -= min_cap;
			cap[pre[i] ^ 1] += min_cap;
			answer += cost[pre[i]] * min_cap;
		}
		return answer;
	}
public:
	void init()
	{
		total = 2;
		std::memset(last, 0, sizeof(last));
	}

	Type get_cost(int s, int t)
	{
		S = s, T = t;
		Type answer = 0;
		while(spfa()) answer += solve();
		return answer;
	}

	void set_edge(int f, int t, Type c, Type s)
	{
		cap[total] = c;
		cost[total] = s;
		point[total] = t;
		from[total] = f;
		next[total] = last[f];
		last[f] = total++;

		cap[total] = 0;
		cost[total] = -s;
		point[total] = f;
		from[total] = t;
		next[total] = last[t];
		last[t] = total++;
	}
};

cost_flow<long long, 2100, 50000> cf;

int main()
{
	int N, M;
	std::scanf("%d %d", &N, &M);

	cf.init();

	int s = 2011, t = 2012;
	int pre = 0;
	for(int i = 1; i <= N; ++i)
	{
		int v;
		std::scanf("%d", &v);
		cf.set_edge(i + 1, i, max_int, 0);
		if(pre < v) cf.set_edge(s, i, v - pre, 0);
		else cf.set_edge(i, t, pre - v, 0);
		pre = v;
	}

	cf.set_edge(N + 1, t, pre, 0);

	for(int i = 1; i <= M; ++i)
	{
		int u, v, w;
		std::scanf("%d %d %d", &u, &v, &w);
		cf.set_edge(u, v + 1, max_int, w);
	}

	std::printf("%lld", cf.get_cost(s, t));
	return 0;
}
