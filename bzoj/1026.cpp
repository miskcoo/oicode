/* BZOJ-1026: [SCOI2009]windy数
 *   数位DP */
#include <cstdio>
#include <cstring>

char digit[11];
int dp[11][11][2][2];

int abs(int x) { return x < 0 ? -x : x; }
int f(int now, int val, int lim, int zero)
{
	if(!digit[now]) return 1;
	if(dp[now][val][lim][zero]) 
		return dp[now][val][lim][zero];

	if(lim)
	{
		int x = digit[now] - '0';
		for(int i = 0; i != x; ++i)
			if(abs(i - val) >= 2 || zero)
				dp[now][val][lim][zero] += f(now + 1, i, 0, zero && !i);
		if(abs(val - x) >= 2 || zero)
			dp[now][val][lim][zero] += f(now + 1, x, 1, 0);
	} else {
		for(int i = 0; i != 10; ++i)
			if(abs(i - val) >= 2 || zero)
				dp[now][val][lim][zero] += f(now + 1, i, 0, zero && !i);
	}

	return dp[now][val][lim][zero];
}

bool check()
{
	for(int i = 1; digit[i]; ++i)
		if(abs(digit[i] - digit[i - 1]) < 2)
			return false;
	return true;
}

int main()
{
	std::scanf("%s", digit);
	int a = f(0, 0, 1, 1) - check();
	std::scanf("%s", digit);
	std::memset(dp, 0, sizeof(dp));
	int b = f(0, 0, 1, 1);
	std::printf("%d", b - a);
	return 0;
}
