/* BZOJ-1845: [Cqoi2005] 三角形面积并 
 *   扫描线 */
#include <cstdio>
#include <cmath>
#include <algorithm>

const int MaxN = 110;
const long double eps = 1.0e-7;
struct point_t
{
	long double x, y;
	point_t operator - (const point_t& r) const
	{
		point_t z;
		z.x = x - r.x;
		z.y = y - r.y;
		return z;
	}
} pt[MaxN][3];
long double coordinate[MaxN * MaxN * 20];

struct data_t
{
	int cnt;
	long double x;
	bool operator < (const data_t& r) const
	{
		return x < r.x;
	}
} data[MaxN * 20];
int n, tot, num;
long double rotate_sin, rotate_cos;

bool cmp(long double a, long double b)
{
	return a - b < eps && a - b > -eps;
}

void add(long double x1, long double x2)
{
	if(x1 > x2) std::swap(x1, x2);
	data[num].cnt = 1;
	data[num++].x = x1;
	data[num].cnt = -1;
	data[num++].x = x2;
}

bool add_horizontal(long double y, int j, int a, int b)
{
	if(cmp(pt[j][a].y, pt[j][b].y) && cmp(pt[j][a].y, y))
	{
		add(pt[j][a].x, pt[j][b].x);
		return true;
	}

	return false;
}

void add_segment(long double a, long double b)
{
	if(a > 1.0e90 || b > 1.0e90) return;
	if(cmp(a, b)) return;
	add(a, b);
}

long double get_intersection(long double y, point_t a, point_t b)
{
	if(a.y > b.y) std::swap(a, b);
	if(a.y > y || b.y < y) return 1.0e100;
	// (y - y0) / (y1 - y0) = (x - x0) / (x1 - x0)
	return (y - a.y) / (b.y - a.y) * (b.x - a.x) + a.x;
}

bool in_range(long double v, long double l, long double r)
{
	if(l > r) std::swap(l, r);
	return v >= l && v <= r;
}

long double get_intersection(point_t a, point_t b, point_t c, point_t d)
{
	if(cmp(a.x, b.x) || cmp(c.x, d.x)) return 1.0e100;
	point_t x = a - b, y = c - d;
	if(cmp(x.x * y.y, x.y * y.x)) return 1.0e100;
	long double k1 = x.y / x.x, k2 = y.y / y.x;
	long double b1 = a.y - k1 * a.x, b2 = c.y - k2 * c.x;
	long double px = (b2 - b1) / (k1 - k2);
	long double py = k1 * px + b1;
	if(!in_range(px, a.x, b.x) || !in_range(px, c.x, d.x)) return 1.0e100;
	if(!in_range(py, a.y, b.y) || !in_range(py, c.y, d.y)) return 1.0e100;
	return py;
}

void add_intersection(int i, int ai, int bi, int j, int aj, int bj)
{
	long double y = get_intersection(pt[i][ai], pt[i][bi], pt[j][aj], pt[j][bj]);
	if(y < 1.0e90) coordinate[tot++] = y;
}

point_t rotate(const point_t& p)
{
	point_t z;
	z.x = p.x * rotate_cos - p.y * rotate_sin;
	z.y = p.x * rotate_sin + p.y * rotate_cos;
	return z;
}

int main()
{
	std::scanf("%d", &n);
	rotate_sin = sinl(233.3);
	rotate_cos = cosl(233.3);
	for(int i = 0; i != n; ++i)
	{
		for(int j = 0; j != 3; ++j)
		{
			std::scanf("%Lf %Lf", &pt[i][j].x, &pt[i][j].y);
			pt[i][j] = rotate(pt[i][j]);
			coordinate[tot++] = pt[i][j].y;
		}

		for(int j = 0; j != i; ++j)
		{
			add_intersection(i, 0, 1, j, 0, 1);
			add_intersection(i, 0, 1, j, 0, 2);
			add_intersection(i, 0, 1, j, 1, 2);
			add_intersection(i, 0, 2, j, 0, 1);
			add_intersection(i, 0, 2, j, 0, 2);
			add_intersection(i, 0, 2, j, 1, 2);
			add_intersection(i, 1, 2, j, 0, 1);
			add_intersection(i, 1, 2, j, 0, 2);
			add_intersection(i, 1, 2, j, 1, 2);
		}
	}

	std::sort(coordinate, coordinate + tot);
	tot = std::unique(coordinate, coordinate + tot, cmp) - coordinate;
	long double area = 0.0, prev_len = 0.0;
	for(int i = 0; i != tot; ++i, num = 0)
	{
		long double y = coordinate[i];
		for(int j = 0; j != n; ++j)
		{
			if(add_horizontal(y, j, 0, 1)) continue;
			if(add_horizontal(y, j, 0, 2)) continue;
			if(add_horizontal(y, j, 1, 2)) continue;
			long double d1 = get_intersection(y, pt[j][0], pt[j][1]);
			long double d2 = get_intersection(y, pt[j][0], pt[j][2]);
			long double d3 = get_intersection(y, pt[j][1], pt[j][2]);
			add_segment(d2, d3);
			add_segment(d1, d3);
			add_segment(d1, d2);
		}

		std::sort(data, data + num);
		int cnt = 0;
		long double prev = 0.0, len = 0.0;
		for(int j = 0; j != num; ++j)
		{
			if(!cnt) prev = data[j].x;
			cnt += data[j].cnt;
			if(!cnt) len += data[j].x - prev;
		}

		if(i) area += (len + prev_len) * (y - coordinate[i - 1]) * (long double)0.5;
		prev_len = len;
	}

	std::printf("%.2Lf\n", area);
	return 0;
}
