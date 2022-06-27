import solver
import math

# first function
def f1(x):
    return x**3 + x**2 - 1.23
def df1(x):
    return 2*x**2 + 2*x
def ddf1(x):
    return 4*x + 2

# second function
def f2(x):
    return 3*x**6 + 4*x**3 + x**2 - 0.3
def df2(x):
    return 18*x**5 + 12*x**2 + 2*x
def ddf2(x):
    return 90*x**4 + 24*x + 2

# third function
def f3(x):
    return math.tan(x)*x**2 - 1

def df3(x):
    return (x**2 + math.sin(2*x)*x)/math.cos(x)**2

def ddf3(x):
    return (4*x*math.cos(x)+math.sin(2*x)*math.cos(x)+2*x**2*math.sin(x))/(math.cos(x)**3)

# first function

print("\nx^3 + x^2 - 1.23 == 0:")
print("Bisection: ", solver.bisection(f1, 0, 1, 0.0001))
print("Chord: ", solver.chord(f1, 0, 1, 0.0001))
print("Newton", solver.newton(f1, df1, 1, 0.0001))
print("Muller", solver.muller(f1, 0, 0.5, 1.5, 0.0001))
print("Halley", solver.halley(f1, df1, ddf1, 1, 0.0001))

# second function

print("\n3*x^6 + 4*x^3 + x^2 - 0.3 == 0:")
print("Bisection: ", solver.bisection(f2, 0, 0.5, 0.0001))
print("Chord: ", solver.chord(f2, 0, 0.5, 0.0001))
print("Newton", solver.newton(f2, df2, 0.5, 0.0001))
print("Muller", solver.muller(f2, 0, 0.25, 0.5, 0.0001))
print("Halley", solver.halley(f2, df2, ddf2, 0.5, 0.0001))

# third function

print("\ntan(x)*x^2 - 1 == 0:")
print("Bisection: ", solver.bisection(f3, 0, 1.5, 0.0001))
print("Chord: ", solver.chord(f3, 0.5, 1.0, 0.0001))
print("Newton", solver.newton(f3, df3, 1.0, 0.0001))
print("Muller", solver.muller(f3, 0, 0.5, 1.0, 0.0001))
print("Halley", solver.halley(f3, df3, ddf3, 0.5, 0.0001))
