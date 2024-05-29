class UpdatedValuesEventListener
{
public:

    enum class UpdatedValues
    {
        wAxis = 1,
        xAxis = 2,
        yAxis = 4
    };

    virtual void ValuesUpdated(UpdatedValues updatedValues) = 0;
};
