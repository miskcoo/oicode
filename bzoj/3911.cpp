/* BZOJ-3911: SGU383 Caravans
 *   Delaunay三角剖分+MST+倍增 */
#include <cstdio>
#include <stack>
#include <cmath>
#include <algorithm>

const double eps = 1.0e-8;
double cmp(double a, double b)
{
	return a - b < eps && a - b > -eps;
}

const int MaxL = 18, MaxN = 100010, MaxM = MaxN * 8;
struct point_t
{
	int id;
	double x, y;
	point_t(double x = 0, double y = 0) : x(x), y(y) {}
	bool operator < (const point_t& a) const
	{
		if(cmp(x, a.x)) return y < a.y;
		return x < a.x;
	}

	point_t operator - (const point_t& a) const
	{
		return point_t(x - a.x, y - a.y);
	}

	point_t operator + (const point_t& a) const
	{
		return point_t(x + a.x, y + a.y);
	}
} pt[MaxN];
int ptmap[MaxN];

double cross(const point_t& a, const point_t& b)
{
	return a.x * b.y - a.y * b.x;
}

double cross(const point_t& p, const point_t& a, const point_t& b)
{
	return cross(a - p, b - p);
}

double dot(const point_t& a, const point_t& b)
{
	return a.x * b.x + a.y * b.y;
}

double dist2(const point_t& a, const point_t& b)
{
	return dot(a - b, a - b);
}

double dist(const point_t& a, const point_t& b)
{
	return sqrt(dist2(a, b));
}

bool check_intersection(
	const point_t& a, const point_t& b, 
	const point_t& c, const point_t& d)
{
	return cross(a, c, b) * cross(a, d, b) < -eps
		&& cross(c, a, d) * cross(c, b, d) < -eps;
}

point_t middle_point(const point_t& a, const point_t& b)
{
	point_t p = a + b;
	return point_t(p.x * 0.5, p.y * 0.5);
}

point_t circumcenter(const point_t& a, const point_t& b, const point_t& c)
{
	point_t p1 = b - a, p2 = c - a;
	double c1 = dot(p1, (b + a)) * 0.5;
	double c2 = dot(p2, (c + a)) * 0.5;
	double det = cross(p1, p2);
	if(cmp(det, 0)) return point_t();
	return point_t( 
		(c1 * p2.y - c2 * p1.y) / det, 
		(p1.x * c2 - p2.x * c1) / det);
}

struct graph_t
{
	int total;
	int head[MaxM], prev[MaxM], next[MaxM], point[MaxM];
	std::stack<int> garbages;
public:
	graph_t() : total(1) {}
	void add(int u, int v)
	{
		int n;
		if(garbages.empty()) n = ++total;
		else n = garbages.top(), garbages.pop();
		point[n] = v;
		if(head[u]) prev[head[u]] = n;
		prev[n] = 0;
		next[n] = head[u];
		head[u] = n;
	}

	void remove(int k)
	{
		if(head[point[k ^ 1]] == k)
			head[point[k ^ 1]] = next[k];
		if(next[k]) prev[next[k]] = prev[k];
		if(prev[k]) next[prev[k]] = next[k];
		garbages.push(k);
	}
} gp;

int st[MaxN];
void delaunay(int l, int r)
{
	if(r - l <= 3)
	{
		for(int i = l; i != r; ++i)
		{
			for(int j = i + 1; j != r; ++j)
			{
				gp.add(i, j);
				gp.add(j, i);
			}
		}
		
		return;
	} 

	// divide 
	int m = (l + r) >> 1;
	delaunay(l, m);
	delaunay(m, r);

	// convex hull
	int qh = 0, qt = 0;
	for(int i = l; i != r; ++i)
	{
		while(qt - qh >= 2 && cross(pt[st[qt - 2]], pt[st[qt - 1]], pt[i]) < 0) --qt;
		st[qt++] = i;
	}

	int left = 0, right = 0;
	for(int i = qh; i + 1 < qt && !left; ++i)
		if(st[i] < m && st[i + 1] >= m) 
			left = st[i], right = st[i + 1];

	// merge 
	gp.add(left, right);
	gp.add(right, left);
	for(;;)
	{
		int p3 = 0, pos = 0;
		double r2;
		point_t c;
		for(int k = gp.head[left]; k; k = gp.next[k])
		{
			int p = gp.point[k];
			if(cross(pt[left], pt[right], pt[p]) > 0 && (!p3 || dist2(c, pt[p]) < r2))
			{
				p3 = p, pos = 1;
				c = circumcenter(pt[left], pt[right], pt[p3]);
				r2 = dist2(c, pt[p3]);
			}
		}

		for(int k = gp.head[right]; k; k = gp.next[k])
		{
			int p = gp.point[k];
			if(cross(pt[right], pt[left], pt[p]) < 0 && (!p3 || dist2(c, pt[p]) < r2))
			{
				p3 = p, pos = 2;
				c = circumcenter(pt[left], pt[right], pt[p3]);
				r2 = dist2(c, pt[p3]);
			}
		}

		if(!p3) break;
		if(pos == 2)
		{
			for(int k = gp.head[right]; k; k = gp.next[k])
			{
				int p = gp.point[k];
				if(p != p3 && check_intersection(pt[right], pt[p], pt[left], pt[p3]))
					gp.remove(k), gp.remove(k ^ 1);
			}
		} else {
			for(int k = gp.head[left]; k; k = gp.next[k])
			{
				int p = gp.point[k];
				if(p != p3 && check_intersection(pt[left], pt[p], pt[right], pt[p3]))
					gp.remove(k), gp.remove(k ^ 1);
			}
		}

		if(pos == 1) left = p3;
		else right = p3;
		gp.add(left, right);
		gp.add(right, left);
	}
}

