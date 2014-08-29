/* POJ-1410 Intersection
 *  判断线段和矩形相交 */
#include <cstdio>
#include <algorithm>

struct point_t
{
	int x, y;
	point_t(int x = 0, int y = 0)
		: x(x), y(y) {}

	friend point_t operator - (const point_t& a, const point_t& b)
	{
		return point_t(a.x - b.x, a.y - b.y);
	}

	friend int operator * (const point_t& a, const point_t& b)
	{
		return a.x * b.y - a.y * b.x;
	}
};

bool segment_banana(
	const point_t& a,
	const point_t& b,
	const point_t& c,
	const point_t& d)
{
	return ((c - a) * (b - a)) * ((d - a) * (b - a)) <= 0
		&& ((a - c) * (d - c)) * ((b - c) * (d - c)) <= 0
		&& std::min(a.x, b.x) <= std::max(c.x, d.x)
		&& std::min(a.y, b.y) <= std::max(c.y, d.y)
		&& std::max(a.x, b.x) >= std::min(c.x, d.x)
		&& std::max(a.y, b.y) >= std::min(c.y, d.y);
}

bool point_in_rect(const point_t& pt, int x1, int y1, int x2, int y2)
{
	return pt.x >= x1 && pt.x <= x2 && pt.y >= y1 && pt.y <= y2;
}

bool check()
{
	int sx, sy, ex, ey, x1, y1, x2, y2;
	std::scanf("%d %d %d %d", &sx, &sy, &ex, &ey);
	std::scanf("%d %d %d %d", &x1, &y1, &x2, &y2);

	point_t p1(sx, sy), p2(ex, ey);
	if(segment_banana(p1, p2, point_t(x1, y1), point_t(x2, y1)))
		return true;
	if(segment_banana(p1, p2, point_t(x1, y2), point_t(x2, y2)))
		return true;
	if(segment_banana(p1, p2, point_t(x1, y1), point_t(x1, y2)))
		return true;
	if(segment_banana(p1, p2, point_t(x2, y1), point_t(x2, y2)))
		return true;

	if(x1 > x2) std::swap(x1, x2);
	if(y1 > y2) std::swap(y1, y2);
	return point_in_rect(p1, x1, y1, x2, y2) 
		|| point_in_rect(p2, x1, y1, x2, y2);
}

int main()
{
	int T;
	std::scanf("%d", &T);
	while(T --> 0)
		std::puts(check() ? "T" : "F");
	return 0;
}
