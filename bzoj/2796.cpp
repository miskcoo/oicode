/* BZOJ-2796: [Poi2012]Fibonacci Representation
 *  记忆化搜索 */
#include <map>
#include <cstdio>
#include <algorithm>

int len;
unsigned long long fib[100];
std::map<unsigned long long, int> record;

void init_fib()
{
	fib[0] = fib[1] = 1;
	for(len = 2; len != 93; ++len)
		fib[len] = fib[len - 1] + fib[len - 2];
}

int solve(unsigned long long v)
{
	if(v == 0) return 0;
	else if(v == 1) return 1;
	else if(record.count(v))
		return record[v];
	int p = std::lower_bound(fib, fib + len, v) - fib;
	int v1 = solve(fib[p] - v);
	int v2 = solve(v - fib[p - 1]);
	return record[v] = std::min(v1, v2) + 1;
}

int main()
{
	init_fib();
	int q;
	std::scanf("%d", &q);
	while(q --> 0)
	{
		unsigned long long v;
		std::scanf("%llu", &v);
		std::printf("%d\n", solve(v));
	}
	return 0;
}
