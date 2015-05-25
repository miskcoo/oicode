/* BZOJ-1935: [Shoi2007]Tree 园丁的烦恼 
 *   树状数组+离散化 */
#include <cstdio>
#include <cstring>
#include <algorithm>

const int MaxN = 500010, MaxQ = MaxN * 5;

struct ques_t
{
	int id, type, x, y;
	bool operator < (const ques_t& r) const
	{
		if(x == r.x) 
		{
			if(y == r.y) 
				return type < r.type;
			return y < r.y;
		}

		return x < r.x;
	}
} ques[MaxQ];
int val_tot, ques_tot, discrete[MaxQ];

bool cmp(int a, int b)
{
	return ques[a].y < ques[b].y;
}

void add_ques(int id, int x, int y)
{
	ques[ques_tot].x = x;
	ques[ques_tot].y = y;
	ques[ques_tot].type = 1;
	ques[ques_tot++].id = id;
}

int ta[MaxQ], ans[MaxN];
void modify(int x)
{
	for(; x <= val_tot; x += x & -x)
		++ta[x];
}

int ask(int x)
{
	int v = 0;
	for(; x; x -= x & -x)
		v += ta[x];
	return v;
}

int main()
{
	int n, m;
	std::scanf("%d %d", &n, &m);
	for(int i = 0; i != n; ++i)
	{
		ques[i].id = i, ques[i].type = 0;
		std::scanf("%d %d", &ques[i].x, &ques[i].y);
	}

	ques_tot = n;
	for(int i = 1; i <= m; ++i)
	{
		int a, b, c, d;
		std::scanf("%d %d %d %d", &a, &b, &c, &d);
		add_ques(i, c, d);
		add_ques(i, a - 1, b - 1);
		add_ques(-i, c, b - 1);
		add_ques(-i, a - 1, d);
	}

	for(int i = 0; i != ques_tot; ++i)
		discrete[i] = i;
	std::sort(discrete, discrete + ques_tot, cmp);
	int prev = ~0u >> 1, cnt = 0;
	for(int i = 0; i != ques_tot; ++i)
	{
		int id = discrete[i];
		if(ques[id].y != prev) 
			++cnt, prev = ques[id].y;
		ques[id].y = cnt;
	}

	val_tot = cnt;
	std::sort(ques, ques + ques_tot);

	for(int i = 0; i != ques_tot; ++i)
	{
		if(ques[i].type)
		{
			int v = ask(ques[i].y);
			if(ques[i].id < 0) 
				ans[-ques[i].id] -= v;
			else ans[ques[i].id] += v;
		} else {
			modify(ques[i].y);
		}
	}

	for(int i = 1; i <= m; ++i)
		std::printf("%d\n", ans[i]);

	return 0;
}
