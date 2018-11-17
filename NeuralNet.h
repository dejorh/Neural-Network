/*
 * NeuralNet.h
 *
 *  Created on: Nov 5, 2018
 *      Author: dcejorh42
 */

#ifndef NEURALNET_H_
#define NEURALNET_H_

#include <iostream>
#include "Layer.h"
#include "Examples.h"
#include <ctime>

using namespace std;


class NeuralNet{
private:
	int netSize;
	Layer** netLayers;
	double alpha;
	int* layerSizes;
	int K_folds;
	double timeLimit; // in  seconds
	double* predicts;
	Examples* trainData;
	double accuracy;
	double error;
	void ForwardProp(int exInd);
	void ForwardProp(Examples* testSet, int index);
	void BackwardProp(int exInd);
	void kfolds_CV(Examples* Data);
public:
	NeuralNet(double learn_rate, int* NNlayersSizes, int networkSize);
	~NeuralNet();
	
	void setTimeLimit(double limit);
	void setTrainingData(Examples* trainSet);	
	void Train();
	void Test(Examples* testSet);
	void printResults();
	//void setKFolds(int folds);
	void CrossVal();
	double getAccuracy();
	double getError();
};

NeuralNet::NeuralNet(double learn_rate, int* NNlayersSizes, int networkSize){
	alpha = learn_rate;
	layerSizes = NNlayersSizes;
	netSize = networkSize;
	predicts = new double[layerSizes[netSize-1]];
	netLayers = new Layer*[netSize];
	trainData = NULL;
	accuracy = 0.0;
	for(int i = 1; i < netSize - 2; ++i){
		netLayers[i] = new Layer(layerSizes[i],(layerSizes[i+1]-1));
	}
	if(netSize > 2){
		netLayers[0] = new Layer(layerSizes[0],(layerSizes[1]-1));
		netLayers[netSize-2] = new Layer(layerSizes[netSize-2],layerSizes[netSize-1]);
	}else{
		netLayers[netSize-2] = new Layer(layerSizes[netSize-2],(layerSizes[netSize-1]));
	}
	netLayers[netSize-1] = new Layer(layerSizes[netSize-1]); //output layer
}

NeuralNet::~NeuralNet(){
	for(int i = 0; i < netSize; ++i){
		delete netLayers[i];
	}
	delete [] netLayers;
	delete [] layerSizes;
	delete [] predicts;
}


void NeuralNet::setTimeLimit(double limit){
	timeLimit = limit;
}

void NeuralNet::setTrainingData(Examples* trainSet){
	trainData = trainSet;
}

void NeuralNet::ForwardProp(int exInd){ //examples index
	double *inputs = trainData->getInSet_i(exInd);
	int L_size = netLayers[netSize-1]->getSize();
	
	netLayers[0]->setLayerOutputs(inputs);
	delete [] inputs;
	for(int i = 0; i < netSize - 2; ++i){ //looping from layer to layer
		for(int j = 0; j < layerSizes[i+1]-1; ++j){ //looping through each layer
			netLayers[i+1]->getNeuron(j+1)->calcOut(netLayers[i]->FeedForward(j));
		}
	}
	
	for(int k = 0; k < L_size; ++k){
		netLayers[netSize-1]->getNeuron(k)->calcOut(netLayers[netSize-2]->FeedForward(k));
		predicts[k] = netLayers[netSize-1]->getNeuron(k)->getOutput();
		//cout << predicts[k] << endl;
	}
}

