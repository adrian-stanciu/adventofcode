import z3

x = []
y = []
z = []
vx = []
vy = []
vz = []

for i in range(3):
    line = input()

    pos, vel = line.split('@')

    xi, yi, zi = pos.split(',')
    x.append(int(xi))
    y.append(int(yi))
    z.append(int(zi))

    vxi, vyi, vzi = vel.split(',')
    vx.append(int(vxi))
    vy.append(int(vyi))
    vz.append(int(vzi))

p = z3.RealVector('p', 3)
v = z3.RealVector('v', 3)
t = z3.RealVector('t', 3)

solver = z3.Solver()

for i in range(3):
    solver.add(p[0] + v[0] * t[i] == x[i] + vx[i] * t[i])
    solver.add(p[1] + v[1] * t[i] == y[i] + vy[i] * t[i])
    solver.add(p[2] + v[2] * t[i] == z[i] + vz[i] * t[i])

solver.check()

print(solver.model().eval(sum(p)))

