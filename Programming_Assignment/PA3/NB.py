#Author: Hyunjae Kim
#Date: 05/27/2022

import sys
import os
import string
import re
import math

#training phase
f = open('trainingSet.txt', 'r')
#f = open('testSet.txt','r')

#Dictionaris for collecting Probability Data
entire_word = dict()

#Class counter for calculating P(class = 0) & P(Class = 1)
class_counter = dict()

#word counter for calculating P(X = v|Class = 0)& P(X = v|Class = 1)
word_counter_for_zero = dict()
word_counter_for_one = dict()

#Pre-Processing Step
#Process to Collect Probability Data
#Read the Entire traing file.
while True:
	#read line by line
	text = f.readline()
	if not text: break
	#Remove arbitrary punctuation
	text = re.sub(r'[^\w\s]','',text)
	
	#collect word by word in each line
	words = text.split()
	
	#for word in words:
	#	word = word.lower()
	#	if word == "0" or word == "1":
	class_counter[words[-1]] = class_counter.get(words[-1], 0)+1

	for i in range(len(words)-1):
		words[i] = words[i].lower()
		entire_word[words[i]] = entire_word.get(words[i],0)+1
		if words[-1] == "0":
			word_counter_for_zero[words[i]] = word_counter_for_zero.get(words[i],0)+1
		else:
			word_counter_for_one[words[i]] = word_counter_for_one.get(words[i],0)+1
	
f.close()

ptrain = open('preprocessed_train.txt','a')
ptest = open('preprocessed_test.txt', 'a')

for word in sorted(entire_word):
	ptrain.write(word)
	ptrain.write(", ")
	ptest.write(word)
	ptest.write(", ")
ptrain.write("\n")
ptest.write("\n")
train = open('trainingSet.txt','r')
test = open('testSet.txt','r')

while True:
	text = train.readline()
	if not text: break
	text = re.sub(r'[^\w\s]','',text)
	words = text.split()
	classLabel = words[-1]
	for i in range(len(words)):
		words[i] = words[i].lower()
	for word in sorted(entire_word):
		if word in words:
			ptrain.write("1, ")
		else:
			ptrain.write("0, ")
	ptrain.write(classLabel)
	ptrain.write("\n")
ptrain.close()

while True:
	text = test.readline()
	if not text: break
	text = re.sub(r'[^\w\s]','',text)
	words = text.split()
	classLabel = words[-1]
	for i in range(len(words)):
		words[i] = words[i].lower()
	for word in sorted(entire_word):
		if word in words:
			ptest.write("1, ")
		else:
			ptest.write("0, ")
	ptest.write(classLabel)
	ptest.write("\n")
ptest.close()


#Classifying Phase Starts

#Training Phase

#Probability Establishment phase
#Probability Dictionaries
class_probability = dict()

class_one_zero = dict()
class_one_one = dict()

class_zero_zero = dict()
class_zero_one = dict()

#Denominator for root probabilities
D = class_counter.get("0",0) + class_counter.get("1",0)

#Calculate probability for P(class = 0) and P(class = 1)
for word in class_counter:
	n = class_counter.get(word,0)
	p = float(n/D)
	q = math.log10(p)
	class_probability[word] = class_probability.get(word,0) + q

#For Class zeros
for word  in entire_word:
	if word in word_counter_for_zero:
		a = word_counter_for_zero.get(word,0)
		b = class_counter.get("0",0)

		n = float(a + 1)
		d = float(b+2)
		
		p = float(n/d)
		np = float(1-p)
		
		q = math.log10(p)
		nq = math.log10(np)
		
		class_zero_one[word] = class_zero_one.get(word, 0)+ q
		class_zero_zero[word] = class_zero_zero.get(word,0)+ nq

	else:
		b = class_counter.get("0",0)
		n = float(1)
		d = float(b + 2)
		p = float(n/d)
		np = float(1-p)
		q = math.log10(p)
		nq = math.log10(np)
		class_zero_one[word] = class_zero_one.get(word, 0) + q
		class_zero_zero[word] = class_zero_zero.get(word, 0) + nq

