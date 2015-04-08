/* BZOJ-2875: [Noi2012]随机数生成器
 *  快速乘，矩阵快速幂 */
#include <cstdio>

typedef unsigned long long value_t;

value_t p;
value_t mod_mul(value_t a, value_t b)
{
	value_t v = 0;
	while(b)
	{
		if(b & 1) 
		{
			v += a;
			if(v >= p) v -= p;
		}

		a <<= 1;
		if(a >= p) a -= p;
		b >>= 1;
	}

	return v;
}

struct matrix_t
{
	value_t m[2][2];

	matrix_t()
	{
		m[0][0] = m[1][1] = 1;
		m[0][1] = m[1][0] = 0;
	}

	matrix_t operator * (const matrix_t& b)
	{
		matrix_t n;
		for(int i = 0; i != 2; ++i)
		{
			for(int j = 0; j != 2; ++j)
			{
				value_t v = 0;
				for(int k = 0; k != 2; ++k)
					v += mod_mul(m[i][k], b.m[k][j]);
				n.m[i][j] = v >= p ? v - p : v;
			}
		}

		return n;
	}
};

matrix_t matrix_pow(matrix_t a, value_t b)
{
	matrix_t v;
	while(b)
	{
		if(b & 1) v = v * a;
		a = a * a;
		b >>= 1;
	}

	return v;
}

int main()
{
	value_t a, c, x0, n, g;
	std::scanf("%llu %llu %llu %llu %llu %llu", &p, &a, &c, &x0, &n, &g);
	matrix_t x;
	x.m[0][0] = a % p;
	x.m[0][1] = c % p;
	x.m[1][0] = 0;
	x.m[1][1] = 1;
	x = matrix_pow(x, n);
	value_t ans = mod_mul(x.m[0][0], x0) + x.m[0][1];
	if(ans >= p) ans -= p;
	std::printf("%llu", ans % g);
	return 0;
}
