/* 
 * BZOJ-1513 Tet-Tetris 3D
 *  二维线段树
 */
#include <cstdio>
#include <cstring>

int width, length;
int pos_map_x[4010];
int pos_map_y[4010];
int map_len_x = 0;
int map_len_y = 0;
int ra, rb, v;

template<typename T, typename LZ>
struct tnode
{
	T value;
	LZ lazy;
};

class ltree
{
	tnode<int, int> elem[2010];
public:
	static int max(int a, int b)
	{
		return a < b ? b : a;
	}

	int ask(int now, int a, int b, int head, int tail)
	{
		if(a == head && b == tail)
			return elem[pos_map_y[now]].value;

		int m = (head + tail) >> 1;
		int mm = elem[pos_map_y[now]].lazy;
		if(m >= b) return max(mm, ask(now << 1, a, b, head, m));
		if(m < a) return max(mm, ask((now << 1) + 1, a, b, m + 1, tail));

		return max(mm, max(ask(now << 1, a, m, head, m),
				ask((now << 1) + 1, m + 1, b, m + 1, tail)));
	}

	void modify(int now, int a, int b, int head, int tail)
	{
		if(a == head && b == tail)
		{
			int mv = pos_map_y[now];
			elem[mv].value = max(v, elem[mv].value);
			elem[mv].lazy = max(v, elem[mv].lazy);
			return;
		}

		bool flag = true;
		int m = (head + tail) >> 1;
		if(m >= b)
		{
			modify(now << 1, a, b, head, m);
			flag = false;
		}
		if(m < a)
		{
			modify((now << 1) + 1, a, b, m + 1, tail);
			flag = false;
		}
		if(flag)
		{
			modify(now << 1, a, m, head, m);
			modify((now << 1) + 1, m + 1, b, m + 1, tail);
		}
		elem[pos_map_y[now]].value = max(elem[pos_map_y[now]].value, v);
	}
};

class mtree
{
	tnode<ltree, ltree> elem[2010];
public:

	static int max(int a, int b)
	{
		return a < b ? b : a;
	}

	int ask(int now, int a, int b, int head, int tail)
	{
		if(a == head && b == tail)
			return elem[pos_map_x[now]].value.ask(1, ra, rb, 1, width);

        int mm = elem[pos_map_x[now]].lazy.ask(1, ra, rb, 1, width);
		int m = (head + tail) >> 1;
		if(m >= b) return max(mm, ask(now << 1, a, b, head, m));
		if(m < a) return max(mm, ask((now << 1) + 1, a, b, m + 1, tail));

		return max(mm, max(ask(now << 1, a, m, head, m),
				ask((now << 1) + 1, m + 1, b, m + 1, tail)));
	}

	void modify(int now, int a, int b, int head, int tail)
	{
		int mv = pos_map_x[now];
		if(a == head && b == tail)
		{
			elem[mv].value.modify(1, ra, rb, 1, width);
			elem[mv].lazy.modify(1, ra, rb, 1, width);
			return;
		}

		bool flag = true;
		int m = (head + tail) >> 1;
		if(m >= b)
		{
			modify(now << 1, a, b, head, m);
			flag = false;
		}
		if(m < a)
		{
			modify((now << 1) + 1, a, b, m + 1, tail);
			flag = false;
		}

		if(flag)
		{
			modify(now << 1, a, m, head, m);
			modify((now << 1) + 1, m + 1, b, m + 1, tail);
		}
		elem[mv].value.modify(1, ra, rb, 1, width);
	}
} stre;

void make_map_x(int now, int head, int tail)
{
	pos_map_x[now] = map_len_x++;
	if(head != tail)
	{
		int m = (head + tail) >> 1;
		make_map_x(now << 1, head, m);
		make_map_x((now << 1) + 1, m + 1, tail);
	}
}

void make_map_y(int now, int head, int tail)
{
	pos_map_y[now] = map_len_y++;
	if(head != tail)
	{
		int m = (head + tail) >> 1;
		make_map_y(now << 1, head, m);
		make_map_y((now << 1) + 1, m + 1, tail);
	}
}

int main()
{
	int N;
	std::scanf("%d %d %d", &length, &width, &N);
	make_map_x(1, 1, length);
	make_map_y(1, 1, width);
	while(N--)
	{
		int d, s, w, x, y;
		std::scanf("%d %d %d %d %d", &d, &s, &w, &x, &y);
		ra = y + 1, rb = y + s;
		v = w + stre.ask(1, x + 1, x + d, 1, length);
		stre.modify(1, x + 1, x + d, 1, length);
	}
	ra = 1, rb = width;
	std::printf("%d\n", stre.ask(1, 1, length, 1, length));
	return 0;
}
