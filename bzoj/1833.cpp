/* BZOJ-1833: [ZJOI2010]count 数字计数
 *  数位DP */
#include <cstdio>
#include <algorithm>

typedef long long value_t;
struct data_t
{
	value_t size, v[10];
	data_t() : size(0) { for(int i = 0; i != 10; v[i++] = 0); }
	data_t& operator += (const data_t& r) 
	{
		size += r.size;
		for(int i = 0; i != 10; ++i)
			v[i] += r.v[i];
		return *this;
	}
} dp[20][2];
int mark_v, mark[20][2];
value_t digit[20];

int conv(value_t x, value_t *f)
{
	int len = 0;
	while(x)
	{
		f[len++] = x % 10;
		x /= 10;
	}

	return len;
}

data_t solve(int now, int limit)
{
	if(now == 0)
	{
		data_t d;
		for(int i = 0; i <= (limit ? digit[0] : 9); ++i)
			d.v[i] = 1;
		d.size = limit ? digit[0] + 1 : 10;
		return d;
	}

	if(mark[now][limit] == mark_v)
		return dp[now][limit];
	data_t d, z;

	if(limit)
	{
		for(int i = 0; i != digit[now]; ++i)
		{
			z = solve(now - 1, 0);
			d += z, d.v[i] += z.size;
		}

		z = solve(now - 1, 1);
		d += z, d.v[digit[now]] += z.size;
	} else {
		for(int i = 0; i != 10; ++i)
		{
			z = solve(now - 1, 0);
			d += z, d.v[i] += z.size;
		}
	}

	mark[now][limit] = mark_v;
	return dp[now][limit] = d;
}

data_t calc(value_t v)
{
	if(v == 0)
	{
		data_t z;
		z.v[0] = 1;
		return z;
	}

	++mark_v;
	int len = conv(v, digit);
	data_t z = solve(len - 1, 1);
	if(len - 2 >= 0)
	{
		value_t v = 1;
		for(int i = 0; i != len - 1; ++i)
			v = v * 10 + 1;
		z.v[0] -= v - 1;
	} 

	return z;
}

int main()
{
	value_t a, b;
	std::scanf("%lld %lld", &a, &b);
	data_t z = calc(a - 1), k = calc(b);
	for(int i = 0; i != 9; ++i)
		std::printf("%lld ", k.v[i] - z.v[i]);
	std::printf("%lld", k.v[9] - z.v[9]);
	return 0;
}
