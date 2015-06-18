/* Codeforces 396B. On Sum of Fractions
 *   math, number theory */
#include <cstdio>
#include <cmath>

bool check(int n)
{
	int r = sqrt(n + 1.0);
	for(int i = 2; i <= r; ++i)
		if(n % i == 0) return false;
	return true;
}

long long gcd(long long a, long long b)
{
	return b ? gcd(b, a % b) : a;
}

void solve()
{
	int n;
	std::scanf("%d", &n);
	if(n == 2)
	{
		std::puts("1/6");
		return;
	}

	long long v = n, u = n + 1;
	while(!check(v)) --v;
	while(!check(u)) ++u;
	long long a = u * v - 2 * v - 2 * (u - n - 1);
	long long b = 2 * u * v;
	long long g = gcd(a, b);
	std::printf("%I64d/%I64d\n", a / g, b / g);
}

int main()
{
	int t;
	std::scanf("%d", &t);
	while(t --> 0) solve();
	return 0;
}
