/*
 * BZOJ-3343 教主的魔法
 *  分块
 */
#include <cstdio>
#include <cstring>
#include <algorithm>

int N, Q;
int bsize = 1000;

struct block
{
	int diff;
	int raw[1002];
	int elem[1002];
} bk[1002];

void range_partial_add(block* b, int l, int r, int v)
{
	for(int i = l; i <= r; ++i)
		b->raw[i] += v;
	std::memcpy(b->elem, b->raw, sizeof(b->elem));
	std::sort(b->elem, b->elem + bsize);
}

int range_partial_count(block* b, int l, int r, int v)
{
	int count = 0;
	v -= b->diff;
	for(int i = l; i <= r; ++i)
		count += b->raw[i] >= v ? 1 : 0;
	return count;
}

void range_add(int l, int r, int v)
{
	int lr = l / bsize, lz = l % bsize;
	int rr = r / bsize, rz = r % bsize;
	for(int i = lr + 1; i < rr; ++i)
		bk[i].diff += v;

	if(lr == rr)
	{
		range_partial_add(bk + lr, lz, rz, v);
	} else {
		range_partial_add(bk + lr, lz, bsize - 1, v);
		range_partial_add(bk + rr, 0, rz, v);
	}
}

int range_count(int l, int r, int v)
{
	int count = 0;
	int lr = l / bsize, lz = l % bsize;
	int rr = r / bsize, rz = r % bsize;
	for(int i = lr + 1; i < rr; ++i)
	{
		int fd = v - bk[i].diff;
		int la = 0, ra = bsize;
		while(la < ra)
		{
			int mid = (la + ra) >> 1;
			if(bk[i].elem[mid] < fd) la = mid + 1;
			else ra = mid;
		}
		count += bsize - la;
	}

	if(lr == rr)
	{
		count += range_partial_count(bk + lr, lz, rz, v);
	} else {
		count += range_partial_count(bk + lr, lz, bsize - 1, v);
		count += range_partial_count(bk + rr, 0, rz, v);
	}
	return count;
}

int main()
{
	std::scanf("%d %d", &N, &Q);
		
	for(int i = 0; i != (N + bsize - 1) / bsize; ++i)
	{
		for(int k = 0; k != bsize; ++k)
		{
			int data;
			std::scanf("%d", &data);
			bk[i].elem[k] = bk[i].raw[k] = data;
		}
		std::sort(bk[i].elem, bk[i].elem + bsize);
	}
		
	for(int i = 0; i != Q; ++i)
	{
		char op[3];
		int a, b, c;
		std::scanf("%s %d %d %d", op, &a, &b, &c);
		if(*op == 'M') range_add(a - 1, b - 1, c);
		else std::printf("%d\n", range_count(a - 1, b - 1, c));
	}
	return 0;	
}
