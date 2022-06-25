import solver

def f(x):
    return x**3 + x**2 - 1.23
def df(x):
    return 2*x**2 + 2*x

print(solver.bisection(f, 0, 1, 0.0001))
print(solver.chord(f, 0, 1, 0.0001))
print(solver.newton(f, df, 1, 0.0001))
