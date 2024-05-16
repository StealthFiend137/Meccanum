#include <iostream>
#include <string>

#include "Meccanum.cpp"

using std::cout;
using std::endl;
using std::string;

int main()
{
    cout << endl << endl << endl;
    

    Meccanum mec(0.5, 0.5);
    auto wheelSpeeds = mec.CalculateAngularVelocities(0, 1, 0.5);

    cout << "Front left " << wheelSpeeds.frontLeft << endl;
    cout << "Front right " << wheelSpeeds.frontRight << endl;
    cout << "Rear left " << wheelSpeeds.rearLeft << endl;
    cout << "Rear right " << wheelSpeeds.rearRight << endl;


    //cout << mec.CalculateAngularVelocities(0, 0, 0) << endl;

    cout << endl << endl << endl;
    return 0;
}

