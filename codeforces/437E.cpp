/*
 * Codeforces 437E. The Child and Polygon
 */
#include <algorithm>
#include <cstdio>

struct point_t
{
	long long x, y;

	point_t(long long x = 0, long long y = 0)
		: x(x), y(y) {}

	friend point_t operator - (const point_t& a, const point_t& b)
	{
		return { a.x - b.x, a.y - b.y };
	}

	friend long long operator * (const point_t& a, const point_t& b)
	{
		return a.x * b.y - a.y * b.x;
	}
};

const long long mod_const = 1000000007LL;
point_t pt[201];
long long f[200][200];
char mark[200][200];

long long solve(int l, int r)
{
	if(mark[l][r]) return f[l][r];
	mark[l][r] = 1;
	if(l + 1 == r) 
		return f[l][r] = 1;

	long long& v = f[l][r];
	for(int i = l + 1; i < r; ++i)
	{
		if((pt[r] - pt[l]) * (pt[r] - pt[i]) > 0)
			v = (v + solve(l, i) * solve(i, r)) % mod_const;
	}

	return v;
}

int main()
{
	int n;
	std::scanf("%d", &n);
	for(int i = 0; i != n; ++i)
		std::scanf("%I64d %I64d", &pt[i].x, &pt[i].y);
	long long area = 0;
	pt[n] = pt[0];
	for(int i = 0; i < n; ++i)
		area += pt[i] * pt[i + 1];
	if(area < 0) std::reverse(pt + 1, pt + n);
	std::printf("%I64d", solve(0, n - 1));
	return 0;
}
