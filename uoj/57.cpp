/* UOJ-#57. 【WC2013】平面图
 *   平面图对偶图+点定位 */
#include <cstdio>
#include <cstring>
#include <vector>
#include <algorithm>
#include <cmath>
#include <set>

const double pi = acos(-1), eps = 1.0e-8;
const int MaxL = 18, MaxN = 200010, MaxM = MaxN << 1, MaxQ = MaxM;

bool cmp(double a, double b) { return a - b < eps && a - b > -eps; }
struct point_t
{
	double x, y;
	point_t(double x = 0, double y = 0) : x(x), y(y) {}
	point_t operator - (const point_t& r) const
	{
		return point_t(x - r.x, y - r.y);
	}

	friend double cross(const point_t& a, const point_t& b)
	{
		return a.x * b.y - a.y * b.x;
	}
};

int n, m;

namespace planar_graph
{
	point_t pts[MaxN];

	struct edge_t
	{
		int u, v, len;
		double angle;
		edge_t(int u = 0, int v = 0, int len = 0) : u(u), v(v), len(len) 
		{
			point_t z = pts[v] - pts[u];
			angle = atan2(z.y, z.x);
			if(angle < 0) angle += 2.0 * pi;
		}
	} edges[MaxM];

	using std::vector;
	using std::pair;
	using std::make_pair;

	int region_tot, inf_area;
	vector<int> et[MaxN];
	int rank[MaxM], near[MaxM], vis[MaxM];

	void read()
	{
		for(int i = 1; i <= n; ++i)
			std::scanf("%lf %lf", &pts[i].x, &pts[i].y);

		for(int i = 0; i != m; ++i)
		{
			int u, v, w;
			std::scanf("%d %d %d", &u, &v, &w);
			edges[i << 1] = edge_t(u, v, w);
			edges[(i << 1) + 1] = edge_t(v, u, w);
		}
	}

	void find_region(int x, int eid)
	{
		if(vis[eid]) return;
		double area = 0;
		while(!vis[eid])
		{
			area += cross(pts[x], pts[edges[eid].v]);
			vis[eid] = 1;
			near[eid] = region_tot;
			x = edges[eid].v;
			if(!rank[eid ^ 1]) eid = et[x].back();
			else eid = et[x][rank[eid ^ 1] - 1];
		}

		if(area < 0) inf_area = region_tot;
		++region_tot;
	}

	void work()
	{
		read();

		pair<double, int> *tmp = new pair<double, int>[m << 1];
		for(int i = 0; i != m << 1; ++i)
			tmp[i] = make_pair(edges[i].angle, i);
		sort(tmp, tmp + (m << 1));
		for(int i = 0; i != m << 1; ++i)
		{
			int eid = tmp[i].second;
			edge_t e = edges[eid];
			rank[eid] = et[e.u].size();
			et[e.u].push_back(eid);
		}

		delete[] tmp;

		for(int i = 1; i <= n; ++i)
		{
			for(int j = 0; j != et[i].size(); ++j)
				find_region(i, et[i][j]);
		}
	}
}

namespace scanning_line
{
	struct ques_t
	{
		int id;
		point_t pt;
		bool operator < (const ques_t& r) const
		{
			return pt.x < r.pt.x;
		}
	} ques[MaxQ];

	struct key_point_t
	{
		int id;
		point_t pt;
		bool operator < (const key_point_t& r) const
		{
			if(cmp(pt.x, r.pt.x)) 
				return id < r.id;
			return pt.x < r.pt.x;
		}
	} kpt[MaxM];

	double nowx;
	struct info_t
	{
		int id;
		double k, x, y;
		info_t(int id, point_t a, point_t b) : id(id), x(a.x), y(a.y)
		{
			k = (a.y - b.y) / (a.x - b.x);
		}

		bool operator < (const info_t& r) const
		{
			double y0 = k * (nowx - x) + y;
			double y1 = r.k * (nowx - r.x) + r.y;
			if(!cmp(y0, y1)) return y0 < y1;
			return k < r.k;
		}
	};

	using std::swap;
	using std::set;
	typedef set<info_t>::iterator iter_t;
	set<info_t> s;
	iter_t it[MaxM];
	int area[MaxQ];

	int ques_tot;
	void read()
	{
		std::scanf("%d", &ques_tot);
		for(int i = 0; i != ques_tot << 1; ++i)
		{
			ques[i].id = i;
			std::scanf("%lf %lf", &ques[i].pt.x, &ques[i].pt.y);
		}
	}

