/*
 * BZOJ-3576 [Hnoi2014]江南乐
 *  博弈论-sg函数
 */
#include <cstdio>
#include <cstring>

int sg[100002];
int temp[100002];
int temp_v = 0;
inline void make_sg(int i)
{
	++temp_v;
	for(int l = 2; l <= i; )
	{
		int p = i / l;
		int r = i / p;

		int v = 0, k = i - p * l;
		if(k & 1) v ^= sg[p + 1];
		if((l - k) & 1) v ^= sg[p];
		temp[v] = temp_v;
		if(r != l)
		{
			if(p & 1)
			{
				temp[v ^ sg[p + 1]] = temp_v;
			} else {
				temp[v ^ sg[p]] = temp_v;
			}
		}
		l = r + 1;
	}

	int mex = 0;
	while(temp[mex] == temp_v)
		++mex;
	sg[i] = mex;
}

int main(int argc, char* argv[])
{
	int T, F;
	std::scanf("%d %d", &T, &F);

	int now_sg = F;
	if(now_sg == 1) now_sg = 2;
	for(int i = 0; i != T; ++i)
	{
		int N;
		std::scanf("%d", &N);
		int v = 0;
		for(int j = 0; j != N; ++j)
		{
			int inp;
			std::scanf("%d", &inp);
			while(now_sg <= inp)
				make_sg(now_sg++);
			v ^= sg[inp];
		}
		std::printf("%d", v != 0 ? 1 : 0);
		if(i != T - 1) std::printf(" ");
	}
	return 0;
}
