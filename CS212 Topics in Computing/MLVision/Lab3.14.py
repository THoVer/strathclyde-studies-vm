import theano
import sys
import numpy
import collections

input_vector = theano.tensor.fvector('input_vector') #theano variable representing image
target_values = theano.tensor.fvector('target_values') #theano variable representing the label of that image

#This is just a template: it does not learn anything, and always returns the digit "0":
W_initial_values = numpy.zeros((10, 784))
W1 = theano.shared(W_initial_values, 'W1')
params = []
theano.config.on_unused_input='ignore'
activations = theano.tensor.dot(W1, input_vector)
predicted_values = theano.tensor.nnet.sigmoid(activations);
predicted_class = theano.tensor.argmax(predicted_values)
Accuracy = -theano.tensor.sqr(predicted_values - target_values).sum()
gradient = theano.tensor.grad(Accuracy, W1)
updates = [(W1, W1 + 0.05 * gradient)]
#Change this to something meaningful and it will work!

# defining Theano functions for training and testing the model:
train = theano.function([input_vector, target_values], [Accuracy, predicted_class], updates=updates, allow_input_downcast=True)
test = theano.function([input_vector, target_values], [Accuracy, predicted_class], allow_input_downcast=True)
    #'allow_input_downcast=True' is needed to avoid any issues converting between 64 and 32 bit numbers

def read_dataset(path): #The function that reads training or testing data and returns it as an array of data points
    number_of_images = len(open(path).readlines()) / 28 #28 lines per each image
    f = open(path)
    dataset = [] #starts with an empty container
    for i in range(number_of_images):
        data_vector = [] #we start with empty data vector, and then we read the data for each image from the 28 lines that it takes:
        for l in range(28): #each image takes 28 lines
            line = f.readline()
            line_parts = line.split() #split the line into all the numbers
            assert(len(line_parts) == 29) #should be total of 29: the label + 28 numbers for the image
            label = int(line_parts[0]) #very first number in the file is the label (the digit that the picture represents)
            assert (0 <= label <= 9) #only digits 0-9 are allowed as labels
            #now, we will create "one-hot vector":
            label_vector = [0,0,0,0,0,0,0,0,0,0] #a vector of 10 zeroes
            label_vector[label] = 1 #except 1 for the label
            data_vector += [float(line_parts[i])/255. for i in xrange(1, len(line_parts))]
            #we divide by 255 so the pixel brightness is represented by a number between 0 and 1

        dataset.append((label_vector, data_vector))
    return dataset


#reading the data:
data_test = read_dataset("test.txt")
data_train = read_dataset("train.txt")

iteration = 0
decreaseCnt = 0
train_test_ratio = 0.95
last_test_accuracy = 0;
last_train_accuracy = 0;
runTraining = True

# while (runTraining):
#     currentPos = 0 # iteration * (1 - train_test_ratio) * len(data_train)
#     cost_sum = 0.0
#     correct = 0
#     # train
#     for i in xrange(int(len(data_train) * train_test_ratio)):
#         idx = int(currentPos % len(data_train))
#         Accuracy, predicted_class = train(data_train[idx][1], data_train[idx][0])
#         cost_sum += Accuracy
#         if (data_train[idx][0][predicted_class] == 1):
#             correct += 1
#         currentPos += 1
#     train_accuracy = cost_sum
#     train_correct = correct
#
#     cost_sum = 0.0
#     correct = 0
#     # test
#     for i in xrange(int(len(data_train) * (1 - train_test_ratio))):
#         idx = int(currentPos % len(data_train))
#         Accuracy, predicted_class = test(data_train[idx][1], data_train[idx][0])
#         cost_sum += Accuracy
#         if (data_train[idx][0][predicted_class] == 1):
#             correct += 1
#         currentPos += 1
#     test_accuracy = cost_sum;
#     test_correct = correct
#
#     print "Iteration: " + str(iteration) + ", Train Accuracy: " + str(train_accuracy) + ", Train correct: " + str(float(train_correct) / (train_test_ratio * len(data_train))*100) + '%', "Test Accuracy: " + str(test_accuracy) + ", Test correct: " + str(float(test_correct) / ((1-train_test_ratio) * len(data_train))*100) + '%'
#     if (test_correct < last_test_accuracy and iteration > 10):
#         decreaseCnt += 1
#         if (decreaseCnt > 1):
#             runTraining = False
#     else:
#         decreaseCnt = 0
#     last_test_accuracy = test_correct
#     iteration += 1

# training
for epoch in xrange(25):
    cost_sum = 0.0
    correct = 0
    for labelv, vector in data_train:
        Accuracy, predicted_class = train(vector, labelv)
        cost_sum += Accuracy
        if (labelv[predicted_class] == 1):
            correct += 1
    print "Epoch: " + str(epoch) + ", Accuracy: " + str(cost_sum) + ", correct: " + str(float(correct) / len(data_train)*100) + '%'

# testing:
cost_sum = 0.0
correct = 0
for labelv, vector in data_test:
    Accuracy, predicted_class = test(vector, labelv)
    cost_sum += Accuracy
    if (labelv[predicted_class] == 1):
        correct += 1
print "\tcorrect on the test set: " + str(float(correct) / len(data_test)*100) + '%'

# anonymous testing:
print "Numbers from unlabeled dataset"
data_test = read_dataset("test-anonymous.txt")
for labelv, vector in data_test[:10]:
    Accuracy, predicted_class = test(vector, labelv)
    print predicted_class