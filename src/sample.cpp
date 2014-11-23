#include "sample.h"

#include <cstdio>

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
