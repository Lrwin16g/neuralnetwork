#include "sample.h"

#include "filelib.h"

Sample::Sample()
    : feature(NULL), category(0.0), dim(0)
{
}

Sample::Sample(int dimension)
    : feature(NULL), category(0.0), dim(dimension)
{
    feature = new double[dim];
    for (int i = 0; i < dim; ++i) {
	feature[i] = 0.0;
    }
}

Sample::Sample(const Sample &obj)
    : feature(NULL)
{
    category = obj.category;
    dim = obj.dim;
    feature = new double[dim];
    for (int i = 0; i < dim; ++i) {
	feature[i] = obj[i];
    }
}

Sample::~Sample()
{
    delete[] feature;
    feature = NULL;
}

Sample& Sample::operator=(const Sample &obj)
{
    if (this != &obj)
    {
	delete[] feature;
	dim = obj.dim;
	feature = new double[dim];
	for (int i = 0; i < dim; ++i) {
	    feature[i] = obj[i];
	}
	category = obj.category;
    }
    return *this;
}

int loadSample(const char *filename, Sample *&sample)
{
    std::vector<std::vector<double> > data = file::loadfile<double>(filename, " ", false);
#ifdef _DEBUG
    for (int i = 0; i < data.size(); ++i) {
	for (int j = 0; j < data[i].size(); ++j) {
	    std::cout << data[i][j] << " ";
	}
	std::cout << std::endl;
    }
#endif
    
    sample = new Sample[data.size()];
    for (size_t i = 0; i < data.size(); ++i)
    {
	sample[i] = Sample(data[i].size() - 1);
	for (size_t j = 0; j < data[i].size() - 1; ++j)
	{
	    sample[i].feature[j] = data[i][j];
	}
	sample[i].category = data[i][data[i].size() - 1] - 1.0;
    }
    
    return data.size();
}
