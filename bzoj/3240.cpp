/* BZOJ-3240: [Noi2013]矩阵游戏
 *   十进制矩阵快速幂 */
#include <cstdio>
#include <cstdlib>
#include <cstring>

const long long mod_v = 1000000007;
const int MaxN = 1000010;

struct matrix_t
{
	long long m[2][2];

	matrix_t() 
	{
		m[0][0] = m[1][1] = 1;
		m[0][1] = m[1][0] = 0;
	}

	matrix_t operator * (const matrix_t& r) const
	{
		matrix_t ans;
		for(int i = 0; i != 1; ++i)
		{
			for(int j = 0; j != 2; ++j)
			{
				long long v = 0;
				for(int k = 0; k != 2; ++k)
					v += m[i][k] * r.m[k][j];
				ans.m[i][j] = v % mod_v;
			}
		}

		return ans;
	}
};

matrix_t matrix_pow(matrix_t x, int p)
{
	matrix_t v;
	while(p)
	{
		if(p & 1) v = x * v;
		x = x * x;
		p >>= 1;
	}
	return v;
}

matrix_t matrix_pow(matrix_t x, const char* p)
{
	matrix_t ans;
	int len = std::strlen(p);
	int start = 0;
	while(p[start] == '0') ++start;
	for(int i = len - 1; i >= start; --i)
	{
		ans = matrix_pow(x, p[i] - '0') * ans;
		x = matrix_pow(x, 10);
	}

	return ans;
}

void dec(char *str)
{
	int len = std::strlen(str);
	int carry = 1;
	for(int i = len - 1; i >= 0; --i)
	{
		str[i] -= carry;
		if(str[i] < '0') 
		{
			str[i] += 10;
			carry = 1;
		} else break;
	}
}

int a, b, c, d;
char n[MaxN], m[MaxN];
int main()
{
	matrix_t m1, m2;
	std::scanf("%s %s %d %d %d %d", n, m, &a, &b, &c, &d);
	m1.m[0][0] = a % mod_v, m1.m[0][1] = b % mod_v;
	m2.m[0][0] = c % mod_v, m2.m[0][1] = d % mod_v;
	dec(n), dec(m);
	m1 = matrix_pow(m1, m);
	m2 = m1 * matrix_pow(m2 * m1, n);
	std::printf("%lld\n", (m2.m[0][0] + m2.m[0][1]) % mod_v);
	return 0;
}
