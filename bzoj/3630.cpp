/* BZOJ-3630: [JLOI2014]镜面通道
 *   网络流  */
#include <cstdio>
#include <cstring>
#include <algorithm>

const int inf = ~0u >> 1;
const int MaxN = 310, MaxV = 620, MaxE = MaxV * MaxV;
struct cell_t
{
	// 1: circle, 2: rectangle
	int type;
	int x, y, r, x2, y2;
} cells[MaxN];

long long abs(long long x) { return x < 0 ? -x : x; }
long long sqr(long long x) { return x * x; }

bool inrect(int x, int y, const cell_t& r)
{
	return r.x <= x && x <= r.x2 && r.y <= y && y <= r.y2;
}

bool check(cell_t a, cell_t b)
{
	if(a.type > b.type) std::swap(a, b);
	if(a.type == 1)
	{
		if(b.type == 1)
		{
			return sqr(a.x - b.x) + sqr(a.y - b.y) <= sqr(a.r + b.r);
		} else if(b.type == 2) {
			if(inrect(a.x, a.y, b)) return 1;
			if(b.y <= a.y && a.y <= b.y2 && (abs(a.x - b.x) <= a.r || abs(a.x - b.x2) <= a.r)) return 1;
			if(b.x <= a.x && a.x <= b.x2 && (abs(a.y - b.y) <= a.r || abs(a.y - b.y2) <= a.r)) return 1;
			if(sqr(a.x - b.x) + sqr(a.y - b.y) <= sqr(a.r)) return 1;
			if(sqr(a.x - b.x2) + sqr(a.y - b.y) <= sqr(a.r)) return 1;
			if(sqr(a.x - b.x) + sqr(a.y - b.y2) <= sqr(a.r)) return 1;
			if(sqr(a.x - b.x2) + sqr(a.y - b.y2) <= sqr(a.r)) return 1;
		} 
	} else {
		if(b.type == 2) return !(a.y2 < b.y || b.y2 < a.y || a.x2 < b.x || b.x2 < a.x);
	}

	return 0;
}

class netflow_t
{
	int total, s, t;
	int head[MaxV], point[MaxE], next[MaxE], cap[MaxE];
	int cur[MaxV], que[MaxV], flag[MaxV], dist[MaxV];
public:
	netflow_t() : total(1) {}
	void add_edge(int u, int v, int f, int r = 0)
	{
		point[++total] = v;
		cap[total] = f;
		next[total] = head[u];
		head[u] = total;

		point[++total] = u;
		cap[total] = r;
		next[total] = head[v];
		head[v] = total;
	}

	bool bfs()
	{
		std::memset(flag, 0, sizeof(flag));
		int qhead = 0, qtail = 0;
		flag[t] = 1;
		que[qtail++] = t;
		while(qhead != qtail)
		{
			int u = que[qhead++];
			for(int k = head[u]; k; k = next[k])
			{
				int v = point[k];
				if(cap[k ^ 1] && !flag[v])
				{
					flag[v] = flag[u] + 1;
					que[qtail++] = v;
					if(v == s) return true;
				}
			}
		}

		return flag[s];
	}

	int dfs(int now, int rest)
	{
		if(now == t) return rest;
		int used = 0;
		for(int& k = cur[now]; k; k = next[k])
		{
			int v = point[k];
			if(cap[k] && flag[v] + 1 == flag[now])
			{
				int g = std::min(rest - used, cap[k]);
				g = dfs(v, g);
				used += g;
				cap[k] -= g;
				cap[k ^ 1] += g;
				if(used == rest) return used;
			}
		}

		return used;
	}

	int dinic(int s, int t)
	{
		int ans = 0;
		this->s = s, this->t = t;
		while(bfs())
		{
			std::memcpy(cur, head, sizeof(head));
			ans += dfs(s, inf);
		}
		return ans;
	}
} flow;

int main()
{
	int x, y, n;
	std::scanf("%d %d %d", &x, &y, &n);
	for(int i = 1; i <= n; ++i)
	{
		flow.add_edge(i, i + n, 1, 1);
		std::scanf("%d", &cells[i].type);
		if(cells[i].type == 1)
		{
			std::scanf("%d %d %d", &cells[i].x, &cells[i].y, &cells[i].r);
		} else {
			std::scanf("%d %d", &cells[i].x, &cells[i].y);
			std::scanf("%d %d", &cells[i].x2, &cells[i].y2);
		}

		for(int j = 1; j != i; ++j)
		{
			if(check(cells[i], cells[j]))
			{
				flow.add_edge(i + n, j, inf);
				flow.add_edge(j + n, i, inf);
			}
		}
	}

	cell_t top, bottom;
	top.type = bottom.type = 2;
	top.y = y; top.y2 = y + 1;
	bottom.y = -1; bottom.y2 = 0;
	top.x2 = bottom.x2 = x;
	top.x = bottom.x = 0;
	int s = 2 * n + 1, t = s + 1;
	for(int i = 1; i <= n; ++i)
	{
		if(check(top, cells[i])) 
			flow.add_edge(s, i, inf);
		if(check(bottom, cells[i]))
			flow.add_edge(i + n, t, inf);
	}

	std::printf("%d", flow.dinic(s, t));
	return 0;
}
