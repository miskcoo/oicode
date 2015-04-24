/* BZOJ-3007: 拯救小云公主
 *   二分+对偶图 */
#include <cstdio>
#include <cstring>
#include <algorithm>

const int MaxN = 3010;
int n, width, height;
struct point_t
{
	double x, y;
} pt[MaxN];

struct graph_t
{
	int total;
	int head[MaxN], point[MaxN * MaxN], next[MaxN * MaxN];

	void reset()
	{
		total = 0;
		std::memset(head, 0, sizeof(head));
	}

	void add_edge(int u, int v)
	{
		point[++total] = v;
		next[total] = head[u];
		head[u] = total;
	}
} gp;

double dist(const point_t& a, const point_t& b)
{
	return (a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y);
}

int mark_v;
int que[MaxN * MaxN], mark[MaxN];
bool check(double r)
{
	gp.reset();
	int s = n + 1, t = n + 2;
	double d = 4.0 * r * r;
	for(int i = 1; i <= n; ++i)
	{
		if(pt[i].x < r + 1.0 || pt[i].y + r > height)
			gp.add_edge(s, i);
		if(pt[i].x + r > width || pt[i].y < r + 1.0)
			gp.add_edge(i, t);
		for(int j = i + 1; j <= n; ++j)
		{
			if(dist(pt[i], pt[j]) < d)
			{
				gp.add_edge(i, j);
				gp.add_edge(j, i);
			}
		}
	}

	int qhead = 0, qtail = 0;
	que[qtail++] = s;
	mark[s] = ++mark_v;
	while(qhead != qtail)
	{
		int u = que[qhead++];
		for(int k = gp.head[u]; k; k = gp.next[k])
		{
			int v = gp.point[k];
			if(v == t) return false;
			if(mark[v] == mark_v) continue;
			que[qtail++] = v;
			mark[v] = mark_v;
		}
	}

	return true;
}

int main()
{
	std::scanf("%d %d %d", &n, &width, &height);
	for(int i = 1; i <= n; ++i)
		std::scanf("%lf %lf", &pt[i].x, &pt[i].y);

	double l = 0.0, r = std::min(height, width) * 2;
	while(r - l > 1.0e-5)
	{
		double m = (l + r) * 0.5;
		if(check(m)) l = m;
		else r = m;
	}

	std::printf("%.2lf", (l + r) * 0.5);
	return 0;
}
