#ifndef _MLUTILITY_H
#define _MLUTILITY_H

namespace ml
{    
    double sigmoid(double value, double gain);
    
    double randu();
    
    double randu(double min, double max);
    
    double randn(double mean, double stddev);
};

#endif
