/* Codeforce 26C. Parquet
 *   构造
 *   combinatorics, constructive algorithms */
#include <cstdio>

const int MaxN = 101;
char map[MaxN][MaxN];
int n, m, a, b, c;

bool solve()
{
	if(n % 2 && m % 2)
		return false;
	if(n & 1)
	{
		for(int i = 0; i != m; i += 2, --a)
		{
			if(i & 2) map[0][i] = map[0][i + 1] = 'z';
			else map[0][i] = map[0][i + 1] = 'y';
		}

		if(a < 0) return false;
	} else if(m & 1) {
		for(int i = 0; i != n; i += 2, --b)
		{
			if(i & 2) map[i][0] = map[i + 1][0] = 'z';
			else map[i][0] = map[i + 1][0] = 'y';
		}

		if(b < 0) return false;
	}

	int x = n & 1, y = m & 1;
	while(x < n && y < m)
	{
		if(c)
		{
			char p = (x ^ y) & 2 ? 'w' : 'x';
			map[x][y] = map[x + 1][y] = p;
			map[x][y + 1] = map[x + 1][y + 1] = p;
			--c;
		} else if(b >= 2) {
			char p1 = x & 2 ? 'p' : 'q';
			char p2 = x & 2 ? 'q' : 'p';
			map[x][y] = map[x + 1][y] = p1;
			map[x][y + 1] = map[x + 1][y + 1] = p2;
			b -= 2;
		} else {
			char p1 = y & 2 ? 'o' : 's';
			char p2 = y & 2 ? 's' : 'o';
			map[x][y] = map[x][y + 1] = p1;
			map[x + 1][y] = map[x + 1][y + 1] = p2;
			a -= 2;
		}
		y += 2;
		if(y == m) x += 2, y = m & 1;
	}

	return a >= 0 && b >= 0 && c >= 0;
}

int main()
{
	std::scanf("%d %d %d %d %d", &n, &m, &a, &b, &c);
	if(solve())
	{
		for(int i = 0; i != n; ++i)
			std::puts(map[i]);
	} else {
		std::puts("IMPOSSIBLE");
	}
	return 0;
}
