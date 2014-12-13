/* BZOJ-2409: 地下车会 
 *  上下界网络流 */
#include <cstdio>
#include <cstring>
#include <algorithm>

const int MaxV = 4000, MaxE = 500 * 500 * 10;
class network_flow
{
	int total, s, t;
	int max_flow;
	int head[MaxV];
	int point[MaxE], next[MaxE];
	int cap[MaxE], cur[MaxV];
	int label[MaxV], que[MaxV];
public:
	network_flow() : total(1) {}

	int add_edge(int u, int v, int f)
	{
		point[++total] = v;
		cap[total] = f;
		next[total] = head[u];
		head[u] = total;

		point[++total] = u;
		cap[total] = 0;
		next[total] = head[v];
		head[v] = total;

		return total - 1;
	}

	void dec_cap(int k, int n)
	{
		cap[k] -= n;
	}

	void set_cap(int k, int n)
	{
		cap[k] = n;
	}

	bool label_vertex()
	{
		std::memset(label, 0, sizeof(label));
		int qhead = 0, qtail = 0;
		label[t] = 1;
		que[qtail++] = t;
		while(qhead != qtail)
		{
			int u = que[qhead++];
			for(int k = head[u]; k; k = next[k])
			{
				int v = point[k];
				if(cap[k ^ 1] && !label[v])
				{
					label[v] = label[u] + 1;
					que[qtail++] = v;
					if(v == s) return true;
				}
			}
		}

		return label[s] != 0;
	}

	int multi_augment(int u, int cap_limit)
	{
		if(u == t) return cap_limit;

		int cap_used = 0;
		for(int& k = cur[u]; k; k = next[k])
		{
			int v = point[k];
			if(!cap[k] || label[v] + 1 != label[u]) 
				continue;
			int ts = std::min(cap_limit - cap_used, cap[k]);
			int ret = multi_augment(v, ts);
			cap_used += ret;
			cap[k] -= ret;
			cap[k ^ 1] += ret;
			if(cap_used == cap_limit)
				return cap_used;
		}

		return cap_used;
	}

	int dinic(int s, int t)
	{
		this->s = s, this->t = t;
		while(label_vertex())
		{
			std::memcpy(cur, head, sizeof(cur));
			max_flow += multi_augment(s, ~0u >> 1);
		}

		return max_flow;
	}
} flow;

int count[501];
int record[501][501];

int main()
{
	int n, m, k;
	std::scanf("%d %d %d", &n, &m, &k);
	int s = (n + m + 1) * 2, t = s + 1;
	int ss = t + 1, st = t + 2, s2 = st + 1;
	for(int i = 1; i <= n; ++i)
	{
		int num;
		std::scanf("%d", &num);
		for(int j = 0; j != num; ++j)
		{
			int type;
			std::scanf("%d", &type);
			++count[type];
		}

		for(int j = 1; j <= m; ++j)
		{
			if(!count[j]) continue;
			record[i][j] = flow.add_edge(i * 2 + 1, (n + j) * 2, count[j]);
			count[j] = 0;
		}
	}

	int rule_num;
	std::scanf("%d", &rule_num);
	for(int i = 1; i <= rule_num; ++i)
	{
		int a, b;
		std::scanf("%d %d", &a, &b);
		flow.add_edge(a * 2 + 1, st, 1);
		flow.add_edge(ss, (n + b) * 2, 1);
		flow.dec_cap(record[a][b], 1);
	}

	for(int i = 1; i <= m; ++i)
	{
		int limit;
		std::scanf("%d", &limit);
		flow.add_edge((n + i) * 2, (n + i) * 2 + 1, limit);
		flow.add_edge((n + i) * 2 + 1, t, limit);
	}

	for(int i = 1; i <= n; ++i)
	{
		int p, l;
		std::scanf("%d %d", &p, &l);

		int u = i * 2, v = u + 1;
		flow.add_edge(u, v, p - l);
		flow.add_edge(ss, v, l);
		flow.add_edge(u, st, l);
		flow.add_edge(s2, u, ~0u >> 1);
	}

	flow.add_edge(s, s2, k);
	int e = flow.add_edge(t, s, ~0u >> 1);
	flow.dinic(ss, st);
	flow.set_cap(e, 0);
	flow.set_cap(e ^ 1, 0);
	std::printf("%d", flow.dinic(s, t) - rule_num);
	return 0;
}

