#ifndef _BPNETWORK_H
#define _BPNETWORK_H

class Sample;

class BPNetwork
{
public:
    BPNetwork();
    BPNetwork(int dimension, int hiddenUnitNum, int categoryNum,
	      double hiddenGain, double outputGain);
    ~BPNetwork();
    
    void train(const Sample *sampleSet, int sampleNum, double trainCoef, int maxLoop, double termCrit);
    bool recognize(const Sample &sample);
    void initParameter();
    
    void loadParameter(const char *filename);
    void saveParameter(const char *filename);
    
private:
    void calcOutput(const Sample &sample);
    void initialize();
    void cleanUp();
    
    int		dimension_;
    int		hiddenUnitNum_;
    double	*hiddenUnit_;
    double	**hiddenWeight_;
    double	hiddenGain_;
    int		categoryNum_;
    double	*outputUnit_;
    double	**outputWeight_;
    double	outputGain_;
    
    // DISALLOW_COPY_AND_ASSIGN
    BPNetwork(const BPNetwork&);
    void operator=(const BPNetwork&);
};

#endif
