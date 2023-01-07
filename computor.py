import math
import sys
import re

eq = sys.argv[1]
input = eq.replace(" ", "").replace("+", "|+").replace("-", "|-")
terms = re.split(r"\||\=", input)
while "" in terms: terms.remove("")

coeff = [float(t.split("*X^")[0]) for t in terms]
coeff[0] -= float(coeff[-1])
coeff.pop()

print("Reduced form: {}= 0".format("{:g} ".format(coeff[0]) + eq[eq.find("*"):eq.find("=")]))

print("Polynomial degree:", len(coeff) - 1)

if (len(coeff) == 1):
	print("There an infinit number of solution." if coeff[0] == 0 else "There is no solution.")
elif (len(coeff) == 2):
	print("The solution is:\n{}".format(-coeff[0] / coeff[1]))
elif (len(coeff) == 3):
	a = coeff[2]
	b = coeff[1]
	c = coeff[0]
	discriminant = b ** 2 - 4 * a * c
	if (discriminant > 0):
		print("Discriminant is strictly positive, the two solutions are:")
		print((-b + math.sqrt(discriminant)) / (2 * a))
		print((-b - math.sqrt(discriminant)) / (2 * a))
	elif (discriminant == 0):
		print("Discriminant is 0, the solution is:")
		print(-b / (2 * a))
	else:
		print("Discriminant is strictly negative, there is no solution in R.")
else:
	print("The polynomial degree is stricly greater than 2, I can't solve.")