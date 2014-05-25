/*
 * BZOJ-2564 集合的面积
 *  计算几何-凸包
 */
#include <algorithm>
#include <cstdio>

struct point
{
	long long x, y;
	friend bool operator < (const point& a, const point& b)
	{
		if(a.x == b.x)
			return a.y < b.y;
		return a.x < b.x;
	}

	friend point operator - (const point& a, const point& b)
	{
		point p;
		p.x = a.x - b.x;
		p.y = a.y - b.y;
		return p;
	}
};

inline long long cross(const point& a, const point& b)
{
	return a.x * b.y - a.y * b.x;
}

inline bool bool_cross(const point& a, const point& b)
{
	return cross(a, b) > 0;
}

inline point* convex_hull(point* pt, point* end, point* convex)
{
	int len = end - pt;
	std::sort(pt, end);
	if(len <= 3)
	{
		for(int i = 0; i != len; ++i)
			convex[i] = *pt;
		return convex + len;
	}

	int st = 0;
	point* tpt = pt;
	while(pt != end)
	{
		while(st > 1 && cross(*pt - convex[st - 1],
			convex[st - 1] - convex[st - 2]) >= 0)
				--st;
		convex[st++] = *pt++;
	}

	st--;
	int st2 = 1;
	while(end != tpt)
	{
		--end;
		while(st2 > 1 && cross(*end - convex[st2 + st - 1], 
			convex[st2 + st - 1] - convex[st2 + st - 2]) >= 0)
				--st2;
		convex[st + st2++] = *end;

	}
	
	return convex + st + st2 - 1;
}

point pt[100000];
point pt2[100000];
point convex[100001];
point convex2[100001];
point convex3[200002];
int main()
{
	int N, M;
	std::scanf("%d %d", &N, &M);
	for(int i = 0; i != N; ++i)
		std::scanf("%lld %lld", &pt[i].x, &pt[i].y);
	for(int i = 0; i != M; ++i)
		std::scanf("%lld %lld", &pt2[i].x, &pt2[i].y);
	point* convex_end = convex_hull(pt, pt + N, convex);
	point* convex2_end = convex_hull(pt2, pt2 + M, convex2);
	*convex_end = convex[0];
	*convex2_end = convex2[0];
	for(point* beg = convex; beg != convex_end; ++beg)
		*beg = beg[0] - beg[1];
	for(point* beg = convex2; beg != convex2_end; ++beg)
		*beg = beg[0] - beg[1];
	point* convex3_end = 
	//	std::unique
	//	(
	//		convex3, 
			std::merge(convex, convex_end, 
					   convex2, convex2_end,
					   convex3, bool_cross
					  );//, 
	//		same_line
	//	);
	long long area = 0;
	for(point* beg = convex3; beg != convex3_end; ++beg)
	{
		beg[1].x += beg->x;
		beg[1].y += beg->y;
		area += cross(*beg, beg[1]);
	}
	std::printf("%lld", area);
	return 0;
}
