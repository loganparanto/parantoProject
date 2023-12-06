//AnalogIn.h

class AnalogIn
{
private:
    unsigned int number;

public:
    AnalogIn();
    AnalogIn(unsigned int n);
    virtual unsigned int getNumber() { return number; }
    virtual void setNumber(unsigned int n);
    virtual int readADCSample();
    virtual ~AnalogIn();
};