	void work()
	{
		using namespace planar_graph;

		read();
		int tot = 0;
		for(int i = 0; i != m; ++i)
		{
			int a = i << 1;
			if(pts[edges[a].u].x > pts[edges[a].v].x)
				a |= 1;

			if(!cmp(pts[edges[a].u].x, pts[edges[a].v].x)) {
				kpt[tot].id = a + 1;
				kpt[tot++].pt = pts[edges[a].u];
				kpt[tot].id = -(a + 1);
				kpt[tot++].pt = pts[edges[a].v];
			}
		}
		
		std::sort(kpt, kpt + tot);
		std::sort(ques, ques + (ques_tot << 1));

		for(int i = 0, j = 0; i != ques_tot << 1; ++i)
		{
			for(; j != tot && kpt[j].pt.x <= ques[i].pt.x; ++j)
			{
				nowx = kpt[j].pt.x;
				int id = kpt[j].id;
				if(id < 0)
				{
					s.erase(it[-id - 1]);
				} else {
					--id;
					it[id] = s.insert(info_t(id, pts[edges[id].u], pts[edges[id].v])).first;
				}
			}

			nowx = ques[i].pt.x;
			point_t a = ques[i].pt, b = a;
			b.x += 1;
			iter_t pos = s.lower_bound(info_t(0, a, b));
			if(pos == s.end()) area[ques[i].id] = inf_area;
			else area[ques[i].id] = near[pos->id ^ 1];
		}
	}
}

namespace mst
{
	struct edge_t
	{
		int u, v, w;
		bool operator < (const edge_t& r) const
		{
			return w < r.w;
		}
	} edges[MaxM];

	int gf[MaxN];

	int find(int v)
	{
		if(gf[v] == v) return gf[v] = v;
		return gf[v] = find(gf[v]);
	}

	void read()
	{
		for(int i = 0; i != m; ++i)
		{
			int a = i << 1;
			edges[i].u = planar_graph::near[a];
			edges[i].v = planar_graph::near[a ^ 1];
			edges[i].w = planar_graph::edges[a].len;
		}
	}

	int total;
	int head[MaxN], next[MaxM], point[MaxM], weight[MaxM];
	int que[MaxN], dist[MaxL][MaxN], fa[MaxL][MaxN], depth[MaxN];

	void add_edge(int u, int v, int w)
	{
		weight[++total] = w;
		point[total] = v;
		next[total] = head[u];
		head[u] = total;
	}

	void bfs(int u)
	{
		int qhead = 0, qtail = 0;
		que[qtail++] = u;
		depth[u] = 0;
		while(qhead != qtail)
		{
			int u = que[qhead++];
			for(int i = 1; i != MaxL; ++i)
			{
				fa[i][u] = fa[i - 1][fa[i - 1][u]];
				dist[i][u] = std::max(dist[i - 1][u], dist[i - 1][fa[i - 1][u]]);
			}

			for(int k = head[u]; k; k = next[k])
			{
				int v = point[k];
				if(fa[0][u] != v)
				{
					fa[0][v] = u;
					dist[0][v] = weight[k];
					depth[v] = depth[u] + 1;
					que[qtail++] = v;
				}
			}
		}
	}

	int ask(int u, int v)
	{
		if(u == planar_graph::inf_area)
			return -1;
		if(find(u) != find(v)) return -1;

		int ans = 0;
		if(depth[u] < depth[v]) 
			std::swap(u, v);
		int diff = depth[u] - depth[v];
		for(int l = 0; diff; diff >>= 1, ++l)
		{
			if(diff & 1) 
			{
				ans = std::max(ans, dist[l][u]);
				u = fa[l][u];
			}
		}

		for(int p = MaxL - 1; u != v; p ? --p : 0)
		{
			if(!p || fa[p][u] != fa[p][v])
			{
				ans = std::max(ans, dist[p][u]);
				ans = std::max(ans, dist[p][v]);
				u = fa[p][u]; v = fa[p][v];
			}
		}

		return ans;
	}

	void work()
	{
		read();
		std::sort(edges, edges + m);
		for(int i = 0; i != planar_graph::region_tot; ++i)
			gf[i] = i;
		for(int i = 0; i != m; ++i)
		{
			int u = edges[i].u, v = edges[i].v;
			if(u == planar_graph::inf_area || v == planar_graph::inf_area)
				continue;
			u = find(u), v = find(v);
			if(u != v) 
			{
				gf[u] = v;
				add_edge(edges[i].u, edges[i].v, edges[i].w);
				add_edge(edges[i].v, edges[i].u, edges[i].w);
			}
		}

		std::memset(fa[0], -1, sizeof(fa[0]));
		for(int i = 0; i <= planar_graph::region_tot; ++i)
			if(fa[0][i] == -1) bfs(i);
	}
}

int main()
{
	std::scanf("%d %d", &n, &m);
	planar_graph::work();
	scanning_line::work();
	mst::work();

	using namespace scanning_line;
	for(int i = 0; i != ques_tot; ++i)
	{
		int a = i << 1;
		std::printf("%d\n", mst::ask(area[a], area[a ^ 1]));
	}
	return 0;
}
