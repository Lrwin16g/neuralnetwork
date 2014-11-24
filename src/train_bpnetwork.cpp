#include "bpnetwork.h"
#include "sample.h"

#include <iostream>

int main(int argc, char *argv[])
{
    Sample *sampleset = NULL;
    int sampleNum = loadSample("data/iris.dat", sampleset);
#ifdef _DEBUG
    for (int i = 0; i < sampleNum; ++i) {
	for (int j = 0; j < sampleset[i].dim; ++j) {
	    std::cout << sampleset[i][j] << " ";
	}
	std::cout << sampleset[i].category << std::endl;
    }
#endif
    
    BPNetwork model(sampleset[0].dim, 5, 3);
    model.train(sampleset, sampleNum, 0.1, 10000000, 0.001);
    
    /*BPNetwork model;
    model.loadParameter("tmp.prm");
    
    int correctCnt = 0;
    for (int i = 0; i < sampleNum; ++i)
    {
	if (model.recognize(sampleset[i]))
	{
	    correctCnt++;
	}
    }
    std::cout << static_cast<double>(correctCnt) / sampleNum << std::endl;*/
    
    delete[] sampleset; sampleset = NULL;
    
    return 0;
}
