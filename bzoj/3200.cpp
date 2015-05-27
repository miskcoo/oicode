/* BZOJ-3200: 表达式
 *   DP+网络流 */
#include <cstdio>
#include <cstring>
#include <algorithm>
#include <vector>

const int MaxN = 25100;
typedef std::vector<int> container_t;
typedef container_t::iterator iter_t;
char str[MaxN];

class bigraph
{
	int mark_v, size;
	container_t son[MaxN];
	int fa[MaxN], mark[MaxN];

	bool _check(int u)
	{
		mark[u] = mark_v;
		if(fa[u] == -1) return true;
		int ru = fa[u];
		for(iter_t it = son[ru].begin(); it != son[ru].end(); ++it)
		{
			if(mark[*it] != mark_v && _check(*it))
				return true;
		}

		return false;
	}

	bool _augment(int u)
	{
		if(mark[u] == mark_v) return false;
		mark[u] = mark_v;
		for(iter_t it = son[u].begin(); it != son[u].end(); ++it)
		{
			if(fa[*it] == -1 || _augment(fa[*it]))
			{
				fa[*it] = u;
				fa[u] = *it;
				return true;
			}
		}

		return false;
	}

public:
	void add_edge(int u, int v)
	{
		son[u].push_back(v);
	}

	void clear(int n)
	{
		++mark_v, size = n;
		for(int i = 0; i != n + 26; ++i)
			son[i].clear();
		std::fill(fa, fa + n + 26, -1);
	}

	bool check(int u)
	{
		++mark_v;
		return _check(u);
	}

	int calc()
	{
		int ans = 0;
		for(int i = 0; i != size; ++i)
		{
			++mark_v;
			ans += _augment(i);
		}

		return ans;
	}
} bi;

container_t son[MaxN];
int oper_del, st[MaxN], stv[MaxN];
int count[MaxN], occer[MaxN];
bool is_oper[MaxN], is_del[MaxN];

void reduction(int now)
{
	if(!is_oper[now]) return;
	std::vector<int> record;
	for(iter_t it = son[now].begin(); it != son[now].end(); ++it)
	{
		reduction(*it);
		if(str[*it] == str[now])
		{
			++oper_del;
			is_del[*it] = true;
			record.push_back(*it);
		}
	}

	for(int i = 0; i != record.size(); ++i)
	{
		int u = record[i];
		for(iter_t it = son[u].begin(); it != son[u].end(); ++it)
			if(!is_del[*it]) son[now].push_back(*it);
		son[u].clear();
	}
}

void solve(int now)
{
	if(!is_oper[now])
	{
		count[now] = 0;
		occer[now] = 1 << (str[now] - 'a');
		return;
	}

	std::vector<int> st;
	for(iter_t it = son[now].begin(); it != son[now].end(); ++it)
	{
		if(!is_del[*it])
		{
			solve(*it);
			count[now] += count[*it];
			if(is_oper[*it])
			{
				st.push_back(*it);
			} else {
				if(occer[now] & occer[*it])
					++count[now];
				occer[now] |= occer[*it];
			}
		}
	}

	int tot = st.size();
	bi.clear(tot);
	for(int i = 0; i != tot; ++i)
	{
		for(int j = 0; j != 26; ++j)
		{
			if(occer[now] & (1 << j) && occer[st[i]] & (1 << j))
			{
				bi.add_edge(i, j + tot);
				bi.add_edge(j + tot, i);
			}
		}
	}

	count[now] += bi.calc();
	for(int i = 0; i != 26; ++i)
	{
		if(occer[now] & (1 << i)) continue;
		for(int j = 0; j != tot; ++j)
		{
			if(occer[st[j]] & (1 << i) && bi.check(j))
			{
				occer[now] |= 1 << i;
				break;
			}
		}
	}
}

int main()
{
	std::scanf("%s", str);
	int len = std::strlen(str);
	int stop = 0, root;
	for(int i = 0; str[i]; ++i)
	{
		is_del[i] = false;
		if(str[i] >= 'a' && str[i] <= 'z')
		{
			is_oper[i] = false;
			st[stop++] = i;
		} else {
			root = i;
			is_oper[i] = true;
			son[i].push_back(st[--stop]);
			son[i].push_back(st[--stop]);
			st[stop++] = i;
		}
	}

	reduction(root);
	solve(root);
	std::printf("%d\n", len - oper_del - count[root]);
	return 0;
}
