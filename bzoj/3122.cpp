/*
 * BZOJ-3122 随机数生成器
 *  扩展欧几里得+大步小步法
 */
#include <iostream>
#include <cmath>
#include <map>

template<typename T>
T exgcd(T a, T b, T& x, T& y)
{
	if(b == 0)
	{
		x = 1;
		y = 0;
		return a;
	}

	T r = exgcd(b, a % b, x, y);
	T t = x;
	x = y;
	y = t - a / b * y;
	return r;
}

template<typename T>
T power(T a, T p, T c)
{
	T r = 1;
	while(p)
	{
		if(p & 1) r = r * a % c;
		a = a * a % c;
		p >>= 1;
	}
	return r;
}

template<typename T>
T bsgs(T a, T b, T c)
{
	std::map<T, T> hash;
	T m = (T)std::sqrt(double(c));
	T gs = 1;
	for(int i = 0; i <= m; ++i)
	{
		T x, y;
		exgcd(gs, c, x, y);
		x %= c;
		if(x < 0) x += c;
		T r = b * x % c;
		if(hash.count(r) == 0)
			hash[r] = i;
		gs = gs * a % c;
	}

	T base = power(a, m, c);
	T bs = 1;
	for(int i = 0; i <= m; ++i)
	{
		if(hash.count(bs))
			return m * i + hash[bs];
		bs = bs * base % c;
	}
	return -2;
}

int main()
{
	int N;
	std::cin >> N;
	while(N--)
	{
		long long p, a, b, x, t;
		std::cin >> p >> a >> b >> x >> t;
		if(x % p == t % p)
		{
			std::cout << 1 << std::endl;
			continue;
		}
		if(a == 1)
		{
			if(b == 0)
			{
				std::cout << (x == t ? 1 : -1) << std::endl;
			} else {
				t = (t - x) % p;
				if(t < 0) t += p;
				long long x, y;
				exgcd(t, p, x, y);
				b = b * x % p;
				if(b < 0) b += p;
				if(t == 0)
				{
					std::cout << 1 << std::endl;
				} else {
					exgcd(b, p, x, y);
					x = x % p;
					if(x < 0) x += p;
					std::cout << x + 1 << std::endl;
				}
			}
		} else if(a == 0) {
			if(t % p == b % p)
				std::cout << 2 << std::endl;
			else std::cout << -1 << std::endl;
		} else {
			t = (t * (a - 1) + b) % p;
			long long r = ((a - 1) * x + b) % p;
			long long x, y;
			exgcd(r, p, x, y);
			t = t * x % p;
			if(t < 0) t += p;
			std::cout << bsgs(a, t, p) + 1 << std::endl;
		}
	}
	return 0;
}
