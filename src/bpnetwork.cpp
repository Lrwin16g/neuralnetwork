#include "bpnetwork.h"

#include <iostream>
#include <fstream>
#include <cmath>
#include <ctime>
#include <limits>

#include "sample.h"
#include "mllib.h"



BPNetwork::BPNetwork()
    : dimension_(0), hiddenUnitNum_(0), categoryNum_(0),
      hiddenGain_(0.5), outputGain_(0.5),
      hiddenUnit_(NULL), hiddenWeight_(NULL),
      outputUnit_(NULL), outputWeight_(NULL)
{
    srand(time(NULL));
}

BPNetwork::BPNetwork(int dimension, int hiddenUnitNum, int categoryNum,
		     double hiddenGain, double outputGain)
    : dimension_(dimension), hiddenUnitNum_(hiddenUnitNum), categoryNum_(categoryNum),
      hiddenGain_(hiddenGain), outputGain_(outputGain),
      hiddenUnit_(NULL), hiddenWeight_(NULL),
      outputUnit_(NULL), outputWeight_(NULL)
{
    srand(time(NULL));
    initialize();
}

BPNetwork::~BPNetwork()
{
    cleanUp();
}

void BPNetwork::train(const Sample *sampleSet, int sampleNum, double trainCoef,
		      int maxLoop, double termCrit)
{
    Sample *trainSet = new Sample[sampleNum];
    for (int i = 0; i < sampleNum; ++i) {
	trainSet[i] = sampleSet[i];
    }
    
    double **criterion = new double*[sampleNum];
    for (int i = 0; i < sampleNum; ++i) {
	criterion[i] = new double[categoryNum_];
	for (int j = 0; j < categoryNum_; ++j) {
	    criterion[i][j] = 0.0;
	}
	int j = static_cast<int>(trainSet[i].category);
	criterion[i][j] = 1.0;
    }
    
    double *hiddenEpsilon = new double[hiddenUnitNum_];
    double *outputEpsilon = new double[categoryNum_];
    
    double rho = trainCoef;
    
    for (int loop = 0; loop < maxLoop; ++loop)
    {
	for (int i = 0; i < sampleNum; ++i)
	{
	    int index = rand() % sampleNum;
	    std::swap(trainSet[i], trainSet[index]);
	    std::swap(criterion[i], criterion[index]);
	}
	
	double sumLoss = 0.0;
	
	for (int i = 0; i < sampleNum; ++i)
	{
	    calcOutput(trainSet[i]);
	    
	    for (int j = 0; j < categoryNum_; ++j)
	    {
		sumLoss += pow(outputUnit_[j] - criterion[i][j], 2.0) / 2.0;
	    }
	    for (int j = 0; j < categoryNum_; ++j)
	    {
		outputEpsilon[j] = (outputUnit_[j] - criterion[i][j]) * outputGain_ * outputUnit_[j] * (1.0 - outputUnit_[j]);
	    }
	    for (int j = 0; j < hiddenUnitNum_; ++j)
	    {
		hiddenEpsilon[j] = 0.0;
		for (int k = 0; k < categoryNum_; ++k)
		{
		    hiddenEpsilon[j] += outputEpsilon[k] * outputWeight_[k][j];
		}
		hiddenEpsilon[j] *= hiddenUnit_[j] * hiddenGain_ * (1.0 - hiddenUnit_[j]);
	    }
	    for (int j = 0; j < categoryNum_; ++j)
	    {
		for (int k = 0; k < hiddenUnitNum_; ++k)
		{
		    outputWeight_[j][k] -= rho * outputEpsilon[j] * hiddenUnit_[k];
		}
	    }
	    for (int j = 0; j < hiddenUnitNum_; ++j)
	    {
		for (int k = 0; k < dimension_; ++k)
		{
		    hiddenWeight_[j][k] -= rho * hiddenEpsilon[j] * trainSet[i][k];
		}
	    }
	    
	    rho -= trainCoef / static_cast<double>(sampleNum * maxLoop);
	}
#ifndef _NO_PROGRESS
	if (loop % 100 == 0)
	{
	    std::cout << "Loop: " << loop << " Loss: " << sumLoss << std::endl;
	    saveParameter("tmp.prm");
	}
#endif
	if (sumLoss < termCrit)
	{
	    std::cout << "<Exit> Loop: " << loop << " Loss: " << sumLoss << std::endl;
	    break;
	}
    }
    
    delete[] hiddenEpsilon; hiddenEpsilon = NULL;
    delete[] outputEpsilon; outputEpsilon = NULL;
    for (int i = 0; i < sampleNum; ++i) {
	delete[] criterion[i];
    }
    delete[] criterion;	criterion = NULL;
    delete[] trainSet; trainSet = NULL;
}

bool BPNetwork::recognize(const Sample &sample)
{
    calcOutput(sample);
    int category = -1;
    double max = -std::numeric_limits<double>::max();
    for (int i = 0; i < categoryNum_; i++)
    {
	if (max < outputUnit_[i])
	{
	    max = outputUnit_[i];
	    category = i;
	}
    }
    if (category == static_cast<int>(sample.category))
    {
	return true;
    }
    else
    {
	return false;
    }
}

