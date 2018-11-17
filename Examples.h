/*
 * Examples.h
 *
 *  Created on: Nov 5, 2018
 *      Author: dcejorh42
 */

#ifndef EXAMPLES_H_
#define EXAMPLES_H_

#include <iostream>
#include <cmath>
#include <random>

using namespace std;


class Examples{
private:
	double *inputs, *outputs;
	int numPairs;
	int featureSize;
	int numOuts;
	double *outBases;
	double *inBases;
	double *in_scales;
	double *out_scales;
	int k_folds;
	int* genRandArray(int n);
	void randomize(int* &arr,int arr_size);
	double out_scaler(int set_num, int out_num);
public:
	Examples(double* ins, double* outs, int* sizes);
	~Examples();
	double* getInputs();
	double* getOutputs();
	double* getInSet_i(int exInd);
	double* getOutSet_i(int exInd);
	//double* getIns();
	//double* getOuts();
	//double* getInBin_i(int k_ind);
	//double* getOutBin_i(int k_ind);
	int size();
	int featSize();
	//void norm(double base);
	double getInBase_i(int ind);
	double getOutBase_i(int ind);
	void setInBase_i(int ind,double b);
	void setOutBase_i(int ind,double b);
	double* getInBases();
	double* getOutBases();
	//void setBase(double b);
	void randomize_data();
	int get_k_folds();
	bool set_k_folds(int k);
	bool swap_parts(int to_bin, int from_bin);
	void printOuts();
};

Examples::Examples(double* ins, double* outs, int* sizes){
	numPairs = sizes[0];
	featureSize = sizes[1];
	numOuts = sizes[2];
	inBases = new double[featureSize];
	outBases = new double[numOuts];
	inputs = new double[featureSize*numPairs];
	//in_scales = new double[featureSize*numPairs];
	outputs = new double[numOuts*numPairs];
	out_scales = new double[numOuts*numPairs];
	k_folds = numPairs;
	//base = 0.0;
	for(int i = 0; i < featureSize; ++i){
		inBases[i] = 0.0;
	}
	for(int j = 0; j < numOuts; ++j){
		outBases[j] = 0.0;
	}
	for(int i = 0; i < numPairs; ++i){
		for(int j = 0; j < featureSize; ++j){
			inputs[i*featureSize+j] = ins[i*featureSize+j];
			if(inBases[j] < abs(ins[i*featureSize+j])){
				inBases[j] = ins[i*featureSize+j];
			}
		}
		for(int k = 0; k < numOuts; ++k){
			outputs[i*numOuts+k] = outs[i*numOuts+k];
			out_scales[i*numOuts+k] = 1.0;
			if(outBases[k] < abs(outs[i*numOuts+k])){
				outBases[k] = outs[i*numOuts+k];
			}
		}
	}
	for(int i = 0; i < featureSize; ++i){
		inBases[i] += 0.1*inBases[i];
		
	}
	for(int j = 0; j < numOuts; ++j){
		outBases[j] += 0.1*outBases[j];
		for(int k = 0; k < numPairs; ++k){	
			double scaler = 1.0;
			while((outBases[j]/(scaler*outputs[j+(numOuts*k)])) > 100){
				scaler *= 10;
			}
			out_scales[j+(numOuts*k)] = scaler;
		}
	}
}

Examples::~Examples(){
	delete [] inputs;
	delete [] outputs;
	delete [] inBases;
	delete [] outBases;
	delete [] out_scales;
}

double* Examples::getInputs(){
	return inputs;
}

double* Examples::getOutputs(){
	return outputs;
}

double Examples::out_scaler(int set_num, int out_num){
	return (out_scales[out_num+set_num*numOuts]);
}

double* Examples::getInSet_i(int exInd){
	double* ins = new double[featureSize];
	for(int i = 0; i < featureSize; ++i){
		ins[i] = inputs[featureSize*exInd+i];
		//ins[i] /= inBases[i];
		ins[i] = (ins[i]-(inBases[i]/2))/inBases[i];
	}
	//double* ins = inputs+(featureSize*exInd);
	return ins;
}