void NeuralNet::BackwardProp(int exInd){
	double *outputs = trainData->getOutSet_i(exInd);
	int L_size = netLayers[netSize-1]->getSize(); //output layer size: Layer L
	double y_hat, out_prime; //out_prime is same as g'(z); y_hat is prediction
	double weight_jk, delta_j, delta_k;
	for(int i = 0; i < L_size; ++i){ //deltas for output layer Neurons
		out_prime = netLayers[netSize-1]->getNeuron(i)->getOutput_prime();
		y_hat = predicts[i];
		netLayers[netSize-1]->getNeuron(i)->setDelta(out_prime*(y_hat-outputs[i]));
	}
	delete [] outputs;
	for(int l = netSize-2; l >= 0; --l){ //looping through the layers in the network
		for(int j = 1; j < layerSizes[l]; ++j){ //looping through each current layer node
			double w_delta_sum = 0.0;
			for(int k = 0; k < layerSizes[l+1]; ++k){
				weight_jk = netLayers[l]->getNeuron(j)->getWeight(k);
				
				delta_k = netLayers[l+1]->getNeuron(k)->getDelta();
				if(l < (netSize-2)){
					delta_k = netLayers[l+1]->getNeuron(k+1)->getDelta();
				}	
				
				w_delta_sum += weight_jk*delta_k;
				//update the weight
				weight_jk -= alpha*delta_k*(netLayers[l]->getNeuron(j)->getOutput());
				netLayers[l]->getNeuron(j)->setWeight(k,weight_jk);
				if((l < (netSize-2)) && (k >= (layerSizes[l+1]-2)))
					break;
			}
			delta_j = w_delta_sum*netLayers[l]->getNeuron(j)->getOutput_prime();
			netLayers[l]->getNeuron(j)->setDelta(delta_j);
		}
		for(int k = 0; k < layerSizes[l+1]; ++k){ // update bias weights 
			weight_jk = netLayers[l]->getNeuron(0)->getWeight(k);
			delta_k = netLayers[l+1]->getNeuron(k)->getDelta();
			if(l < (netSize-2)){
				delta_k = netLayers[l+1]->getNeuron(k+1)->getDelta();
			}
			weight_jk -= alpha*delta_k;
			netLayers[l]->getNeuron(0)->setWeight(k,weight_jk);
			if((l < (netSize-2)) && (k >= (layerSizes[l+1]-2)))
				break;
		}
	}
}

void NeuralNet::Train(){
	clock_t start = clock();
	while(((clock() - start)/CLOCKS_PER_SEC) < timeLimit){	
		for(int i = 0; i < trainData->size(); ++i){
			ForwardProp(i);
			BackwardProp(i);
		}
	}	
}

void NeuralNet::ForwardProp(Examples* testSet, int index){
	double *inputs = testSet->getInSet_i(index);
	int L_size = netLayers[netSize-1]->getSize();
	
	netLayers[0]->setLayerOutputs(inputs);
	delete [] inputs;
	for(int i = 0; i < netSize - 2; ++i){ //looping from layer to layer
		for(int j = 0; j < layerSizes[i+1]-1; ++j){ //looping through each layer
			netLayers[i+1]->getNeuron(j+1)->calcOut(netLayers[i]->FeedForward(j));
		}
	}
	
	for(int k = 0; k < L_size; ++k){
		netLayers[netSize-1]->getNeuron(k)->calcOut(netLayers[netSize-2]->FeedForward(k));
		predicts[k] = netLayers[netSize-1]->getNeuron(k)->getOutput();
	}
}

void NeuralNet::Test(Examples* testSet){
	double ave, err_sum, run_sum = 0.0;
	double ave_err = 0.0;
	double out_size = (layerSizes[netSize-1]);
	double in_size = netLayers[0]->getSize()-1;
	for(int l = 0; l < in_size; ++l){
				testSet->setInBase_i(l,trainData->getInBase_i(l));
			}
	for(int l = 0; l < out_size; ++l){
				testSet->setOutBase_i(l,trainData->getOutBase_i(l));
	}
	double* outBases = trainData->getOutBases();
	for(int i = 0; i < testSet->size(); ++i){
		ForwardProp(testSet,i);
		cout << "Set #" << i+1 << ":\n" ;
		printResults();
		err_sum = 0.0;
		double *outs = testSet->getOutSet_i(i);
		for(int j = 0; j < layerSizes[netSize-1]; ++j){
			err_sum += abs((predicts[j] - outs[j])/(outs[j]));
			cout << "output(s): " << (outs[j]*(outBases[j])) << endl;
		}
		run_sum += err_sum;
		ave_err = (err_sum/(out_size))*100;
		ave = (100*(1 - (err_sum/(out_size))));
		cout << "Accuracy: " << ave << "%" << endl;
		cout << "Error: " << ave_err << "%" << endl;
		ave = 100*(1 - (run_sum/((i+1)*(out_size))));
		ave_err = (run_sum/((i+1)*(out_size)))*100;
		cout << "Accum Accuracy: " << ave << "%" << endl;
		cout << "Accum Error: " << ave_err << "%" << endl;
		delete [] outs;
	}
	error = ave_err;
	accuracy = ave;
}

