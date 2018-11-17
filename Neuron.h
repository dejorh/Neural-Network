/*
 * Neuron.h
 *
 *  Created on: Nov 5, 2018
 *      Author: dcejorh42
 */

#ifndef NEURON_H_
#define NEURON_H_

#include <iostream>
#include <cmath>
#include <random>
#include <string>

using namespace std;


class Neuron{
private:
	int outputSize;
	double* weights;
	double nodeOut;
	double nodeOut_prime;
	double sigmoid(double x);
	double sigmoid_prime(double x);
	double leakyRelu(double x);
	double leakyRelu_prime(double x);
	double genWeight(double from, double to);
	double delta;
	string activate;
public:
	Neuron();
	Neuron(int outSize);
	~Neuron();
	void setWeight(int ind,double val);
	double getWeight(int ind);
	void setOutput(double val);
	double getOutput();
	double getOutput_prime();
	void calcOut(double sum);
	int getOutputSize();
	double getDelta();
	void setDelta(double val);
	void setActivate(string act);
};

Neuron::Neuron(){
	activate = "Sigmoid";
    outputSize = 1;
    weights = new double[outputSize];
    setWeight(0,1.0);
}

Neuron::Neuron(int outSize){
	activate = "Sigmoid";
	outputSize = outSize;
	weights = new double[outputSize];
	for(int i = 0; i < outputSize; ++i){
		weights[i] = genWeight(-0.1,0.1);
	}
}

Neuron::~Neuron(){
	delete [] weights;
}

double Neuron::sigmoid(double x){
	return (1/(1+(exp(-x))));
}

double Neuron::sigmoid_prime(double x){
	double g = sigmoid(x);
	return (g*(1-g));
}

double Neuron::leakyRelu(double x){
	if(x > 0){
		return x;
	}
	else{
		return (0.01*x);
	}
}

double Neuron::leakyRelu_prime(double x){
	if(x > 0){
		return 1;
	}
	else{
		return (0.01);
	}
}

void Neuron::setWeight(int ind,double val){
	weights[ind] = val;
}

double Neuron::getWeight(int ind){
	if(ind >= outputSize || ind < 0){
		cout << ind  << " >= " << outputSize << endl;
		cout << "bad index" << endl;
		return -1;
	}
	return weights[ind];
}

double Neuron::genWeight(double from, double to){
	random_device rng;
	mt19937 urng(rng());
	uniform_real_distribution<double> dist(from,to);
	return dist(urng);
}

void Neuron::setOutput(double val){
	nodeOut = val;
}

double Neuron::getOutput(){
	return nodeOut;
}

double Neuron::getOutput_prime(){
	return nodeOut_prime;
}

void Neuron::calcOut(double sum){
	nodeOut = sigmoid(sum);
	nodeOut_prime = sigmoid_prime(sum);
	if(activate == "Leaky Relu"){
		nodeOut = leakyRelu(sum);
		nodeOut_prime = leakyRelu_prime(sum);
	}
}

int Neuron::getOutputSize(){
	return outputSize;
}

double Neuron::getDelta(){
	return delta;
}

void Neuron::setDelta(double val){
	delta = val;
}

void Neuron::setActivate(string act){
	activate = act;
}
/*void Neuron::FeedFwd(double* inputArray){
	
}*/

#endif /* NEURON_H_ */