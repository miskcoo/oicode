/* UOJ-#113. 【UER #2】手机的生产 */
#include <cstdio>

const int MaxN = 100010, mod_v = 998244353;
long long count[MaxN][2];
int n, oper[MaxN], next[MaxN];

long long solve(int now, int val)
{
	if(count[now][val]) 
		return count[now][val];
	if(val == 1)
	{
		if(oper[now] == 1)  // &&
		{
			count[now][val] += solve(now + 1, 1);
			count[now][val] += solve(now + 1, 0);
		} else {   // ||
			count[now][val] += 1;
		}
	} else {
		if(oper[now] == 1)  // &&
		{
			if(next[now] < n - 1)
			{
				count[now][val] += solve(next[now] + 1, 1);
				count[now][val] += solve(next[now] + 1, 0);
			} else count[now][val] += 1;
		} else {  // ||
			count[now][val] += solve(now + 1, 1);
			count[now][val] += solve(now + 1, 0);
		}
	}

	return count[now][val] %= mod_v;
}

int main()
{
	std::scanf("%d", &n);
	for(int i = 0; i != n - 1; ++i)
	{
		char op[3];
		std::scanf("%s", op);
		if(*op == '|') oper[i] = 0;
		else oper[i] = 1;
	}

	int tmp = n;
	for(int i = n - 1; i >= 0; --i)
	{
		next[i] = tmp;
		if(!oper[i]) tmp = i;
	}

	count[n - 1][0] = count[n - 1][1] = 1;
	long long ans = solve(0, 1) + solve(0, 0);
	std::printf("%lld\n", ans % mod_v);
	return 0;
}
