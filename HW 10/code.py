from ulab import numpy as np # to get access to ulab numpy functions
import time
x = np.linspace(0, 10, num=1024)
y1 = np.sin(x)
y2 = np.sin(x/2)
y3 = np.sin(x/3)
y = y1+y2+y3

a,b = np.fft.fft(y)
for i in range(1024):
    time.sleep(.01)
    print("("+str(a[i])+",)")

# Want to know all the functions available in numpy? In REPL type np. and press Tab.