#For Class ones
for word in entire_word:
	if word in word_counter_for_one:
		a = word_counter_for_one.get(word,0)
		b = class_counter.get("1",0)

		n = float(a + 1)
		d = float(b + 2)
		
		p = float(n/d)


		np = float(1-p)
		q = math.log10(p)
		
		nq = math.log10(np)
		
		class_one_one[word] = class_one_one.get(word,0)+ q
		class_one_zero[word] = class_one_zero.get(word,0)+nq
	
	else:
		b = class_counter.get("1",0)
		n = float(1)
		d = float(b + 2)
		p = float(n/d)
		np = float(1-p)
		q = math.log10(p)
		nq = math.log10(np)
		class_one_one[word] = class_one_one.get(word,0)+ q
		class_one_zero[word] = class_one_zero.get(word,0)+ nq

#Testing Phase
#Counters for Acurracy testing
Counter = 0
Correct = 0

#Initial values for starting the loop iteration
zero = class_probability.get("0",0)
one = class_probability.get("1",0)

#Open the Testing file

output = open('results.txt','a')
output.write("Training Data: trainSet.txt, Testing Data: testSet.txt")
output.write("\n")
output.write("\n")
output.write("Testing on training data ('trainingSet.txt')")
output.write("\n")

#Test on the training file
f2 = open('trainingSet.txt','r')
#Iteration for the entire test file.
while True:	
	#Root Probability Values
	Pzero = zero
	Pone =  one

	#Read line by line
	text = f2.readline()
	if not text: break

	#Remove the arbitrary punctuations same as the training phase
	text = re.sub(r'[^\w\s]','',text)
	
	#split word by word
	words = text.split()
	
	#Convert all words in sentence into lower case.
	for i in range(len(words)):
		words[i] = words[i].lower()
	classLabel = words[-1]
	words =  words[:-1]

	#Summing each probabilities
	for w in entire_word:
		if w  in words:
			Pzero += class_zero_one.get(w,0)
			Pone += class_one_one.get(w,0)
		else:
			Pzero += class_zero_zero.get(w,0)
			Pone += class_one_zero.get(w,0)
	
	Counter += 1	
	
	#Predict the Class Label
	if Pone > Pzero:
		result = "1"
	else:
		result = "0"
	
	#Check if the prediction is correct
	if result == classLabel:
		Correct += 1
	output.write("Predicted: ")
	output.write(result)
	output.write(", Actual: ")
	output.write(classLabel)
	output.write("\n")
output.write("\n")
output.write("Correct: ")
output.write(str(Correct))
output.write(", Total Count: ")
output.write(str(Counter))
output.write("\n")
output.write("\n")
Rate = float(Correct/Counter)
output.write("Accuracy Rate: ")
output.write(str(round(Rate,2)))
output.write("\n")
#Need to print this on the output file. 
#After reading the Entrie test File
#Report the Accuracy


Counter = 0
Correct = 0
output.write("\n")
output.write("Testing on testing data ('testSet.txt')")
output.write("\n")
f3 = open('testSet.txt','r')
while True:	
	#Root Probability Values
	Pzero = zero
	Pone =  one

	#Read line by line
	text = f3.readline()
	if not text: break

	#Remove the arbitrary punctuations same as the training phase
	text = re.sub(r'[^\w\s]','',text)
	
	#split word by word
	words = text.split()
	
	#Convert all words in sentence into lower case.
	for i in range(len(words)):
		words[i] = words[i].lower()
	classLabel = words[-1]
	words =  words[:-1]

	#Summing each probabilities
	for w in entire_word:
		if w  in words:
			Pzero += class_zero_one.get(w,0)
			Pone += class_one_one.get(w,0)
		else:
			Pzero += class_zero_zero.get(w,0)
			Pone += class_one_zero.get(w,0)
	
	Counter += 1	
	
	#Predict the Class Label
	if Pone > Pzero:
		result = "1"
	else:
		result = "0"
	
	#Check if the prediction is correct
	if result == classLabel:
		Correct += 1
	output.write("Predicted: ")
	output.write(result)
	output.write(", Actual: ")
	output.write(classLabel)
	output.write("\n")

output.write("\n")
output.write("Correct: ")
output.write(str(Correct))
output.write(", Total Count: ")
output.write(str(Counter))
output.write("\n")

output.write("\n")
Rate = float(Correct/Counter)
output.write("Accuracy Rate: ")
output.write(str(round(Rate,2)))

output.close()
f2.close()
f3.close()
