/* BZOJ-3217: ALOEXT
 *  替罪羊树套Trie树 */
#include <cstdio>
#include <algorithm>

const double alpha = 0.88;
const int inf = ~0u >> 1;
const int MaxH = 20, MaxL = MaxH - 1; 
const int ModV = 1048576, MaxN = 1000010;

struct trie_t
{
	int w;
	trie_t *son[2];
};

trie_t *trie_nil;

void trie_destroy(trie_t* n)
{
	if(n == trie_nil) return;
	trie_destroy(n->son[0]);
	trie_destroy(n->son[1]);
	delete n;
}

trie_t* trie_modify(trie_t* n, int l, int v, int d)
{
	if(n->w + d == 0)
	{
		trie_destroy(n);
		return trie_nil;
	}

	if(n == trie_nil)
		n = new trie_t(*trie_nil);
	n->w += d;
	if(l < 0) return n;
	int bit = (v >> l) & 1;
	n->son[bit] = trie_modify(n->son[bit], l - 1, v, d);
	return n;
}

int trie_kth(trie_t* n, int l, int k)
{
	if(l < 0) return 0;
	int sz = n->son[1]->w;
	if(k <= sz) return trie_kth(n->son[1], l - 1, k) | (1 << l);
	else return trie_kth(n->son[0], l - 1, k - sz);
}

int trie_xor(trie_t* n, int l, int x)
{
	if(l < 0) return 0;
	int bit = (x >> l) & 1;
	if(!n->son[!bit]->w) return trie_xor(n->son[bit], l - 1, x);
	else return trie_xor(n->son[!bit], l - 1, x) | (1 << l);
}

struct scape_t
{
	int val, size, max_size;
	scape_t *l, *r;
	trie_t *trie;
};

struct scape_second_t
{
	int a, b;

	scape_second_t() : a(-inf), b(-inf) {}

	void push(int v)
	{
		if(v >= a) b = a, a = v;
		else if(v > b) b = v;
	}
};

int data_len, data[MaxN];
scape_t *scape_nil, *root, *rebuild_node, *rebuild_fa;

void scape_destroy(scape_t* n)
{
	if(n == scape_nil) return;
	trie_destroy(n->trie);
	scape_destroy(n->l);
	if(n->val != -1)
		data[data_len++] = n->val;
	scape_destroy(n->r);
	delete n;
}

scape_t* scape_build(int l, int r)
{
	if(l > r) return scape_nil;
	scape_t* n = new scape_t(*scape_nil);

	if(l == r) 
	{
		n->val = data[l];
		n->size = n->max_size = 1;
		n->trie = trie_modify(trie_nil, MaxL, data[l], 1);
		return n;
	}

	int m = (l + r) >> 1;
	n->val = data[m];
	n->l = scape_build(l, m - 1);
	n->r = scape_build(m + 1, r);
	n->size = n->max_size = r - l + 1;
	for(int i = l; i <= r; ++i)
		n->trie = trie_modify(n->trie, MaxL, data[i], 1);
	return n;
}

scape_t* scape_rebuild(scape_t* n)
{
	data_len = 0;
	scape_destroy(n);
	return scape_build(0, data_len - 1);
}

int scape_modify(scape_t* n, int pos, int v)
{
	int exist = n->val == -1 ? 0 : 1;
	int old_val, sz = n->l->size;
	if(pos <= sz)
	{
		old_val = scape_modify(n->l, pos, v);
	} else if(sz + exist == pos) {
		old_val = n->val;
		n->val = v;
	} else {
		old_val = scape_modify(n->r, pos - sz - exist, v);
	}

	n->trie = trie_modify(n->trie, MaxL, v, 1);
	n->trie = trie_modify(n->trie, MaxL, old_val, -1);
	return old_val;
}

scape_t* scape_insert(scape_t* n, int pos, int v)
{
	if(n == scape_nil)
	{
		n = new scape_t(*scape_nil);
		n->val = v;
		n->size = n->max_size = 1;
		n->trie = trie_modify(trie_nil, MaxL, v, 1);
		return n;
	}

	int sz = n->l->size;
	int exist = n->val == -1 ? 0 : 1;
	if(pos <= sz) n->l = scape_insert(n->l, pos, v);
	else n->r = scape_insert(n->r, pos - sz - exist, v);
	n->trie = trie_modify(n->trie, MaxL, v, 1);
	n->size = n->l->size + n->r->size + exist;
	n->max_size = std::max(n->max_size, n->size);

	if(n->max_size * alpha < std::max(n->l->max_size, n->r->max_size))
		rebuild_node = n;
	if(n->l == rebuild_node || n->r == rebuild_node)
		rebuild_fa = n;

	return n;
}

