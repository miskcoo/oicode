/* BZOJ-3620: 似乎在梦中见过的样子
 *   KMP */
#include <cstdio>
#include <cstring>

const int MaxN = 15010;
char str[MaxN];
int len, k, next[MaxN];

int calc(const char* str)
{
	next[1] = 0;
	for(int i = 2; str[i]; ++i)
	{
		int now = next[i - 1];
		while(now && str[now + 1] != str[i])
			now = next[now];
		if(str[now + 1] == str[i]) 
			next[i] = now + 1;
		else next[i] = now;
	}

	int ans = 0, now = 0;
	for(int i = 1; str[i]; ++i)
	{
		while(now && str[now + 1] != str[i])
			now = next[now];
		if(str[now + 1] == str[i])
			++now;
		while(now << 1 >= i) now = next[now];
		if(now >= k) ++ans;
	}

	return ans;
}

int main()
{
	std::scanf("%s", str);
	int ans = 0;
	std::scanf("%d", &k);
	for(int i = 0; str[i]; ++i)
		ans += calc(str + i - 1);
	std::printf("%d", ans);
	return 0;
}
