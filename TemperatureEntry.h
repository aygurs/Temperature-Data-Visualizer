//Inspired by OrderBookEntry in Merkle trader simulator
#pragma once
#include <string> //Access to string type

//Holds one string of data from the dataset
class TemperatureEntry
{
    public:
        TemperatureEntry(std::string date, std::string country, double value);
        
        //Data members - Holds data from constructor
        std::string date;
        std::string country;
        double value;
};