int scape_delete(scape_t* n, int pos)
{
	int exist = n->val == -1 ? 0 : 1;
	int old_v, sz = n->l->size;
	if(pos <= sz)
	{
		old_v = scape_delete(n->l, pos);
	} else if(sz + exist == pos) {
		old_v = n->val;
		n->val = -1;
	} else {
		old_v = scape_delete(n->r, pos - sz - exist);
	}

	--n->size;
	n->trie = trie_modify(n->trie, MaxL, old_v, -1);
	return old_v;
}

void scape_second(scape_t* n, int l, int r, scape_second_t& s)
{
	if(l == 1 && r == n->size)
	{
		s.push(trie_kth(n->trie, MaxL, 1));
		if(n->trie->w >= 2)
			s.push(trie_kth(n->trie, MaxL, 2));
		return;
	}

	int sz = n->l->size;
	int exist = n->val == -1 ? 0 : 1;
	if(exist && l <= sz + 1 && sz + 1 <= r)
		s.push(n->val);
	if(l <= sz)
		scape_second(n->l, l, std::min(r, sz), s);
	if(r > sz + exist)
		scape_second(n->r, std::max(1, l - sz - exist), r - sz - exist, s);
}

int scape_query(scape_t* n, int l, int r, int x)
{
	if(l == 1 && r == n->size)
		return trie_xor(n->trie, MaxL, x);
	int sz = n->l->size, ans = -1;
	int exist = n->val == -1 ? 0 : 1;
	if(exist && l <= sz + 1 && sz + 1 <= r)
		ans = std::max(ans, x ^ n->val);
	if(l <= sz)
		ans = std::max(ans, scape_query(n->l, l, std::min(r, sz), x));
	if(r > sz + exist)
		ans = std::max(ans, scape_query(n->r, std::max(1, l - sz - exist), r - sz - exist, x));
	return ans;
}

void init()
{
	static trie_t trie_base;
	trie_nil = &trie_base;
	trie_nil->w = 0;
	trie_nil->son[0] = trie_nil->son[1] = trie_nil;

	static scape_t scape_base;
	scape_nil = &scape_base;
	scape_nil->trie = trie_nil;
	scape_nil->size = scape_nil->max_size = 0;
	scape_nil->l = scape_nil->r = scape_nil;
}

int main()
{
	init();
	int n, m;
	std::scanf("%d %d", &n, &m);
	for(int i = 0; i != n; ++i)
		std::scanf("%d", data + i);
	root = scape_build(0, n - 1);
	int lastans = 0;
	for(int i = 0; i != m; ++i)
	{
		char op[2];
		int x, y;
		scape_second_t s;
		std::scanf("%s", op);
		switch(*op)
		{
		case 'I':
			std::scanf("%d %d", &x, &y);
			x = (x + lastans) % root->size;
			y = (y + lastans) % ModV;
			rebuild_node = rebuild_fa = 0;
			root = scape_insert(root, x, y);
			if(rebuild_node)
			{
				if(!rebuild_fa) root = scape_rebuild(root);
				else if(rebuild_fa->l == rebuild_node)
					rebuild_fa->l = scape_rebuild(rebuild_node);
				else rebuild_fa->r = scape_rebuild(rebuild_node);
			}
			break;
		case 'D':
			std::scanf("%d", &x);
			x = (x + lastans) % root->size + 1;
			scape_delete(root, x);
			break;
		case 'C':
			std::scanf("%d %d", &x, &y);
			x = (x + lastans) % root->size + 1;
			y = (y + lastans) % ModV;
			scape_modify(root, x, y);
			break;
		case 'F':
			std::scanf("%d %d", &x, &y);
			x = (x + lastans) % root->size + 1;
			y = (y + lastans) % root->size + 1;
			scape_second(root, x, y, s);
			lastans = scape_query(root, x, y, s.b);
			std::printf("%d\n", lastans);
			break;
		}
	}

	scape_destroy(root);
	return 0;
}