struct edge_t
{
	int u, v;
	double w;
	bool operator < (const edge_t& r) const
	{
		return w < r.w;
	}
} e[MaxM];

struct mset
{
	int fa[MaxN];
	int find(int u)
	{
		if(!fa[u] || fa[u] == u)
			return fa[u] = u;
		return fa[u] = find(fa[u]);
	}
} ms;

struct tree_t
{
	int total;
	int head[MaxN], point[MaxM], next[MaxM];
	double weight[MaxM], val[MaxL][MaxN];
	int fa[MaxL][MaxN], depth[MaxN];

	void add_edge(int u, int v, double w)
	{
		point[++total] = v;
		weight[total] = w;
		next[total] = head[u];
		head[u] = total;
	}

	void dfs(int u)
	{
		for(int i = 1; i != MaxL; ++i)
		{
			fa[i][u] = fa[i - 1][fa[i - 1][u]];
			val[i][u] = std::max(val[i - 1][u], val[i - 1][fa[i - 1][u]]);
		}

		for(int k = head[u]; k; k = next[k])
		{
			int v = point[k];
			if(v != fa[0][u])
			{
				fa[0][v] = u;
				val[0][v] = weight[k];
				depth[v] = depth[u] + 1;
				dfs(v);
			}
		}
	}

	double calc(int u, int v)
	{
		if(depth[u] < depth[v]) 
			std::swap(u, v);
		int diff = depth[u] - depth[v];
		double ans = 0.0;
		for(int i = 0; diff; diff >>= 1, ++i)
		{
			if(diff & 1)
			{
				ans = std::max(ans, val[i][u]);
				u = fa[i][u];
			}
		}

		for(int p = MaxL - 1; u != v; p ? --p : 0)
		{
			if(!p || fa[p][u] != fa[p][v])
			{
				ans = std::max(ans, val[p][u]);
				ans = std::max(ans, val[p][v]);
				u = fa[p][u];
				v = fa[p][v];
			}
		}

		return ans;
	}
} tree;

int main()
{
	int n;
	std::scanf("%d", &n);
	for(int i = 1; i <= n; ++i)
	{
		pt[i].id = i;
		std::scanf("%lf %lf", &pt[i].x, &pt[i].y);
	}
	std::sort(pt + 1, pt + n + 1);
	for(int i = 1; i <= n; ++i)
		ptmap[pt[i].id] = i;

	delaunay(1, n + 1);
	int tot = 0;
	for(int i = 1; i <= n; ++i)
	{
		for(int k = gp.head[i]; k; k = gp.next[k])
		{
			int v = gp.point[k];
			if(v < i)
			{
				e[tot].u = i;
				e[tot].v = v;
				e[tot++].w = dist(pt[i], pt[v]);
			}
		}
	}

	std::sort(e, e + tot);
	for(int i = 0; i != tot; ++i)
	{
		int u = ms.find(e[i].u), v = ms.find(e[i].v);
		if(u != v)
		{
			tree.add_edge(e[i].u, e[i].v, e[i].w);
			tree.add_edge(e[i].v, e[i].u, e[i].w);
			ms.fa[u] = v;
		}
	}

	tree.dfs(1);

	int q;
	std::scanf("%d", &q);
	for(int i = 0; i != q; ++i)
	{
		int a, b;
		std::scanf("%d %d", &a, &b);
		std::printf("%.6lf\n", tree.calc(ptmap[a], ptmap[b]));
	}
	return 0;
}
