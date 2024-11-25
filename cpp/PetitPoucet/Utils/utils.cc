#include "utils.hh"

namespace petitpoucet::utils
{
    double ConvertNMEAToWGS84Decimal(double coordinate)
    {
        int degreeCoordinate = coordinate / 100;
        long double restCoordinate = coordinate - (100 * degreeCoordinate);
        long double newCoordinateDecimals = degreeCoordinate + (restCoordinate/60);
        return newCoordinateDecimals;
    }
}
