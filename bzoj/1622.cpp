/* BZOJ-1622: [Usaco2008 Open]Word Power 名字的能量
 *   暴力 */
#include <cstdio>
#include <cstring>

const int MaxN = 1001, MaxM = 100, MaxE = 31;
char names[MaxN][MaxN], energys[MaxM][MaxE];

void to_lowercase(char* str)
{
	for(int i = 0; str[i]; ++i)
	{
		if(str[i] >= 'A' && str[i] <= 'Z')
			str[i] -= 'A' - 'a';
	}
}

int check(const char* str, const char* pat)
{
	for(int i = 0, j = 0; pat[i]; ++i)
	{
		while(str[j] && pat[i] != str[j]) ++j;
		if(!str[j++]) return 0;
	}

	return 1;
}

int main()
{
	int n, m;
	std::scanf("%d %d", &n, &m);
	for(int i = 0; i != n; ++i)
	{
		std::scanf("%s", names[i]);
		to_lowercase(names[i]);
	}

	for(int i = 0; i != m; ++i)
	{
		std::scanf("%s", energys[i]);
		to_lowercase(energys[i]);
	}

	for(int i = 0; i != n; ++i)
	{
		int count = 0;
		for(int j = 0; j != m; ++j)
			count += check(names[i], energys[j]);
		std::printf("%d\n", count);
	}
	return 0;
}
