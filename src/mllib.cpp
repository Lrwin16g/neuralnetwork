#include "mllib.h"

#include <cstdlib>
#include <cmath>

double ml::sigmoid(double value, double gain)
{
    return 1.0 / (1.0 + exp(-gain * value));
}

double ml::randu()
{
    return (static_cast<double>(rand()) + 1.0) / (static_cast<double>(RAND_MAX) + 2.0);
}

double ml::randu(double min, double max)
{
    double z = static_cast<double>(rand()) / static_cast<double>(RAND_MAX) * (max - min);
    return z + min;
}

double ml::randn(double mean, double stddev)
{
    double z = sqrt(-2.0 * log(ml::randu())) * sin(2.0 * M_PI * randu());
    return mean + stddev * z;
}
