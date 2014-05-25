/*
 * BZOJ-2124 等差子序列
 *  线段树+字符串哈希
 */
#include <cstdio>
#include <cstring>

const unsigned long long mod_const = 1000000007ULL;
const unsigned long long base_const = 7ULL;
unsigned power[10001];
unsigned data[100001];
unsigned N;

class str_hash
{
	unsigned _f[40003];
	void _push_elem(int elem, int now, int head, int tail)
	{
		if(head == tail)
		{
			_f[now] = 1;
			return;
		}
		
		int middle = (head + tail) >> 1;
		if(elem <= middle)
			_push_elem(elem, now << 1, head, middle);
		else _push_elem(elem, (now << 1) + 1, middle + 1, tail);
		
		_f[now] = ((unsigned long long)_f[now << 1] * 
			power[tail - middle] + _f[(now << 1) + 1]) % mod_const;
	}
	
	unsigned _ask(int beg, int end, int now, int head, int tail)
	{
		if(head == beg && end == tail)
			return _f[now];
			
		int middle = (head + tail) >> 1;
		if(end <= middle) return _ask(beg, end, now << 1, head, middle);
		if(beg > middle) return _ask(beg, end, (now << 1) + 1, middle + 1, tail);
		
		unsigned long long left = _ask(beg, middle, now << 1, head, middle);
		unsigned long long right = _ask(middle + 1, end, (now << 1) + 1, middle + 1, tail);
		return (left * power[end - middle] + right) % mod_const;
	}
public:
	void push_elem(int elem)
	{
		_push_elem(elem, 1, 1, N);
	}
	
	unsigned ask(int beg, int end)
	{
		return _ask(beg, end, 1, 1, N);
	}
	
	str_hash()
	{
		std::memset(_f, 0, sizeof(_f));
	}
};

int main()
{
	power[0] = 1;
	for(int i = 1; i != 10001; ++i)
		power[i] = power[i - 1] * base_const % mod_const;
		
	int T;
	bool re_false = false;
	std::scanf("%d", &T);
	while(T--)
	{
		std::scanf("%d", &N);
		str_hash ha, hr;
		
		for(int i = 0; i != N; ++i)
			std::scanf("%d", data + i);
		
		for(int i = 0; i != N; ++i)
		{
			int elem = data[i];
			int k = (elem << 1) - 1;
			if(k <= N)
			{
				if(ha.ask(1, k) != hr.ask(N + 1 - k, N))
				{
					std::printf("Y\n");
					re_false = false;
					break;
				}
			} else if(ha.ask(N - 2 * (N - elem), N)
				!= hr.ask(1, N + 1 - (N - 2 * (N - elem)))) {
				std::printf("Y\n");
				re_false = false;
				break;
			}
			ha.push_elem(elem);
			hr.push_elem(N + 1 - elem);
			re_false = true;
		}
		if(re_false) std::printf("N\n");
	}
		
	return 0;
}

