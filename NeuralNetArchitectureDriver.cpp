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
	int num_hidden, hidden_size, k_folds;
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
	inFile >> k_folds;
	inFile >> num_hidden;	 //number of hidden layers
	inFile >> hidden_size;	 //nodes per layer
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
	dataFile >> numInputs; cout << numInputs << endl;
	dataFile.ignore();
	dataFile >> numOutputs;cout << numOutputs << endl;
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
		//cout << count << endl;
	}
	dataFile.close();
	
	double* inputs = new double[inList.size()];
	double* outputs = new double[outList.size()];
	for(int i = 0; i < inList.size(); ++i){
		inputs[i] = inList.get(i);
	}
	for(int i = 0; i < outList.size(); ++i){
		outputs[i] = outList.get(i);
	}
	int sizes[] = {count,numInputs,numOutputs};
	const int MAX_HIDDEN_LAYERS = num_hidden;
    const int MAX_HIDDEN_NODES = hidden_size;
    int NUM_NETWORKS = (MAX_HIDDEN_LAYERS*MAX_HIDDEN_NODES)+1;
	Examples* trainSet = new Examples(inputs,outputs,sizes);
	Examples* testData = new Examples(inputs,outputs,sizes);
	double accuracy = 0.0;
	double error = 3.4*pow(10.0,38.0);;
	int optimalArch[] = {0,0};
	trainSet->setOutBase_i(0,outBase);
	
	delete [] inputs;
	delete [] outputs;
	
    for(int n = 0; n < NUM_NETWORKS; ++n){
        if(n == 0){
            // WHAT IS BEING EVALUATED: HIDDEN LAYER ARCHITECTURE
            num_hidden = 0;
            hidden_size = 0; //
            // FOR PURPOSED OF OPTIMIZATION
        }else{
            num_hidden = 1 + ((n-1)/MAX_HIDDEN_NODES);
            hidden_size = 1 + ((n-1)%MAX_HIDDEN_NODES); //
        }
        int network_size = num_hidden+2;
        int* layersizes = new int[network_size];
        layersizes[0] = numInputs+1; // input layer has bias
        layersizes[network_size-1] = numOutputs; //outputs layer has no bias
        for(int j = 1; j < network_size-1; ++j){ //setting hidden-layer sizes
            layersizes[j] = hidden_size+1; //hidden layer have biases
        }
        NeuralNet *network = new NeuralNet(alpha,layersizes,network_size);
        //trainSet->setBase(outBase);
        network->setTrainingData(trainSet);
        cout << endl << "ARCHITECTURE: HIDDEN LAYERS<" << num_hidden << ">";
        cout << "   NODES_PER_LAYER<" << hidden_size << ">\n\n";
        trainSet->set_k_folds(k_folds);
		network->setTrainingData(trainSet);
		network->setTimeLimit(stop_time);
		network->CrossVal();
		if(error > network->getError()){
			accuracy = network->getAccuracy();
			error = network->getError();
			optimalArch[0] = num_hidden;
			optimalArch[1] = hidden_size;
		}
		delete network;
    }
	cout << "\nThe OPTIMAL ARCHITECTURE IS:  HIDDEN LAYERS<" << optimalArch[0] << ">";
	cout << "   NODES_PER_LAYER<" << optimalArch[1] << ">\n";
	cout << "OPTIMAL ARCHITECTURE ACCURACY: " << accuracy << "%" << endl;
	cout << "OPTIMAL ARCHITECTURE ERROR: " << error << "%" << endl;
	//network->Train();
	//network->Test(testData);
	delete trainSet;
	delete testData;
	//delete network;
	
return 0;	
}