double* Examples::getOutSet_i(int exInd){
	double* outs = new double[numOuts];
	for(int i = 0; i < numOuts; ++i){
		double out_scale = out_scaler(exInd,i);
		outs[i] = outputs[numOuts*exInd+i];
		//outs[i] = (outs[i]-(outBases[i]/2))/outBases[i];
		outs[i] /= outBases[i];
		//outs[i] *= out_scale;
	}
	/*double* outs = outputs+(numOuts*exInd);
	for(int i = 0; i < numOuts; ++i){
		outs[i] /= base;
	}*/
	return outs;
}

void Examples::setInBase_i(int ind,double b){
	if(ind >= featureSize || ind < 0){
		cout << "bad index" << endl;
		return;
	}
	inBases[ind] = b;
}

void Examples::setOutBase_i(int ind,double b){
	if(ind >= numOuts || ind < 0){
		cout << "bad index" << endl;
		return;
	}
	outBases[ind] = b;
}

/*double* Examples::getIns(){
	double* ins = new double[featureSize*numPairs];
	for(int i = 0; i < featureSize*numPairs; ++i){
		ins[i] = inputs[i];
	}
	return ins;
}

double* getOuts(){
	double* outs = new double[numOuts*numPairs];
	for(int i = 0; i < numOuts*numPairs; ++i){
		outs[i] = outputs[i];
		outs[i] /= base;
	}
	return outs;
}*/

/*double* getInBin_i(int k_ind){
	
}

double* getOutBin_i(int k_ind){
	
}*/


int Examples::size(){
	return numPairs;
}

int Examples::featSize(){
	return featureSize;
}

void Examples::randomize(int* &arr,int arr_size){
	random_device rng;
	mt19937 urng(rng());
	shuffle ( arr, arr+arr_size-1, urng);
}

int* Examples::genRandArray(int n){
	int* arr = new int[n];
	for(int i = 0; i < n; ++i){
		arr[i] = i;
	}
	randomize(arr,n);
	return arr;
}

/*void Examples::norm(int index){
	for(int i = 0; i < numOuts; ++i){
		
	}
}*/

double Examples::getInBase_i(int ind){
	return inBases[ind];
}

double Examples::getOutBase_i(int ind){
	return outBases[ind];
}

double* Examples::getInBases(){
	return inBases;
}

double* Examples::getOutBases(){
	return outBases;
}

/*void Examples::setBase(double b){
	base = b;
}*/

void Examples::randomize_data(){
	int *ind_arr = genRandArray(numPairs);
	for(int i = 0; i < numPairs; ++i){
		for(int j = 0; j < featureSize; ++j){
			double temp = inputs[j+(i*featureSize)];
			inputs[j+(i*featureSize)] = inputs[j+(ind_arr[i]*featureSize)];
			inputs[j+(ind_arr[i]*featureSize)] = temp;
		}
		for(int j = 0; j < numOuts; ++j){
			double temp = outputs[j+(i*numOuts)];
			outputs[j+(i*numOuts)] = outputs[j+(ind_arr[i]*numOuts)];
			outputs[j+(ind_arr[i]*numOuts)] = temp;
		}
	}
	delete [] ind_arr;
}

int Examples::get_k_folds(){
	return k_folds;
}

bool Examples::set_k_folds(int k){
	if((k > numPairs) || (k < 1)){
		return false;
	}
	k_folds = k;
	return true;
}

bool Examples::swap_parts(int to_bin, int from_bin){
	if((from_bin >= k_folds) || (to_bin >= k_folds)){
		return false;
	}
	if((from_bin < 0) || (to_bin < 0)){
		return false;
	}
	int part_size = numPairs/k_folds;
	for(int i = 0; i < part_size*featureSize; ++i){
		double temp = inputs[i+(to_bin*featureSize*part_size)];
		inputs[i+(to_bin*featureSize*part_size)] = inputs[i+(from_bin*featureSize*part_size)];
		inputs[i+(from_bin*featureSize*part_size)] = temp;
	}
	for(int i = 0; i < part_size*numOuts; ++i){
		double temp = outputs[i+(to_bin*numOuts*part_size)];
		outputs[i+(to_bin*numOuts*part_size)] = outputs[i+(from_bin*numOuts*part_size)];
		outputs[i+(from_bin*numOuts*part_size)] = temp;
	}
	return true;
}

void Examples::printOuts(){
	for(int i = 0; i < numOuts*size(); ++i){
		cout << "Total Outputs: " << outputs[i] << endl;
	}
	cout << "\n\n";
}

#endif /* EXAMPLES_H_ */