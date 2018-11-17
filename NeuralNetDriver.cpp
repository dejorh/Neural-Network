#include <iostream>
#include <fstream>
#include "NeuralNet.h"
#include "List.h"

using namespace std;

const int NUM_ARGS = 2;

int main(int argc, char** argv){
	string in_filename,data_filename ;
	fstream inFile,dataFile;
	
	int numInputs, numOutputs, count = 0;
	int num_hidden, hidden_size;
	double alpha, err_tol; //error tolerance is in "fraction/or percentage of a minute"
	double outBase;
	double input, output;
	List<double> inList, outList;
	
	//input validation to ensure correct number of command line arguments are passed	
	if(argc != NUM_ARGS) 
	{
		cout << "You must enter "<< NUM_ARGS<< " arguments. ";
		cout << "There were only "<< argc <<" arguments entered.";
		cout << "\nEnter arguments In this order: ";
		cout << "ProgramName in_filename.txt\n"; //specify the correct input format
		return 0; //end program so user may attempt command line argument re-entry
	}
	else //assumes the the attempt to open the file was successful
	{
		in_filename = argv[1]; //second argument should be the input file name
	}
	
	inFile.open(in_filename); //file open attempt
	while(!inFile) //if attempt to open file fails...
	{
		cout << "Could not open file\n"; //indicate such to the user
		cout << "Please enter valid filename: "; //request a proper filename to be entered
		cin >> in_filename; //store again in the filename variable
		inFile.open(in_filename); //attempt to open the file once more
	}
	inFile >> data_filename; //datafile name
	inFile >> num_hidden;	 //number of hidden layers
	int *layer_arr = new int[num_hidden];
	for(int i = 0; i < num_hidden; ++i){
		inFile >> layer_arr[i];	 //nodes per layer
	}
	inFile >> alpha; 		 //learning rate
	inFile >> err_tol;		 //error tolerance:  stopping time(in secs) = err_tol*60
	double stop_time = err_tol*60; //
	inFile.close();
	
	dataFile.open(data_filename);
	while(!dataFile) //if attempt to open file fails...
	{
		cout << "Could not open file\n"; //indicate such to the user
		cout << "Please enter valid filename: "; //request a proper filename to be entered
		cin >> in_filename; //store again in the filename variable
		dataFile.open(data_filename); //attempt to open the file once more
	}
	dataFile >> outBase;
	dataFile >> numInputs;
	dataFile.ignore();
	dataFile >> numOutputs;
	dataFile.ignore();
	while(!dataFile.eof()){
		for(int i = 0; i < numInputs; ++i){
			if(!(dataFile >> input))
				break;
			dataFile.ignore();
			inList.append(input);
		}
		if(!dataFile)
			break;
		for(int i = 0; i < numOutputs; ++i){
			dataFile >> output;
			dataFile.ignore();
			outList.append(output);
		}
		count++;
	}
	dataFile.close();
	
	double* inputs = new double[inList.size()];
	double* outputs = new double[outList.size()];
	for(int i = 0; i < inList.size(); ++i){
		inputs[i] = inList.get(i);
		//if(i >= (count-2)*numInputs)
			//cout << inputs[i] << endl;
	}
	for(int i = 0; i < outList.size(); ++i){
		outputs[i] = outList.get(i);
	}
	//int div_ind = count*.9;
	//int sizes[] = {div_ind,numInputs,numOutputs};
	//int sizes2[] = {count-div_ind,numInputs,numOutputs};
	//int in_cv_ind = div_ind*numInputs;
	//int out_cv_ind = div_ind*numOutputs;
	int sizes[] = {count,numInputs,numOutputs};
	
	//cout << "then" << endl;
	Examples* trainSet = new Examples(inputs,outputs,sizes);
	trainSet->set_k_folds(10);
	//cout << " hmm" << endl;
	//Examples* testData = new Examples((inputs+in_cv_ind),(outputs+out_cv_ind),sizes2);
	/*if(trainSet->getBase() > testData->getBase()){
		testData->setBase(trainSet->getBase());
	}else{
		trainSet->setBase(testData->getBase());
	}*/
	//for(int i = 0; i < numInputs; ++i)
		//cout << inputs[in_cv_ind+i] << endl;
	delete [] inputs;
	delete [] outputs;
	trainSet->setOutBase_i(0,outBase);
	//testData->setBase(outBase);
	//cout << "why" << endl; 
	int network_size = num_hidden+2; 
	int *layersizes = new int[network_size];
	layersizes[0] = numInputs+1; // input layer has bias
	layersizes[network_size-1] = numOutputs; //outputs layer has no bias
	for(int j = 1; j < network_size-1; ++j){ //setting hidden-layer sizes
		layersizes[j] = layer_arr[j-1]+1; //hidden layer have biases
		cout << layersizes[j] << endl;
	}
	delete [] layer_arr;
	NeuralNet *network = new NeuralNet(alpha,layersizes,network_size);
	network->setTrainingData(trainSet);
	network->setTimeLimit(stop_time);
	//cout<< " what" << endl;
	//network->CrossVal();
	network->Train();
	network->Test(trainSet);
	//network->Test(testData);
	delete trainSet;
	//delete testData;
	delete network;
	
return 0;	
}