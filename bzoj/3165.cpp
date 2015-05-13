/* BZOJ-3165: [Heoi2013]Segment
 *   线段树  */
#include <deque>
#include <cstdio>
#include <algorithm>

const int MaxN = 100010, mod_v = 39989;
const double eps = 1.0e-7;
struct point_t
{
	double x, y;
	point_t(double x = 0, double y = 0.0) : x(x), y(y) {}
	point_t operator - (const point_t& a) const
	{
		return point_t(x - a.x, y - a.y);
	}
};

double cross(const point_t& a, const point_t& b)
{
	return a.x * b.y - a.y * b.x;
}

bool cmp(double a, double b)
{
	return a - b < eps && a - b > -eps;
}

bool in_range(double x, double l, double r)
{
	if(l < r) return l <= x && x <= r;
	return r <= x && x <= l;
}

bool has_intersection;
point_t intersection(const point_t& a, const point_t& b, const point_t& c, const point_t& d)
{
	point_t m = a - b, n = c - d;
	double k1 = m.y / m.x, k2 = n.y / n.x;
	double b1 = a.y - k1 * a.x, b2 = c.y - k2 * c.x;
	if(cmp(k1, k2))
	{
		has_intersection = false;
		return point_t();
	}

	point_t p;
	p.x = (b1 - b2) / (k2 - k1);
	p.y = p.x * k1 + b1;
	if(!in_range(p.x, a.x, b.x) || !in_range(p.x, c.x, d.x)
		|| !in_range(p.y, a.y, b.y) || !in_range(p.y, c.y, d.y))
	{
		has_intersection = false;
		return point_t();
	}

	has_intersection = true;
	return p;
}

point_t cut_line(double x, const point_t& a, const point_t& b)
{
	// (x - x0) / (x1 - x0) = (y - y0) / (y1 - y0)
	return point_t(x, (x - b.x) / (a.x - b.x) * (a.y - b.y) + b.y);
}

int not_empty[mod_v << 2], line_id[mod_v << 2];
point_t line[mod_v << 2][2];

void insert(int now, int head, int tail, const point_t& a, const point_t& b, int now_id)
{
	if(b.x - a.x < -eps) return;
	int m = (head + tail) >> 1;
	int l = now << 1, r = l + 1;
	if(cmp(head, a.x) && cmp(tail, b.x))
	{
		bool set_seg = false;
		if(!not_empty[now])
		{
			set_seg = true;
		} else {
			point_t p = intersection(a, b, line[now][0], line[now][1]);
			if(has_intersection)
			{
				if(a.y > line[now][0].y)
				{
					if(p.x <= m) insert(l, head, m, a, cut_line(m, a, b), now_id);
					else {
						set_seg = true;
						insert(r, m + 1, tail, cut_line(m + 1, line[now][0], line[now][1]), line[now][1], line_id[now]);
					}
				} else {
					if(p.x > m) insert(r, m + 1, tail, cut_line(m + 1, a, b), b, now_id);
					else {
						set_seg = true;
						insert(l, head, m, line[now][0], cut_line(m, line[now][0], line[now][1]), line_id[now]);
					}
				}
			} else if(a.y > line[now][0].y) {
				set_seg = true;
			}
		}

		if(set_seg)
		{
			line[now][0] = a;
			line[now][1] = b;
			line_id[now] = now_id;
			not_empty[now] = 1;
		}
	} else {
		if(b.x <= m) return insert(l, head, m, a, b, now_id);
		if(m < a.x) return insert(r, m + 1, tail, a, b, now_id);
		insert(l, head, m, a, cut_line(m, a, b), now_id);
		insert(r, m + 1, tail, cut_line(m + 1, a, b), b, now_id);
	}
}

int now_ans;
double now_val;

void ask(int now, int head, int tail, int pos)
{
	if(not_empty[now])
	{
		double y = line[now][0].y;
		if(head != tail) y = cut_line(pos, line[now][0], line[now][1]).y;
		if(cmp(y, now_val))
		{
			if(now_ans > line_id[now])
				now_ans = line_id[now];
		} else if(y > now_val) {
			now_val = y;
			now_ans = line_id[now];
		}
	}

	if(head == tail) return;
	int m = (head + tail) >> 1;
	if(pos <= m) ask(now << 1, head, m, pos);
	else ask((now << 1) + 1, m + 1, tail, pos);
}

int main()
{
	int n;
	std::scanf("%d", &n);
	int lastans = 0, count = 0;
	for(int i = 0; i != n; ++i)
	{
		int op;
		std::scanf("%d", &op);
		if(op == 0)
		{
			now_ans = 0;
			now_val = -1.0e100;
			int pos;
			std::scanf("%d", &pos);
			pos = (pos + lastans - 1) % mod_v + 1;
			ask(1, 1, mod_v, pos);
			std::printf("%d\n", lastans = now_ans);
		} else {
			int x1, x2, y1, y2;
			std::scanf("%d %d %d %d", &x1, &y1, &x2, &y2);
			x1 = (x1 + lastans - 1) % mod_v + 1;
			x2 = (x2 + lastans - 1) % mod_v + 1;
			y1 = (y1 + lastans - 1) % 1000000000 + 1;
			y2 = (y2 + lastans - 1) % 1000000000 + 1;
			if(x1 > x2 || x1 == x2 && y1 > y2)
				std::swap(x1, x2), std::swap(y1, y2);
			insert(1, 1, mod_v, point_t(x1, y1), point_t(x2, y2), ++count);
		}
	}
	return 0;
}
