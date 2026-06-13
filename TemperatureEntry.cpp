//Inspired by OrderBookEntry in Merkle trader simulator
#include "TemperatureEntry.h"

TemperatureEntry::TemperatureEntry(std::string date, std::string country, double value)
:   date(date), 
    country(country), 
    value(value) 
{
    //Gets data from initialisation
};