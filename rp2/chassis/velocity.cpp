#pragama once
#include <algorithm>
#include "velocity.h"

/// @brief Represents an immutable speed between forward and reverse in the range 100% forwards or reverse.
/// @param velocity The forward or reverse speed.
/// @remarks Will clamp to -100 to 100, (100% forward or reverse), but will not throw error.
Velocity::Velocity(int velocity)
{
    this->_velocity = std::clamp(velocity, -100, 100);
};

/// @brief Gets the speed.
/// @return Returns the speed.
int Velocity::Get()
{
    return this->_velocity;
};
