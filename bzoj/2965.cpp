/* BZOJ-2965: 保护古迹
 *   平面图+最小割 */
#include <cstdio>
#include <cstring>
#include <vector>
#include <cmath>
#include <algorithm>

const int inf = ~0u >> 1;
const double pi = acos(-1), eps = 1.0e-8;
const int MaxP = 15, MaxN = 310, MaxM = MaxN * MaxN;

bool cmp(double x, double y) { return x - y > -eps && x - y < eps; }

struct point_t
{
	double x, y;
	point_t(double x = 0, double y = 0) 
		: x(x), y(y) {}
};

double cross(const point_t& a, const point_t& b)
{
	return a.x * b.y - a.y * b.x;
}

int p, n, m;
namespace planar_graph
{
	point_t keys[MaxP], pts[MaxN];
	struct edge_t
	{
		double k, angle;
		int u, v, w;
		void calc()
		{
			point_t z = point_t(pts[v].x - pts[u].x, pts[v].y - pts[u].y);
			angle = atan2(z.y, z.x);
			if(angle < 0) angle += 2.0 * pi;

			if(!cmp(pts[u].x, pts[v].x))
				k = z.y / z.x;
			else k = 1.0e10;
		}
	} edges[MaxM];
	int area_num, inf_area;
	int area[MaxP], rank[MaxM], near[MaxM], vis[MaxM];
	std::vector<int> et[MaxN];

	void read()
	{
		for(int i = 0; i != p; ++i)
			std::scanf("%lf %lf", &keys[i].x, &keys[i].y);
		for(int i = 1; i <= n; ++i)
			std::scanf("%lf %lf", &pts[i].x, &pts[i].y);
		for(int i = 0; i != m; ++i)
		{
			int a = i << 1;
			std::scanf("%d %d %d", &edges[a].u, &edges[a].v, &edges[a].w);
			edges[a].calc();
			edges[a ^ 1] = edges[a];
			std::swap(edges[a ^ 1].u, edges[a ^ 1].v);
			edges[a ^ 1].calc();
		}
	}

	void find_region(int x, int eid)
	{
		double area = 0;
		while(!vis[eid])
		{
			area += cross(pts[x], pts[edges[eid].v]);
			vis[eid] = 1;
			near[eid] = area_num;
			x = edges[eid].v;
			if(!rank[eid ^ 1]) eid = et[x].back();
			else eid = et[x][rank[eid ^ 1] - 1];
		}

		if(area < 0) inf_area = area_num;
		++area_num;
	}

	void dual()
	{
		std::pair<double, int> *tmp = new std::pair<double, int>[m << 1];
		for(int i = 0; i != m << 1; ++i)
			tmp[i] = std::make_pair(edges[i].angle, i);
		std::sort(tmp, tmp + (m << 1));
		for(int i = 0; i != m << 1; ++i)
		{
			int eid = tmp[i].second;
			rank[eid] = et[edges[eid].u].size();
			et[edges[eid].u].push_back(eid);
		}
		delete[] tmp;

		for(int i = 1; i <= n; ++i)
		{
			for(int j = 0; j != et[i].size(); ++j)
				if(!vis[et[i][j]]) find_region(i, et[i][j]);
		}
	}

	void scan()
	{
		for(int i = 0; i != p; ++i)
		{
			int area_id = inf_area;
			double nowy = 1.0e10, nowk;
			for(int j = 0; j < m << 1; j += 2)
			{
				point_t u = pts[edges[j].u], v = pts[edges[j].v];
				if(u.x > v.x) 
				{
					j ^= 1;
					u = pts[edges[j].u], v = pts[edges[j].v];
				}

				if(u.x > keys[i].x || v.x < keys[i].x) continue;

				double y0 = edges[j].k * (keys[i].x - u.x) + u.y;
				if(y0 > keys[i].y && y0 < nowy)
				{
					area_id = near[j ^ 1];
					nowy = y0, nowk = edges[j].k;
				} else if(cmp(y0, keys[i].y) && 0 <= edges[j].k && edges[j].k < nowk) {
					area_id = near[j ^ 1];
					nowk = edges[j].k;
				}
			}

			area[i] = area_id;
		}
	}

	void work()
	{
		read();
		dual();
		scan();
	}
}

namespace network_flow
{
	const int MaxV = MaxN + MaxP + 10, MaxE = MaxV * MaxV;
	int s, t, total;
	int head[MaxV], next[MaxE], point[MaxE], cap[MaxE];
	int cur[MaxV], flag[MaxV], que[MaxV];

	void reset()
	{
		total = 1;
		std::memset(head, 0, sizeof(head));
	}

	void add_edge(int u, int v, int f1, int f2 = 0)
	{
		point[++total] = v;
		next[total] = head[u];
		cap[total] = f1;
		head[u] = total;

		point[++total] = u;
		next[total] = head[v];
		cap[total] = f2;
		head[v] = total;
	}

	bool bfs()
	{
		std::memset(flag, 0, sizeof(flag));
		int qhead = 0, qtail = 0;
		que[qtail++] = t;
		flag[t] = 1;
		while(qhead != qtail)
		{
			int u = que[qhead++];
			for(int k = head[u]; k; k = next[k])
			{
				int v = point[k];
				if(!flag[v] && cap[k ^ 1])
				{
					flag[v] = flag[u] + 1;
					que[qtail++] = v;
					if(v == s) return true;
				}
			}
		}

		return flag[s];
	}

	int dfs(int u, int rest)
	{
		if(u == t) return rest;
		int used = 0;
		for(int& k = cur[u]; k; k = next[k])
		{
			int v = point[k];
			if(cap[k] && flag[v] + 1 == flag[u])
			{
				int g = std::min(rest - used, cap[k]);
				g = dfs(v, g);
				cap[k] -= g;
				cap[k ^ 1] += g;
				used += g;
				if(used == rest) return used;
			}
		}

		return used;
	}

	int dinic()
	{
		int ans = 0;
		while(bfs()) 
		{
			std::memcpy(cur, head, sizeof(head));
			ans += dfs(s, inf);
		}
		return ans;
	}

	int calc(int s0)
	{
		reset();
		using namespace planar_graph;
		for(int i = 0; i != m << 1; i += 2)
		{
			int w = edges[i].w;
			add_edge(near[i], near[i ^ 1], w, w);
		}

		s = MaxV - 1, t = inf_area;
		for(int i = 0; i != p; ++i)
		{
			if(s0 & (1 << i))
			{
				add_edge(i + area_num, area[i], inf);
				add_edge(s, i + area_num, inf);
			}
		}

		return dinic();
	}

	void work()
	{
		static int bitcnt[1 << MaxP], ans[MaxP + 1];
		for(int i = 1; i != 1 << p; ++i)
			bitcnt[i] = bitcnt[i ^ (i & -i)] + 1;
		for(int i = 0; i <= p; ++i) ans[i] = inf;
		for(int s = 1; s != 1 << p; ++s)
			ans[bitcnt[s]] = std::min(ans[bitcnt[s]], calc(s));
		for(int i = 1; i <= p; ++i)
			std::printf("%d\n", ans[i]);
	}
}

int main()
{
	std::scanf("%d %d %d", &p, &n, &m);
	planar_graph::work();
	network_flow::work();
	return 0;
}
