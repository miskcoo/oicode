/*
 * BZOJ-1041 [HAOI2008]圆上的整点
 *  分解因式
 */
#include <cstdio>
#include <cmath>

long long gcd(long long a, long long b)
{
	return b ? gcd(b, a % b) : a;
}

int main()
{
	long long r;
	std::scanf("%lld", &r);
	r <<= 1;
	int sr = (int)std::sqrt(double(r));
	long long count = 0;
	for(int i = 1; i <= sr; ++i)
	{
		if(r % i) continue;
		long long d = r / i;
		int sd = (int)std::sqrt(double(d));
		for(long long u = 1; u <= sd; ++u)
		{
			long long v = (int)std::sqrt(double(d - u * u));
			if(v < u) break;
			if(v * v + u * u == d && gcd(u, v) == 1)
				++count;
		}

		if((long long)i * i == r)
			continue;
		d = i;
		sd = (int)std::sqrt(double(d));
		for(long long u = 1; u <= sd; ++u)
		{
			long long v = (int)std::sqrt(double(d - u * u));
			if(v < u) break;
			if(v * v + u * u == d && gcd(u, v) == 1)
				++count;
		}
	}

	count <<= 2;
	std::printf("%lld", count);
	return 0;
}
