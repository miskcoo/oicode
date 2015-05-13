/* BZOJ-4002: [JLOI2015]有意义的字符串
 *   线性递推+矩阵快速幂 */
#include <cstdio>
#include <algorithm>

typedef long long int64;
typedef unsigned long long uint64;
const int64 mod_v = 7528443412579576937ll;

int64 mod_add(int64 a, int64 b)
{
	uint64 z = uint64(a) + uint64(b);
	if(z >= uint64(mod_v)) return z - mod_v;
	return z;
}

int64 mod_mul(int64 a, int64 b)
{
	if(a < b) std::swap(a, b);
	int64 x = 0;
	while(b)
	{
		if(b & 1) x = mod_add(x, a);
		a = mod_add(a, a);
		b >>= 1;
	}
	return x;
}

struct matrix_t
{
	int64 m[2][2];
	matrix_t operator * (const matrix_t& x) const
	{
		matrix_t z;
		for(int i = 0; i != 2; ++i)
		{
			for(int j = 0; j != 2; ++j)
			{
				int64 v = 0;
				for(int k = 0; k != 2; ++k)
					v = mod_add(v, mod_mul(m[i][k], x.m[k][j]));
				z.m[i][j] = v;
			}
		}

		return z;
	}
};

matrix_t matrix_pow(matrix_t a, int64 p)
{
	matrix_t v;
	v.m[0][0] = v.m[1][1] = 1;
	v.m[1][0] = v.m[0][1] = 0;
	while(p)
	{
		if(p & 1) v = a * v;
		a = a * a;
		p >>= 1;
	}
	return v;
}

int main()
{
	int64 b, d, n;
	std::scanf("%lld %lld %lld", &b, &d, &n);
	if(n == 0) 
	{
		std::puts("1");
		return 0;
	}

	matrix_t m;
	m.m[0][0] = b;
	m.m[0][1] = mod_v - ((b * b - d) >> 2);
	m.m[1][0] = 1;
	m.m[1][1] = 0;
	m = matrix_pow(m, n - 1);
	int64 ans = mod_add(mod_mul(m.m[0][0], b), mod_mul(m.m[0][1], 2));
	if(n % 2 == 0) ans -= 1;
	std::printf("%lld\n", ans);
	return 0;
}
