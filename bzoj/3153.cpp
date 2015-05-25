/* BZOJ-3153: Sone1
 *   Top Tree */
#include <cstdio>
#include <stack>
#include <algorithm>
 
const int MaxN = 202000, inf = ~0u >> 1;

struct info_t
{
	int min, max, sum, size;

	info_t(int min = inf, int max = -inf, int sum = 0, int size = 0)
		: min(min), max(max), sum(sum), size(size) {}

	info_t& operator += (const info_t& a)
	{
		if(a.size == 0) return *this;
		if(size == 0) return *this = a;
		min = std::min(min, a.min);
		max = std::max(max, a.max);
		sum = sum + a.sum;
		size = size + a.size;
		return *this;
	}
};

struct flag_t
{
	int mul, add;

	flag_t(int mul = 1, int add = 0)
		: mul(mul), add(add) {}

	flag_t operator += (const flag_t& a)
	{
		mul = mul * a.mul;
		add = add * a.mul + a.add;
		return *this;
	}

	bool empty() const 
	{
		return mul == 1 && add == 0;
	}
};

info_t operator + (const info_t& a, const flag_t& b)
{
	if(!a.size) return a;

	info_t z;
	z.max = a.max * b.mul + b.add;
	z.min = a.min * b.mul + b.add;
	z.sum = a.sum * b.mul + b.add * a.size;
	z.size = a.size;
	return z;
}

struct node_t;
node_t* newnode();
void delnode(node_t *n);
struct node_t
{
	int val;
	node_t *fa, *ch[4];
	info_t chain, tree, all;
	flag_t chain_flag, tree_flag;
	bool is_rev, is_inner;

	bool is_root(int z) const
	{
		if(z == 0)
		{
			if(!fa) return true;
			return fa->ch[0] != this && fa->ch[1] != this;
		} else {
			return !fa || !fa->is_inner || !is_inner;
		}
	}

	void setc(node_t *c, int z)
	{
		ch[z] = c;
		if(c) c->fa = this;
	}

	int where() const
	{
		for(int i = 0; i != 4; ++i)
			if(fa->ch[i] == this) return i;
		return -1;  // throw 0;
	}

	int where(int z) const
	{
		return fa->ch[z] != this;
	}

	node_t* son(int i) 
	{
		if(ch[i]) ch[i]->pushdown();
		return ch[i];
	}

	void pushup()
	{
		chain = tree = all = info_t();
		if(!is_inner) chain = all = info_t(val, val, val, 1);
		for(int i = 0; i != 4; ++i) if(ch[i]) all += ch[i]->all;
		for(int i = 2; i != 4; ++i) if(ch[i]) tree += ch[i]->all;
		for(int i = 0; i != 2; ++i) 
		{
			if(ch[i])
			{
				chain += ch[i]->chain;
				tree += ch[i]->tree;
			}
		}
	}

	void rotate(int z)
	{
		node_t *p = fa;
		int d = where(z);
		if(!p->fa) fa = 0;
		else p->fa->setc(this, p->where());
		p->setc(ch[z + !d], z + d);
		setc(p, z + !d);
		p->pushup();
	}

	void push_chain(const flag_t& r)
	{
		chain = chain + r, chain_flag += r;
		all = chain, all += tree;
		val = val * r.mul + r.add;
	}

	void push_tree(const flag_t& r, int virt)
	{
		all = all + r;
		tree = tree + r;
		tree_flag += r;
		if(virt) push_chain(r);
	}

	void make_rev()
	{
		is_rev ^= 1;
		std::swap(ch[0], ch[1]);
	}

	void pushdown()
	{
		if(is_rev)
		{
			is_rev = false;
			std::swap(ch[0], ch[1]);
			if(ch[0]) ch[0]->is_rev ^= 1;
			if(ch[1]) ch[1]->is_rev ^= 1;
		}

		if(!tree_flag.empty())
		{
			for(int i = 0; i != 4; ++i)
				if(ch[i]) ch[i]->push_tree(tree_flag, i >> 1);
			tree_flag = flag_t();
		}

		if(!chain_flag.empty())
		{
			for(int i = 0; i != 2; ++i)
				if(ch[i]) ch[i]->push_chain(chain_flag);
			chain_flag = flag_t();
		}
	}

	void splay(int z)
	{
		for(; !is_root(z); rotate(z))
		{
			if(!fa->is_root(z))
			{
				if(where(z) == fa->where(z))
					fa->rotate(z);
				else rotate(z);
			}
		}
		
		pushup();
	}

