#!/usr/bin/python3

expr = input()
ans = eval(expr)

factor = expr.split('*')
for i in range(1, len(factor)):
	ans = max(ans, eval('(' + '*'.join(factor[0:i]) + ')*' + '*'.join(factor[i:])))
	ans = max(ans, eval('*'.join(factor[0:i]) + '*(' + '*'.join(factor[i:]) + ')'))
	for j in range(i + 1, len(factor)):
		ans = max(ans, eval('*'.join(factor[0:i]) + '*(' + '*'.join(factor[i:j]) + ')*' + '*'.join(factor[j:])))

print(ans)
