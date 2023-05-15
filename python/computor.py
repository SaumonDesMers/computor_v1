import math
import sys
eq = sys.argv[1]
terms = eq.translate({32: '', 43: '|+', 45: '|-', 61: '|'}).split('|')
coeff = [float(t[:t.find('*')]) for t in filter(len, terms)]
coeff[0] -= coeff.pop()
print('Reduced form: {}= 0'.format(f'{coeff[0]:g} ' + eq[eq.find('*'):eq.find('=')]))
print('Polynomial degree:', len(coeff) - 1)
if (len(coeff) == 1):
	print('All real numbers are solution.' if coeff[0] == 0 else 'There is no solution.')
elif (len(coeff) == 2):
	print(f'The solution is:\n{-coeff[0] / coeff[1]}')
elif (len(coeff) == 3):
	c, b, a = coeff
	D = b ** 2 - 4 * a * c
	if (D > 0):
		print(f'Discriminant is strictly positive, the solutions are:\n{(-b + math.sqrt(D)) / (2 * a)}\n{(-b - math.sqrt(D)) / (2 * a)}')
	elif (D == 0):
		print(f'Discriminant is 0, the solution is:\n{-b / (2 * a)}')
	else:
		print('Discriminant is strictly negative, there is no real solution.')
else:
	print('I can\'t solve polynomial degree greater than 2.')