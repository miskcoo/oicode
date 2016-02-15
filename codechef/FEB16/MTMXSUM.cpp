#include <cstdio>
#include <algorithm>
#include <cstring>

using namespace std;

const int mod_v = 1000000007;
const int MaxN = 100002;
int n, A[MaxN], B[MaxN], SA[MaxN], SB[MaxN];
int st[MaxN], ls[MaxN], rs[MaxN], sl[MaxN], sr[MaxN];

void add(int *S, int l, int r, int v)
{
	S[l] = (S[l] + v) % mod_v;
	S[r + 1] = (S[r + 1] - v) % mod_v;
}

void calc(int *A, int *S)
{
	// build cartesian tree
	memset(ls, 0, sizeof(int) * (n + 1));
	memset(rs, 0, sizeof(int) * (n + 1));
	memset(st, 0, sizeof(int) * (n + 1));
	
	int stop = 1; 
	st[stop] = 1;
	for(int i = 2; i <= n; ++i)
	{
		int out = 0;
		while(stop && A[st[stop]] < A[i]) 
			out = st[stop--];
		rs[st[stop]] = i;
		ls[i] = out;
		st[++stop] = i;
	}

	// calc maximum value
	stop = 1;
	sl[1] = 1, sr[1] = n;
	while(stop)
	{
		int x = st[stop], l = sl[stop], r = sr[stop], s = r - l + 1;
		long long v = A[x] % mod_v;
		--stop;

		int r1 = min(x - l, r - x) + 1;
		int r2 = max(x - l, r - x) + 1;
		for(int i = 1; i < r1; ++i)
			add(S, i, i, v * i % mod_v);
		add(S, r1, r2, v * r1 % mod_v);
		for(int i = r2 + 1; i <= s; ++i)
			add(S, i, i, (s - i + 1) * v % mod_v);

		// push sons into stack
		if(ls[x]) {
			st[++stop] = ls[x];
			sl[stop] = l, sr[stop] = x - 1;
		}

		if(rs[x]) {
			st[++stop] = rs[x];
			sl[stop] = x + 1, sr[stop] = r;
		}
	}

	// sum
	for(int i = 2; i <= n; ++i)
		S[i] = (S[i] + S[i - 1]) % mod_v;
}

int main()
{
	scanf("%d", &n);
	for(int i = 1; i <= n; ++i) {
		scanf("%d", A + i);
		A[i] += i;
	}

	for(int i = 1; i <= n; ++i) {
		scanf("%d", B + i);
		B[i] += i;
	}

	calc(A, SA);
	calc(B, SB);
	for(int i = 1; i <= n; ++i)
		printf("%lld ", (1ll * SA[i] * SB[i] % mod_v + mod_v) % mod_v);
	return 0;
}
