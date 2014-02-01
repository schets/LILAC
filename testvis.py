import math
n = 10000
f = open("values.3D", "wt")
f.write("x y z value\n");
for i in range(n):
    t = float(i) / float(n-1)
    angle = t * (math.pi * 2.) * 50.
    r = t * 10.
    x = r * math.cos(angle)
    y = r * math.sin(angle)
    z = t * 10.
    value = math.sqrt(x*x + y*y + z*z)
    f.write("%g %g %g %g\n" % (x,y,z,value))
f.close()
