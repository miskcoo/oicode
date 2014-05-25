/*
 * BZOJ-2961 共点圆
 *  CDQ分治+凸包
 */
#include <cstdio>
#include <algorithm>

const double eps = 1.0e-8;
inline double fabs(double v)
{
	return v < 0 ? -v : v;
}

struct point
{
	double x, y;

	point(double x_ = 0, double y_ = 0)
		: x(x_), y(y_) {}

	bool operator < (const point& v) const
	{
		if(fabs(x - v.x) < eps)
			return y < v.y;
		return x < v.x;
	}

	point operator - (const point& v) const
	{
		return point(x - v.x, y - v.y);
	}
};

inline double cross(const point& a, const point& b)
{
	return a.x * b.y - a.y * b.x;
}

point pt[500000], temp[250000], convex[250000];
bool answer[500000];
bool updated[500000];
int oper[500000];
int N;

inline double sqr(double v)
{
	return v * v;
}

inline void update_ans(int i, point r)
{
	double v = sqr(pt[i].x) + sqr(pt[i].y)
		- 2 * (pt[i].x * r.x + pt[i].y * r.y);
	if(v > eps) answer[i] = false;
}

void solve(int l, int r)
{
	if(l == r)
		return;
	int middle = (l + r) >> 1;

	solve(l, middle);
	solve(middle + 1, r);

	int total = 0;
	for(int i = l; i <= middle; ++i)
	{
		if(oper[i] == 0)
			temp[total++] = pt[i];
	}

	if(total == 0)
		return;

	std::sort(temp, temp + total);
	int stack_top = 0;
	for(int i = 0; i != total; ++i)
	{
		while(stack_top > 1 && cross(
			temp[i] - convex[stack_top - 1],
			convex[stack_top - 1]
			- convex[stack_top - 2]) > -eps)
				--stack_top;
		convex[stack_top++] = temp[i];
	}

	convex[stack_top].x = convex[stack_top - 1].x + eps;
	convex[stack_top].y = 1.0e50;
	for(int i = middle + 1; i <= r; ++i)
	{
		if(!answer[i] || !oper[i])
			continue;

		point p(1, -pt[i].x / pt[i].y);
		int l = 0, r = stack_top - 1;
		while(l < r - 1)
		{
			int m = (l + r) >> 1;
			if(cross(p, convex[m + 1] - convex[m]) < eps)
				l = m;
			else r = m;
		}

		l = r - 1;
		if(l < 0) l = 0;
		if(r + 1 < stack_top)
			r = r + 1;
		for(int h = l; h <= r; ++h)
			update_ans(i, convex[h]);
		updated[i] = true;
	}
}

int main()
{
	std::scanf("%d", &N);
	for(int i = 0; i != N; ++i)
	{
		updated[i] = false;
		answer[i] = true;
		std::scanf("%d %lf %lf", 
			oper + i, &pt[i].x, &pt[i].y);
	}
	solve(0, N - 1);
	for(int i = 0; i != N; ++i)
	{
		if(oper[i] == 0)
			continue;
		if(answer[i] && updated[i])
			std::puts("Yes");
		else std::puts("No");
	}
	return 0;
}
