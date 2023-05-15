import math
import sys
l=len
p=print
e=sys.argv[1]
c=[float(t[:t.find('*')])for t in filter(l,e.translate({32:'',43:'|+',45:'|-',61:'|'}).split('|'))]
c[0]-=c.pop()
p('Reduced form: {}= 0'.format(f'{c[0]:g} '+e[e.find('*'):e.find('=')]))
p('Polynomial degree:',l(c)-1)
if(l(c)==1):
	p('Infinit solutions.'if c[0]==0 else'No solution.')
elif(l(c)==2):
	p(f'The solution is:\n{-c[0]/c[1]}')
elif(l(c)==3):
	c,b,a=c
	D=b**2-4*a*c
	if(D>0):p(f'Discriminant > 0:\n{(-b+math.sqrt(D))/(2*a)}\n{(-b-math.sqrt(D))/(2*a)}')
	elif(D==0):p(f'Discriminant = 0:\n{-b/(2*a)}')
	else:p('Discriminant < 0, no solution.')
else:p('I can\'t solve this one.')