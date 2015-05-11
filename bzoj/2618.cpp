/* BZOJ-2618: [Cqoi2006]凸多边形
 *   扫描线求面积交 */
#include <cstdio>
#include <cmath>
#include <algorithm>

const int MaxN = 105, MaxM = 51;
const double eps = 1.0e-7;
struct point_t
{
	double x, y;
	point_t(double x = 0, double y = 0) : x(x), y(y) {}
	point_t operator - (const point_t& r) const
	{
		return point_t(x - r.x, y - r.y);
	}
} pt[MaxN][MaxM];
double rotate_cos = cos(233.333), rotate_sin = sin(233.333);
double coord[MaxN * MaxM * MaxM];
int ptnum[MaxN];

bool cmp(double a, double b)
{
	return a - b < eps && a - b > -eps;
}

struct data_t
{
	int cnt;
	double y;
	bool operator < (const data_t& z) const
	{
		if(cmp(y, z.y)) return cnt > z.cnt;
		return y < z.y;
	}
} data[MaxN * MaxM * 4];

bool in_range(double x, double l, double r)
{
	if(l < r) return x >= l && x <= r;
	return x >= r && x <= l;
}

double intersection(const point_t& a, const point_t& b, const point_t& c, const point_t& d)
{
	point_t m = a - b, n = c - d;
	double k1 = m.y / m.x, k2 = n.y / n.x;
	double b1 = a.y - k1 * a.x;
	double b2 = c.y - k2 * c.x;
	if(cmp(k1, k2)) return 1.0e100;
	double px = (b1 - b2) / (k2 - k1);
	double py = k1 * px + b1;
	if(!in_range(px, a.x, b.x) || !in_range(px, c.x, d.x)) return 1.0e100;
	if(!in_range(py, a.y, b.y) || !in_range(py, c.y, d.y)) return 1.0e100;
	return px;
}

double intersection(double x, const point_t& a, const point_t& b)
{
	if(!in_range(x, a.x, b.x)) return 1.0e100;
	// (y - y0) / (y1 - y0) = (x - x0) / (x1 - x0)
	double y = (x - a.x) / (b.x - a.x) * (b.y - a.y) + a.y;
	return y;
}

point_t rotate(const point_t& p)
{
	point_t r;
	r.x = p.x * rotate_cos - p.y * rotate_sin;
	r.y = p.x * rotate_sin + p.y * rotate_cos;
	return r;
}

int main()
{
	int n, tot = 0;
	std::scanf("%d", &n);
	for(int i = 0; i != n; ++i)
	{
		std::scanf("%d", ptnum + i);
		for(int j = 0; j != ptnum[i]; ++j)
		{
			std::scanf("%lf %lf", &pt[i][j].x, &pt[i][j].y);
			pt[i][j] = rotate(pt[i][j]);
			coord[tot++] = pt[i][j].x;
		}

		for(int j = 0; j != i; ++j)
		{
			for(int pi = 0; pi != ptnum[i]; ++pi)
			{
				for(int pj = 0; pj != ptnum[j]; ++pj)
				{
					double z = intersection(
						pt[i][pi], pt[i][(pi + 1) % ptnum[i]], 
						pt[j][pj], pt[j][(pj + 1) % ptnum[j]]);
					if(z < 1.0e90) coord[tot++] = z;
				}
			}
		}
	}

	std::sort(coord, coord + tot);
	tot = std::unique(coord, coord + tot, cmp) - coord;

	double prev = -1.0, area = 0.0;
	for(int i = 0; i != tot; ++i)
	{
		int num = 0;
		double x = coord[i];
		for(int j = 0; j != n; ++j)
		{
			double lm = 1.0e100, rm = -lm;
			for(int k = 0; k != ptnum[j]; ++k)
			{
				double z = intersection(x, pt[j][k], pt[j][(k + 1) % ptnum[j]]);
				if(z < 1.0e90) 
				{
					if(z < lm) lm = z;
					if(z > rm) rm = z;
				}
			}

			if(lm < 1.0e90)
			{
				data[num].cnt = 1;
				data[num++].y = lm;
				data[num].cnt = -1;
				data[num++].y = rm;
			}
		}

		std::sort(data, data + num);
		int cnt = 0;
		double len = 0.0, prev_len = 0.0;
		for(int j = 0; j != num; ++j)
		{
			if(cnt == n) len += data[j].y - prev_len, prev_len = data[j].y;
			cnt += data[j].cnt;
			if(cnt == n) prev_len = data[j].y;
		}

		if(prev > -eps)
			area += 0.5 * (prev + len) * (coord[i] - coord[i - 1]);
		prev = len;
	}

	std::printf("%.3lf\n", area);
	return 0;
}