	void add(node_t *w) 
	{
		w->pushdown();
		for(int i = 2; i != 4; ++i)
		{
			if(!w->ch[i])
			{
				w->setc(this, i);
				return;
			}
		}

		node_t *x = newnode(), *n = w;
		for(; n->ch[2]->is_inner; n = n->son(2));
		x->setc(n->ch[2], 2);
		x->setc(this, 3);
		n->setc(x, 2);
		x->splay(2);  
	}

	void del()
	{
		if(fa->is_inner)
		{
			fa->fa->setc(fa->ch[5 - where()], fa->where());
			fa->fa->splay(2);
			delnode(fa);
		} else fa->setc(0, where());
		fa = 0; 
	}

	void access()
	{
		static node_t *stack[MaxN];
		int st = 0;
		for(node_t *u = this; u; u = u->fa)
			stack[st++] = u;
		while(st) stack[--st]->pushdown();
		node_t *prev = 0, *now = this;
		while(now)
		{
			now->splay(0);
			if(now->ch[1]) now->ch[1]->add(now);
			if(prev) prev->del();
			now->setc(prev, 1);
			prev = now;
			now->pushup();
			for(now = now->fa; now && now->is_inner; now = now->fa);
		}

		splay(0);
	}

	void make_root()
	{
		access();
		is_rev ^= 1;
	}

	node_t *find_root() 
	{
		node_t *z = this;
		for(; z->fa; z = z->fa);
		return z;
	}

	node_t *find_parent()
	{
		access();
		node_t *z = son(0);
		while(z && z->ch[1]) z = z->son(1);
		return z;
	}
};

node_t* cut(node_t *u)
{
	node_t *v = u->find_parent();
	if(v) v->access(), u->del(), v->pushup();
	return v;
}

void link(node_t *u, node_t *v)
{
	node_t *p = cut(u);
	if(u->find_root() != v->find_root()) p = v;
	if(p) p->access(), u->add(p), p->pushup();
}

int node_used;
node_t node[MaxN];
std::stack<node_t*> garbages;
void delnode(node_t *n)
{
	garbages.push(n);
}

node_t* newnode() 
{
	node_t* z;
	if(!garbages.empty())
	{
		z = garbages.top();
		garbages.pop();
	} else z = node + node_used++;
	z->chain = z->all = info_t();
	z->chain_flag = z->tree_flag = flag_t();
	z->val = z->is_rev = 0;
	z->is_inner = 1; z->fa = 0;
	for(int i = 0; i != 4; ++i) z->ch[i] = 0;
	return z;
}

struct edge_t
{
	int u, v;
} e[MaxN];

int main()
{
	int n, m;
	std::scanf("%d %d", &n, &m);
	node_used = n + 1;
	for(int i = 1; i != n; ++i)
		std::scanf("%d %d", &e[i].u, &e[i].v);
	for(int i = 1; i <= n; ++i)
	{
		std::scanf("%d", &node[i].val);
		node[i].pushup();
	}

	for(int i = 1; i != n; ++i)
	{
		node[e[i].v].make_root();
		link(node + e[i].v, node + e[i].u);
	}

	int root;
	std::scanf("%d", &root);
	node[root].make_root();
	for(int i = 0; i != m; ++i)
	{
		int k, x;
		std::scanf("%d %d", &k, &x);
		node_t *u = node + x;
		if(k == 0 || k == 3 || k == 4 || k == 5 || k == 11)
		{
			u->access();
			if(k == 3 || k == 4 || k == 11)
			{
				int ans = u->val;
				for(int i = 2; i != 4; ++i)
				{
					if(u->ch[i])
					{
						info_t info = u->ch[i]->all;
						if(k == 3) ans = std::min(ans, info.min);
						else if(k == 4) ans = std::max(ans, info.max);
						else if(k == 11) ans += info.sum;
					}
				}

				std::printf("%d\n", ans);
			} else {
				int y;
				std::scanf("%d", &y);
				flag_t flag(k == 5, y);
				u->val = u->val * flag.mul + flag.add;
				for(int i = 2; i != 4; ++i)
					if(u->ch[i]) u->ch[i]->push_tree(flag, 1);
				u->pushup();
			}
		} else if(k == 9) {
			int v;
			std::scanf("%d", &v);
			link(u, node + v);
		} else if(k == 1) {
			u->make_root();
			root = x;
		} else {
			int y;
			std::scanf("%d", &y);
			u->make_root();
			node[y].access();
			u->splay(0);
			if(k == 7 || k == 8 || k == 10)
			{
				info_t ans = u->chain;
				if(k == 7) std::printf("%d\n", ans.min);
				else if(k == 8) std::printf("%d\n", ans.max);
				else std::printf("%d\n", ans.sum);
			} else {
				int v;
				std::scanf("%d", &v);
				u->push_chain(flag_t(k == 6, v));
			}

			node[root].make_root();
		}
	}
	return 0;
}
