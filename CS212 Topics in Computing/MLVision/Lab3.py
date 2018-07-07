import theano
import sys
import numpy
import collections

# Task 9
print "Task 9.b)"
v1 = numpy.asarray([1, 2])
v2 = numpy.asarray([3, 4])
print numpy.dot(v1, v2)

print "Task 9.d)"
W = numpy.asarray([[1, 2], [3, 4], [5, 6]])
x = numpy.asarray([1, 1])
print numpy.dot(W, x)

# Task 10
print "Task 10"
texts = numpy.asarray([[1,1], [2,0], [1,0], [0,1], [0,1]])
scores = numpy.asarray([-2, 2, 0, 1, -1])

W = theano.shared(numpy.asarray([0.0, 0.0]), 'W')
predictions = theano.tensor.dot(texts, W)
Accuracy = -theano.tensor.sqr(predictions - scores).sum() + 10
gradients = theano.tensor.grad(Accuracy, [W])

W_updated = W + (0.1 * gradients[0])
updates = [(W, W_updated)]

f = theano.function([], Accuracy, updates=updates)

for i in range(10):
    output = f()
    print output

# Task 11
print "Task 11.a)"
input_vector = theano.tensor.fvector('input_vector')
target_values = theano.tensor.fvector('target_values')

W_initial_values = numpy.zeros((5, 2))
W = theano.shared(W_initial_values, 'W')
activations = theano.tensor.dot(W, input_vector)
predicted_values = theano.tensor.nnet.sigmoid(activations)
Accuracy = -theano.tensor.sqr(predicted_values - target_values).sum()
predicted_class = theano.tensor.argmax(predicted_values)
gradients = theano.tensor.grad(Accuracy, W)
updates = [(W, W + 0.1 * gradients)]

train = theano.function([input_vector, target_values],
                        [W, activations, predicted_values, predicted_class, Accuracy, gradients],
                        updates=updates, allow_input_downcast=True)

data_vector = [1.0, 0.0]
target_vector = [0, 0, 0, 0, 1]
_W, activations, predicted_values, predicted_class, Accuracy, gradients_W = train(data_vector, target_vector)
print "1st time:"
print _W, activations, predicted_values, predicted_class, Accuracy
print gradients_W
_W, activations, predicted_values, predicted_class, Accuracy, gradients_W = train(data_vector, target_vector)
print "2nd time:"
print _W, activations, predicted_values, predicted_class, Accuracy
print gradients_W

print "Task 11.b)"
data_vector = [0.0, 1.0]
target_vector = [1, 0, 0, 0, 0]
W.set_value(W_initial_values)
_W, activations, predicted_values, predicted_class, Accuracy, gradients_W = train(data_vector, target_vector)
print "1st time:"
print _W, activations, predicted_values, predicted_class, Accuracy
print gradients_W
_W, activations, predicted_values, predicted_class, Accuracy, gradients_W = train(data_vector, target_vector)
print "2nd time:"
print _W, activations, predicted_values, predicted_class, Accuracy
print gradients_W

print "Task 12"
#This is just a template: it does not learn anything, and always returns Accuracy of 0:
W_initial_values = numpy.zeros((3, 4))
W = theano.shared(W_initial_values, 'W')
theano.config.on_unused_input='ignore'
activations = theano.tensor.dot(W, input_vector)
predicted_values = theano.tensor.nnet.sigmoid(activations);
predicted_class = theano.tensor.argmax(predicted_values)
Accuracy = -theano.tensor.sqr(predicted_values - target_values).sum()
gradients = theano.tensor.grad(Accuracy, W)
updates = [(W, W + 0.1 * gradients)]
#Change this to something meaningful and it will work!

train = theano.function([input_vector, target_values], [Accuracy, predicted_values, predicted_class], updates=updates, allow_input_downcast=True)
    #'allow_input_downcast=True' is needed to avoid any issues converting between 64 and 32 bit numbers

def read_dataset(path): #The function that reads the training data from a given file
    dataset = [] #starts with an empty container
    for line in open(path): #Read every line of the given file
            line_parts = line.split('\t') #split the line into the rating and the review text
            score = int(line_parts[0]) #very first number in the file is the review score
            review_text = line_parts[1]
            #create one-hot vector to represent the review score:
            if score == -1:
                score_vector = [1, 0, 0]
            if score == 0:
                score_vector = [0, 1, 0]
            if score == 1:
                score_vector = [0, 0, 1]
            #cteate the features: number of times the words "good" and "bad" happen in the review
            number_of_super = review_text.count("super")
            number_of_good = review_text.count("good")
            number_of_bad = review_text.count("bad")
            number_of_terrible = review_text.count("terrible")
            features = [number_of_super, number_of_good, number_of_bad, number_of_terrible]
            dataset.append((score_vector, features))
            print "added to data:", score_vector, features
    return dataset

data_train = read_dataset("movie-reviews-4-3.txt")
for epoch in xrange(1000):
        for score_vector, features in data_train:
            Accuracy, predicted_values, predicted_class = train(features, score_vector)
            if epoch % 100 == 0: #print progress uptate on every 100-th epoch
             print epoch, Accuracy, score_vector, predicted_values, features, predicted_class
