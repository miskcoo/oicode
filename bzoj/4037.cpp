/* BZOJ-4037: [HAOI2015]Str
 *   DP+十进制矩阵快速幂 */
#include <cstdio>
#include <algorithm>
#include <cstring>

typedef long long calc_t;
const int mod_v = 998244353;
const int MaxN = 505, MaxM = 5;

int matrix_size;
struct matrix_t
{
	int m[MaxM][MaxM];

	matrix_t operator * (const matrix_t& r) const
	{
		matrix_t z;
		for(int i = 0; i != matrix_size; ++i)
		{
			for(int j = 0; j != matrix_size; ++j)
			{
				calc_t v = 0;
				for(int k = 0; k != matrix_size; ++k)
					v += (calc_t)m[i][k] * r.m[k][j];
				z.m[i][j] = v % mod_v;
			}
		}

		return z;
	}

	matrix_t& operator += (const matrix_t& r)
	{
		for(int i = 0; i != matrix_size; ++i)
		{
			for(int j = 0; j != matrix_size; ++j)
			{
				m[i][j] += r.m[i][j];
				if(m[i][j] >= mod_v) m[i][j] -= mod_v;
			}
		}

		return *this;
	}
} one, pw[10][MaxN], f[MaxN];

int len, vec[MaxM];
char digit[MaxN];

matrix_t matrix_pow(matrix_t x, int p)
{
	matrix_t v = one;
	while(p)
	{
		if(p & 1) v = x * v;
		if(p >> 1) x = x * x;
		p >>= 1;
	}

	return v;
}

void prework()
{
	std::memset(one.m, 0, sizeof(one.m));
	for(int i = 0; i != matrix_size; ++i)
		one.m[i][i] = 1;
	matrix_t unit;
	std::memset(unit.m, 0, sizeof(unit.m));
	for(int i = 0; i != matrix_size; ++i)
		unit.m[0][i] = 1;
	for(int i = 1; i != matrix_size; ++i)
		unit.m[i][i - 1] = 1;

	for(int i = 0; i != 10; ++i)
	{
		pw[i][0] = one;
		pw[i][1] = matrix_pow(unit, i);
		for(int j = 2; j <= len; ++j)
			pw[i][j] = matrix_pow(pw[i][j - 1], 10);
	}

	vec[0] = 1;
	for(int i = 1; i != matrix_size; ++i)
	{
		vec[i] = 0;
		for(int j = i - 1; j >= 0; --j)
		{
			vec[i] += vec[j];
			if(vec[i] >= mod_v) vec[i] -= mod_v;
		}
	}

	std::reverse(vec, vec + matrix_size);
}

int work()
{
	f[0] = one;
	for(int i = 1; i <= len; ++i)
	{
		matrix_t now = one;
		for(int j = i - 1; j >= 0; --j)
		{
			now = pw[digit[j] - '0'][i - j] * now;
			f[i] += now * f[j];
		}
	}

	calc_t ans = 0;
	for(int i = 0; i != matrix_size; ++i)
		ans += (calc_t)vec[i] * f[len].m[matrix_size - 1][i];
	return ans % mod_v;
}

int main()
{
	std::scanf("%s %d", digit, &matrix_size);
	len = std::strlen(digit);
	prework();
	std::printf("%d\n", work());
	return 0;
}
