/* BZOJ-1814: Ural 1519 Formula 1 
 *   插头DP（最小表示法）*/
#include <cstdio>
#include <cstring>
#include <algorithm>

const int MaxN = 15, MaxS = 1000001;
int n, m, last_x, last_y;
char data[MaxN][MaxN];

struct hash_t
{
	static const int mod_v = 30007;
	int total, head[mod_v], next[MaxS];
	long long status[MaxS], val[MaxS];

	void reset()
	{
		total = 0;
		std::memset(head, 0, sizeof(head));
	}

	long long& operator [](long long state)
	{
		int hash_v = state % mod_v;
		for(int k = head[hash_v]; k; k = next[k])
			if(status[k] == state)
				return val[k];

		status[++total] = state;
		val[total] = 0;
		next[total] = head[hash_v];
		head[hash_v] = total;
		return val[total];
	}
} hash[2];

void decode(int *c, long long v)
{
	for(int i = m; i >= 0; --i)
	{
		c[i] = v & 7;
		v >>= 3;
	}
}

long long encode(int *c)
{
	static int map[MaxN];
	std::memset(map, -1, sizeof(map));
	map[0] = 0;
	int cnt = 1;
	long long ans = 0;
	for(int i = 0; i <= m; ++i)
	{
		if(map[c[i]] == -1) map[c[i]] = cnt++;
		ans = (ans << 3) | (c[i] = map[c[i]]);
	}
	return ans;
}

void shift_line(int *c)
{
	for(int i = m; i; --i)
		c[i] = c[i - 1];
	c[0] = 0;
}

void solve_blank(int i, int j, hash_t &f, hash_t &g)
{
	static int c[MaxN];
	for(int k = 1; k <= g.total; ++k)
	{
		decode(c, g.status[k]);
		int left = c[j], top = c[j + 1];
		if(!left && !top)
		{
			// no plugs, construct a new connected component
			if(data[i][j + 1] == '.' && data[i + 1][j] == '.')
			{
				c[j] = c[j + 1] = n + 1;
				f[encode(c)] += g.val[k];
			}
		} else if(left && top) {
			if(left == top)
			{
				// connect the path, only in the last blank
				if(i == last_x && j == last_y)
				{
					c[j] = c[j + 1] = 0;
					if(j + 1 == m) shift_line(c);
					f[encode(c)] += g.val[k];
				}
			} else {
				// merge two connected components
				c[j] = c[j + 1] = 0;
				for(int i = 0; i <= m; ++i)
					if(c[i] == top) c[i] = left;
				if(j + 1 == m) shift_line(c);
				f[encode(c)] += g.val[k];
			}
		} else {
			int plug = left ? left : top;

			// connect to next blank
			if(data[i][j + 1] == '.')
			{
				c[j] = 0;
				c[j + 1] = plug;
				f[encode(c)] += g.val[k];
			}

			// connect to next line
			if(data[i + 1][j] == '.')
			{
				c[j] = plug;
				c[j + 1] = 0;
				if(j + 1 == m) shift_line(c);
				f[encode(c)] += g.val[k];
			}
		}
	}
}

void solve_block(int i, int j, hash_t& f, hash_t &g)
{
	static int c[MaxN];
	for(int k = 1; k <= g.total; ++k)
	{
		decode(c, g.status[k]);
		c[j] = c[j + 1] = 0;
		if(j + 1 == m) shift_line(c);
		f[encode(c)] += g.val[k];
	}
}

int main()
{
	std::scanf("%d %d", &n, &m);
	if(n < m)
	{
		char tmp[MaxN];
		for(int i = 0; i != n; ++i)
		{
			std::scanf("%s", tmp);
			for(int j = 0; j != m; ++j)
				data[j][i] = tmp[j];
		}

		std::swap(n, m);
	} else {
		for(int i = 0; i != n; ++i)
			std::scanf("%s", data[i]);
	}

	last_x = last_y = -1;
	for(int i = 0; i != n; ++i)
	{
		for(int j = 0; j != m; ++j)
		{
			if(data[i][j] == '.')
				last_x = i, last_y = j;
		}
	}

	if(last_x == -1)
	{
		std::puts("0");
	} else {
		int cur = 0;
		hash[cur][0] = 1;
		for(int i = 0; i != n; ++i)
		{
			for(int j = 0; j != m; ++j)
			{
				hash[cur ^= 1].reset();
				if(data[i][j] == '.') 
					solve_blank(i, j, hash[cur], hash[cur ^ 1]);
				else solve_block(i, j, hash[cur], hash[cur ^ 1]);
			}
		}

		long long ans = 0;
		for(int k = 1; k <= hash[cur].total; ++k)
			ans += hash[cur].val[k];
		std::printf("%lld\n", ans);
	}
	return 0;
}
