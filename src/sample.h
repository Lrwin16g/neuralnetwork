#ifndef _SAMPLE_H
#define _SAMPLE_H

struct Sample
{
    Sample();
    explicit Sample(int dimension);
    Sample(const Sample &obj);
    ~Sample();
    
    Sample& operator=(const Sample &obj);
    
    inline double operator[](int index) const {
	return feature[index];
    }
    
    double *feature;
    double category;
    int dim;
};

int loadSample(const char *filename, Sample *&sample);

#endif
