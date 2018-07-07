import theano
import sys
import numpy
import collections

input_vector = theano.tensor.fvector('input_vector') 
target_values = theano.tensor.fvector('target_values') 

#This is just a template: it does not learn anything, and always returns Accuracy of 0:
updates = []
theano.config.on_unused_input='ignore'
predicted_values = theano.shared( numpy.zeros(5), 'predicted_class')
Accuracy = theano.shared(0, 'Accuracy')
#Change this to something meaningful and it will work!

train = theano.function([input_vector, target_values], [Accuracy, predicted_values], updates=updates, allow_input_downcast=True)
    #'allow_input_downcast=True' is needed to avoid any issues converting between 64 and 32 bit numbers

def read_dataset(path): #The function that reads the training data from a given file
    dataset = [] #starts with an empty container
    for line in open(path): #Read every line of the given file
            line_parts = line.split('\t') #split the line into the rating and the review text
            score = int(line_parts[0]) #very first number in the file is the review score
            review_text = line_parts[1]
            #create one-hot vector to represent the review score:
            if score == -2:
                score_vector = [1,0,0,0,0]
            if score == -1:
                score_vector = [0,1,0,0,0]
            if score == 0:
                score_vector = [0,0,1,0,0]
            if score == 1:
                score_vector = [0,0,0,1,0]
            if score == 2:
                score_vector = [0,0,0,0,1]
            #cteate the features: number of times the words "good" and "bad" happen in the review
            number_of_good = review_text.count("good")
            number_of_bad = review_text.count("bad")
            features = [number_of_good,number_of_bad]
            dataset.append((score_vector, features))
            print "added to data:", score_vector, features
    return dataset

data_train = read_dataset("movie-reviews-2.txt")
for epoch in xrange(1000):
        for score_vector, features in data_train:
            Accuracy, predicted_values = train(score_vector, features)
            if epoch % 100 == 0: #print progress uptate on every 100-th epoch
             print epoch, Accuracy, score_vector, predicted_values, features