void BPNetwork::initParameter()
{
    for (int i = 0; i < hiddenUnitNum_; ++i)
    {
	for (int j = 0; j < dimension_; ++j)
	{
	    hiddenWeight_[i][j] = static_cast<double>(rand()) / RAND_MAX / 10.0 * pow(-1.0, rand() % 2);
	}
    }
    for (int i = 0; i < categoryNum_; ++i)
    {
	for (int j = 0; j < hiddenUnitNum_; ++j)
	{
	    outputWeight_[i][j] = static_cast<double>(rand()) / RAND_MAX / 10.0 * pow(-1.0, rand() % 2);
	}
    }
}

void BPNetwork::loadParameter(const char *filename)
{
    std::ifstream ifs(filename, std::ios::binary);
    if (ifs.is_open())
    {
	cleanUp();
	ifs.read(reinterpret_cast<char*>(&dimension_), sizeof(int));
	ifs.read(reinterpret_cast<char*>(&hiddenUnitNum_), sizeof(int));
	ifs.read(reinterpret_cast<char*>(&categoryNum_), sizeof(int));
	initialize();
	for (int i = 0; i < hiddenUnitNum_; ++i)
	{
	    for (int j = 0; j < dimension_; ++j)
	    {
		ifs.read(reinterpret_cast<char*>(&hiddenWeight_[i][j]), sizeof(double));
	    }
	}
	
	for (int i = 0; i < categoryNum_; ++i)
	{
	    for (int j = 0; j < hiddenUnitNum_; ++j)
	    {
		ifs.read(reinterpret_cast<char*>(&outputWeight_[i][j]), sizeof(double));
	    }
	}
	
	ifs.read(reinterpret_cast<char*>(&hiddenGain_), sizeof(double));
	ifs.read(reinterpret_cast<char*>(&outputGain_), sizeof(double));
	ifs.close();
    }
    else
    {
	std::cerr << "Error: File " << filename << " can't open." << std::endl;
    }
}

void BPNetwork::saveParameter(const char *filename)
{
    std::ofstream ofs(filename, std::ios::binary);
    if (ofs.is_open())
    {
	ofs.write(reinterpret_cast<char*>(&dimension_), sizeof(int));
	ofs.write(reinterpret_cast<char*>(&hiddenUnitNum_), sizeof(int));
	ofs.write(reinterpret_cast<char*>(&categoryNum_), sizeof(int));
	for (int i = 0; i < hiddenUnitNum_; ++i)
	{
	    for (int j = 0; j < dimension_; ++j)
	    {
		ofs.write(reinterpret_cast<char*>(&hiddenWeight_[i][j]), sizeof(double));
	    }
	}
	
	for (int i = 0; i < categoryNum_; ++i)
	{
	    for (int j = 0; j < hiddenUnitNum_; ++j)
	    {
		ofs.write(reinterpret_cast<char*>(&outputWeight_[i][j]), sizeof(double));
	    }
	}
	
	ofs.write(reinterpret_cast<char*>(&hiddenGain_), sizeof(double));
	ofs.write(reinterpret_cast<char*>(&outputGain_), sizeof(double));
	ofs.close();
    }
    else
    {
	std::cerr << "Error: File " << filename << " can't open." << std::endl;
    }
}

void BPNetwork::calcOutput(const Sample &sample)
{
    for (int i = 0; i < hiddenUnitNum_; ++i)
    {
	hiddenUnit_[i] = 0.0;
	for (int j = 0; j < dimension_; ++j)
	{
	    hiddenUnit_[i] += hiddenWeight_[i][j] * sample[j];
	}
	hiddenUnit_[i] = ml::sigmoid(hiddenUnit_[i], hiddenGain_);
    }
    for (int i = 0; i < categoryNum_; ++i)
    {
	outputUnit_[i] = 0.0;
	for (int j = 0; j < hiddenUnitNum_; ++j)
	{
	    outputUnit_[i] += outputWeight_[i][j] * hiddenUnit_[j];
	}
	outputUnit_[i] = ml::sigmoid(outputUnit_[i], outputGain_);
    }
}

void BPNetwork::initialize()
{
    hiddenWeight_ = new double*[hiddenUnitNum_];
    for (int i = 0; i < hiddenUnitNum_; ++i) {
	hiddenWeight_[i] = new double[dimension_];
    }
    outputWeight_ = new double*[categoryNum_];
    for (int i = 0; i < categoryNum_; ++i) {
	outputWeight_[i] = new double[hiddenUnitNum_];
    }
    initParameter();
    hiddenUnit_ = new double[hiddenUnitNum_];
    outputUnit_ = new double[categoryNum_];
}

void BPNetwork::cleanUp()
{
    for (int i = 0; i < hiddenUnitNum_; ++i) {
        delete[] hiddenWeight_[i];
    }
    delete[] hiddenWeight_;
    hiddenWeight_ = NULL;
    
    for (int i = 0; i < categoryNum_; ++i) {
        delete[] outputWeight_[i];
    }
    delete[] outputWeight_;
    outputWeight_ = NULL;
    
    delete[] hiddenUnit_;
    hiddenUnit_ = NULL;
    
    delete[] outputUnit_;
    outputUnit_ = NULL;
}