void NeuralNet::printResults(){
	int outSize;
	double* outBases = trainData->getOutBases();
	double* inBases = trainData->getInBases();
	cout << "input(s): " << endl;
	for(int i = 1; i < netLayers[0]->getSize(); ++i){
		double in_norm = netLayers[0]->getNeuron(i)->getOutput();
		double inputVal = ((in_norm)*(inBases[i-1]))+(inBases[i-1]/2);
		cout << "Input #" << i << ": " << inputVal;
		cout << endl;
	}
	cout << endl << "prediction(s): " << endl;
	outSize = layerSizes[netSize-1];
	for(int j = 0; j < outSize; ++j){
		double b = outBases[j] ;
		cout << "Predict #" << j+1 << ": " << (predicts[j]*b) << endl;
	}
}

/*void NeuralNet::setKFolds(int folds){
	if(trainData != NULL){	
		if(trainData->set_k_folds(folds)){
			K_folds = folds;
		}
	}
}*/

void NeuralNet::kfolds_CV(Examples* Data){
	double *ins,*outs;
	int in_size, out_size,size;
	Data->randomize_data();
	//ins = Data->getInputs();
	//outs = Data->getOutputs();
	//Data->printOuts();
	in_size = Data->featSize();
	out_size = netLayers[netSize-1]->getSize();
	size = Data->size();
	double sum = 0.0;
	double err_sum = 0.0;
	//cout << size; cout << Data->get_k_folds();
	if((size/Data->get_k_folds()) >= 1){
		int part_size = size/Data->get_k_folds();
		int train_size = (part_size*Data->get_k_folds()) - part_size;
		for(int i = 0; i < Data->get_k_folds(); ++i){
			clock_t start = clock();
			while(((clock() - start)/CLOCKS_PER_SEC) < (timeLimit/Data->get_k_folds())){
				for(int j = 0; j < train_size; ++j){
					ForwardProp(j+part_size);
					BackwardProp(j+part_size);
				}
			}
			int sizes[] = {part_size,in_size,out_size};
			ins = Data->getInputs();
			outs = Data->getOutputs();
			Examples* test_data = new Examples(ins,outs,sizes);
			
			//for(int k = 0; k < part_size; ++k){
				Test(test_data);
			//}
			delete test_data;
			sum += accuracy;
			err_sum += error;
			Data->swap_parts(0,Data->get_k_folds()-(1+i));
		}
		accuracy = sum/Data->get_k_folds();
		error = err_sum/Data->get_k_folds();
	}else{
		cout << "Enter a valid value for k (between 1 and " << size << ") : " << endl;
		cin >> K_folds;
		Data->set_k_folds(K_folds);
	}
	//delete [] ins;
	//delete [] outs;
}

double NeuralNet::getAccuracy(){
	return accuracy;
}

double NeuralNet::getError(){
	return error;
}

void NeuralNet::CrossVal(){
	kfolds_CV(trainData);
	cout << "The accuracy for this Architecture is: " << accuracy << "%\n";
	cout << "The error for this Architecture is: " << error << "%\n\n"; 
}

#endif /* NEURALNET_H_ */