/* Codeforces 552D. Vanya and Triangles
 *   brute force, combinatorics, data structures, 
 *   geometry, math, matrices, sortings */
#include <iostream>
#include <cmath>
#include <tuple>
#include <map>

const int MaxN = 2000;
struct point_t
{
	int x, y;
} pt[MaxN];

typedef std::tuple<int, int, int> line_t;

int gcd(int a, int b)
{
	return b ? gcd(b, a % b) : a;
}

int abs(int x) { return x < 0 ? -x : x; }

line_t get_line(const point_t& a, const point_t& b)
{
	// (x - a.x) / (b.x - a.x) = (y - a.y) / (b.y - a.y)
	// (x - a.x) * (b.y - a.y) = (y - a.y) * (b.x - a.x)
	int A = b.y - a.y, B = a.x - b.x, C = -a.x * A - a.y * B;
	int g = abs(gcd(A, gcd(B, C)));
	if(A < 0 || !A && B < 0 || !A && !B && C < 0) g = -g;
	return line_t( A / g, B / g, C / g );
}

std::map<line_t, int> s0;
std::map<int, int> s;

int main()
{
	int n;
	std::cin >> n;
	for(int i = 0; i != n; ++i)
	{
		s[(i + 1) * (i + 2)] = i + 2;
		std::cin >> pt[i].x >> pt[i].y;
		for(int j = 0; j != i; ++j)
			s0[get_line(pt[i], pt[j])] += 2;
	}

	if(n < 3) 
	{
		std::cout << 0 << std::endl;
	} else {
		long long ans = (long long)n * (n - 1) * (n - 2);
		for(auto z : s0)
		{
			long long x = s[z.second];
			ans -= x * (x - 1) * (x - 2);
		}

		std::cout << ans / 6 << std::endl;
	}
	return 0;
}
