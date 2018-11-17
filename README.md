# Neural-Network
C++ Neural Network made from scratch
In order to compile, run with command line input file specified.
Example #1(for NeuralNetDriver.cpp): ./NNDriver inputfile.txt
Where the inputfile.txt is a text file that has the following format:
  - data file name (in the same format as the data files given below)
  - number of hidden layers
  - number of nodes in each hidden layer
  - learning rate
  - error tolerance (use to determine the allowed time in minutes)
**********************************************************************
Datafiles are of the following format:
Line #1: output Base (for normalization)
Line #2: #of inputs,#of outputs (make sure comma separated) 
Rest of File: Input1,Input2,Input3,.....InputN,Output1,Output2,Output3,....OutputN
***********************************************************************************

Example #2(for NeuralNetArchitectureDriver.cpp): ./NNArchDriver inputfile2.txt
Where the inputfile2.txt is a text file that has the following format:
  - data file name (in the same format as the data files given above)
  - number of cross validation bins (k)
  - maximum number of hidden layers
  - maximum number of nodes in a given hidden layer
  - learning rate
  - error tolerance (use to determine the allowed time in minutes)
