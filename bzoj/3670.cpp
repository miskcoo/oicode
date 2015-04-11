/* BZOJ-3670: [Noi2014]动物园
 *  KMP算法 */
#include <cstdio>

const int MaxN = 1000010, mod_v = 1000000007;
char str[MaxN];
int next[MaxN], num[MaxN];

void kmp_next()
{
	num[1] = 1;
	int cur = 0;
	for(int i = 2; str[i]; ++i)
	{
		while(cur && str[cur + 1] != str[i])
			cur = next[cur];
		if(str[cur + 1] == str[i]) ++cur;
		next[i] = cur;
		num[i] = num[cur] + 1;
	}
}

long long calc()
{
	int cur = 0;
	long long ans = 1;
	for(int i = 2; str[i]; ++i)
	{
		while(cur && str[cur + 1] != str[i])
			cur = next[cur];
		if(str[cur + 1] == str[i]) ++cur;
		while((cur << 1) > i) cur = next[cur];
		ans = ans * (num[cur] + 1) % mod_v;
	}
	return ans;
}

int main()
{
	int n;
	std::scanf("%d", &n);
	while(n --> 0)
	{
		std::scanf("%s", str + 1);
		kmp_next();
		std::printf("%lld\n", calc());
	}
	return 0;
}
