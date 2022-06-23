import solver

def f(x):
    return x**3 + x**2 - 1.23

print(solver.bisection(f, 0, 1, 0.0001))

