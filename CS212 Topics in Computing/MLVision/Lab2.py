import numpy as np
import theano

# Task 1
print "Task 1:"

x = theano.tensor.fscalar('x')

z = 3 * x * x

derivative = theano.tensor.grad(z, [x])

differentiate = theano.function([x], derivative)

result = differentiate(1)

print result
assert result[0] == 6


# Task 2
print "Task 2"

x = theano.tensor.fscalar('x')
y = theano.tensor.fscalar('y')

z = -(x + y + 1)**2

derivative = theano.tensor.grad(z, [x, y])

differentiate = theano.function([x, y], derivative)

result = differentiate(0, 0)

print result
assert result[0] == -2 and result[1] == -2


# Task 3
print "Task 3"

StartingPoint = np.asanyarray([0.0, 0.0])
W = theano.shared(StartingPoint, 'W')
z = -(W[0] + W[1] + 1)**2

gradients = theano.tensor.grad(z, W)
W_updated = W + (0.1 * gradients[0])
list_of_updates = [(W, W_updated)]
f = theano.function([], [W, z], updates = list_of_updates)

for i in range(10):
    output = f()
    print output