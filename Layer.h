/*
 * Layer.h
 *
 *  Created on: Nov 5, 2018
 *      Author: dcejorh42
 */

#ifndef LAYER_H_
#define LAYER_H_

#include <iostream>
#include "Neuron.h"

using namespace std;


class Layer{
private:
	int layerSize;
	Neuron** nodeArray;
	
public:
	
	Layer(int nodeAmount, int nextlayerSize, string act_fn); //hidden layers
	Layer(int nodeAmount, int nextlayerSize); //input layer
	Layer(int nodeAmount); //output layer
	~Layer();
	int getSize();
	double FeedForward(int nextLayerNode);
	void setLayerOutputs(double* values); //really only for inputs layer
	Neuron* getNeuron(int index);
};

Layer::Layer(int nodeAmount, int nextlayerSize, string act_fn){
	layerSize = nodeAmount;
	nodeArray = new Neuron*[layerSize];
	while(act_fn != "Sigmoid" && act_fn != "Leaky Relu"){
		cout << "Invalid Function Name!\n" << "Enter 'Sigmoid' or 'Leaky Relu'\n"; 
		cin >> act_fn;
	}
	for(int i = 0; i < layerSize; ++i){
		nodeArray[i] = new Neuron(nextlayerSize);
		nodeArray[i]->setActivate(act_fn);
	}
	nodeArray[0]->setOutput(1.0);
}

Layer::Layer(int nodeAmount, int nextlayerSize){
	layerSize = nodeAmount;
	nodeArray = new Neuron*[layerSize];
	
	for(int i = 0; i < layerSize; ++i){
		nodeArray[i] = new Neuron(nextlayerSize);
	}
	nodeArray[0]->setOutput(1.0);
}

Layer::Layer(int nodeAmount){
	layerSize = nodeAmount;
	nodeArray = new Neuron*[layerSize];
	/*while(act_fn != "Sigmoid" || act_fn != "Leaky Relu"){
		cout << "Invalid Function Name!\n" << "Enter 'Sigmoid' or 'Leaky Relu'\n";
		cin >> act_fn; 
	}*/
	for(int i = 0; i < layerSize; ++i){
		nodeArray[i] = new Neuron();
		//nodeArray[i]->setActivate(act_fn);
	}
}

Layer::~Layer(){
	for(int i = 0; i < layerSize; ++i){
		delete nodeArray[i];
	}
	delete [] nodeArray;
}

int Layer::getSize(){
	return layerSize;
}

void Layer::setLayerOutputs(double* values){ 
	for(int i = 1; i < layerSize; ++i){
		nodeArray[i]->setOutput(values[i-1]);
	}
	
}

Neuron* Layer::getNeuron(int index){
	return nodeArray[index];
}

double Layer::FeedForward(int nextLayerNode){
	double sum = 0.0;
	int index = nextLayerNode;
	//sum += (nodeArray[0]->getOutput())*(nodeArray[0]->getWeights(index));
	for(int i = 0; i < layerSize; ++i){
		sum += (nodeArray[i]->getOutput())*(nodeArray[i]->getWeight(index));
	}
	return sum;
}
#endif /* LAYER_H_ */