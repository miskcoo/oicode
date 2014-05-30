/*
 * BZOJ-3238 [Ahoi2013]差异
 *  后缀数组，笛卡尔树
 */
#include <cstdio>
#include <cstring>

const int MaxN = 500010;
int left[MaxN], right[MaxN];
int father[MaxN], size[MaxN];
int height[MaxN], sa[MaxN], rank[MaxN];
char str[MaxN];

template<
	typename StrIter,
	typename OutIterSA, 
	typename OutIterRank>
inline OutIterSA suffix_array(
	int alpha_table_size,
	StrIter str, StrIter str_end,
	OutIterSA sa, OutIterRank rank)
{
	int len = str_end - str;
	int m = alpha_table_size;
	int size = m > len ? m : len;
	int* mems = new int[size << 2];
	int* kx = mems, *ky = mems + size;
	int* wa = ky + size, *wb = wa + size;

	for(int i = 0; i != m; ++i)
		wa[i] = 0;
	for(int i = 0; i != len; ++i)
		++wa[kx[i] = str[i]];
	for(int i = 1; i != m; ++i)
		wa[i] += wa[i - 1];
	for(int i = len - 1; i >= 0; --i)
		sa[--wa[kx[i]]] = i;

	int p = 0;
	for(int j = 1; p != len; j <<= 1, m = p)
	{
		p = 0;
		for(int i = len - j; i != len; ++i)
			ky[p++] = i;
		for(int i = 0; i != len; ++i)
			if(sa[i] >= j) ky[p++] = sa[i] - j;

		for(int i = 0; i != len; ++i)
			wb[i] = kx[ky[i]];
		for(int i = 0; i != m; ++i)
			wa[i] = 0;
		for(int i = 0; i != len; ++i)
			++wa[wb[i]];
		for(int i = 1; i != m; ++i)
			wa[i] += wa[i - 1];
		for(int i = len - 1; i >= 0; --i)
			sa[--wa[wb[i]]] = ky[i];

		int* temp = kx;
		kx = ky;
		ky = temp;

		p = 1, kx[sa[0]] = 0;
		for(int i = 1; i != len; ++i)
		{
			int a = sa[i], na = a + j;
			int b = sa[i - 1], nb = b + j;
			if(ky[a] == ky[b] && na < len && 
				nb < len && ky[na] == ky[nb])
			{
				kx[a] = p - 1;
			} else {
				kx[a] = p++;
			}
		}
	}

	if(rank)
	{
		for(int i = 0; i != len; ++i)
			rank[i] = kx[i];
	}

	delete[] mems;
	return sa;
}

int build_cartesian_tree(int n)
{
	father[0] = left[0] = right[0] = 0;
	height[0] = size[0] = 0;

	int roof = 0;
	int now = 0;
	for(int i = 1; i != n; ++i)
	{
		if(!roof) 
		{
			roof = now = i;
			continue;
		}

		while(now && height[now] > height[i])
			now = father[now];
		if(now)
		{
			left[i] = right[now];
			right[now] = i;
			father[i] = now;
		} else if(height[roof] > height[i]) {
			left[i] = roof;
			father[roof] = i;
			roof = i;
		} else {
			father[i] = roof;
			left[i] = right[roof];
			right[roof] = i;
		}

		now = i;
	}

	return roof;
}

void dfs_size(int u)
{
	size[u] = 1;
	if(left[u])
	{
		dfs_size(left[u]);
		size[u] += size[left[u]];
	}

	if(right[u])
	{
		dfs_size(right[u]);
		size[u] += size[right[u]];
	}
}

int main()
{
	std::scanf("%s", str);
	int len = std::strlen(str);
	for(int i = 0; i != len; ++i)
		str[i] -= 'a';
	suffix_array(26, str, str + len, sa, rank);
	for(int i = 0, k = 0; i != len; height[rank[i++]] = k)
	{
		if(!rank[i])
		{
			k = 0;
			continue;
		}
		if(k != 0) --k;
		int j = sa[rank[i] - 1];
		while(i + k < len && j + k < len && str[i + k] == str[j + k])
			++k;
	}
	int roof = build_cartesian_tree(len);
	dfs_size(roof);
	long long ans = (len + 1LL) * len * (len - 1) / 2;
	for(int i = 1; i != len; ++i)
		ans -= (size[left[i]] + 1LL) * (size[right[i]] + 1) * height[i] * 2;
	std::printf("%lld", ans);
	return 0;
}
