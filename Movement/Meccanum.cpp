#include <cmath>

/// @brief Meccanum drive drive wheel calculations.
class Meccanum
{
private:

    /// @brief The distance betwee the centre of the wheels front to back.
    float _wheelBase;

    /// @brief The distance between the centre of the wheels left to right.
    float _track;

public:

    /// @brief Constructs an instance of the Meccanum class.
    /// @param wheelBase The distance between the centre of the wheels front to back.
    /// @param track The distance between the centre of the wheels left to right.
    Meccanum(float wheelBase, float track) : _wheelBase(wheelBase), _track(track) {};

    /// @brief 
    /// @param x 
    /// @param y 
    /// @param w 
    /// @param L 
    /// @param W 
    /// @param wheelSpeeds 
    void CalculateAngularVelocities(double x, double y, double w, double L, double W, double wheelSpeeds[4]) {
        // L is the distance between front and back wheels (robot length)
        // W is the distance between left and right wheels (robot width)
        
        // Calculate the terms for the wheel speed equations
        double r = sqrt((L * L) + (W * W));
        double A = x - w * (L / r);
        double B = x + w * (L / r);
        double C = y - w * (W / r);
        double D = y + w * (W / r);
        
        // Calculate the speed for each wheel
        wheelSpeeds[0] = sqrt(B * B + C * C); // Front Left Wheel
        wheelSpeeds[1] = sqrt(B * B + D * D); // Front Right Wheel
        wheelSpeeds[2] = sqrt(A * A + D * D); // Rear Right Wheel
        wheelSpeeds[3] = sqrt(A * A + C * C); // Rear Left Wheel
        
        // Optionally normalize the wheel speeds if needed (if any speed exceeds a certain max value)
        double maxSpeed = fmax(fmax(wheelSpeeds[0], wheelSpeeds[1]), fmax(wheelSpeeds[2], wheelSpeeds[3]));
        if (maxSpeed > 1.0) {
            for (int i = 0; i < 4; ++i) {
                wheelSpeeds[i] /= maxSpeed;
            }
        }
    }
};
