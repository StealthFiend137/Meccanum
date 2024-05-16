#include <cmath>

/// @brief Meccanum drive train wheel calculations.
class Meccanum
{
public:

    /// @brief Container calculated wheel speeds.
    struct WheelSpeeds {
        double frontLeft;
        double frontRight;
        double rearLeft;
        double rearRight;
    };

private:

    /// @brief The distance lengthwise from the middle of the chassis to the wheel
    float _wheelBase;

    /// @brief The distance widthwise between the middle of the chassis to the wheel
    float _guage;

    /// @brief Precalculates the distance from the centre of the chassis to the wheel.
    float _halfDiagonalLength;

    /// @brief 
    /// @param speeds 
    void normalize(WheelSpeeds *speeds)
    {
        // Find the maximum speed among the wheels
        double maxFront = std::max(std::abs(speeds->frontLeft), std::abs(speeds->frontRight));
        double maxRear = std::max(std::abs(speeds->rearLeft), std::abs(speeds->rearRight));
        double maxSpeed = std::max(maxFront, maxRear);

        // If the maximum speed is greater than 1, normalize the speeds
        if (maxSpeed > 1.0) {
            speeds->frontLeft  /= maxSpeed;
            speeds->frontRight /= maxSpeed;
            speeds->rearLeft   /= maxSpeed;
            speeds->rearRight  /= maxSpeed;
        }
    }

public:

    /// @brief Constructs an instance of the Meccanum class.
    /// @param wheelBase The distance between the centre of the wheels front to back.
    /// @param track The distance between the centre of the wheels left to right.
    Meccanum(const float wheelBase, const float guage) : _wheelBase(wheelBase), _guage(guage)
    {
        _halfDiagonalLength = wheelBase + guage;
    };

    /// @brief 
    /// @param vX Forwards velocity.
    /// @param vY Horizontal velocity.
    /// @param vW Rotational velocity.
    /// @return Returns the calculated speeds of the wheels.
    WheelSpeeds CalculateAngularVelocities(const double vX, const double vY, const double vW)
    {
        WheelSpeeds speeds;
        speeds.frontLeft  = vX - vY - vW * _halfDiagonalLength;
        speeds.frontRight = vX + vY + vW * _halfDiagonalLength;
        speeds.rearLeft   = vX + vY - vW * _halfDiagonalLength;
        speeds.rearRight  = vX - vY + vW * _halfDiagonalLength;
        
        normalize(&speeds);

        return speeds;
    }
};
