#ifndef _SAMPLE_H
#define _SAMPLE_H

struct Sample
{
    double *feature;
    int category;
    int dim;
    
    Sample(int dim);
    Sample(const Sample &obj);
    ~Sample();
    
    Sample& operator=(const Sample &obj) const;
    double& operator[](int index);
};

#endif
