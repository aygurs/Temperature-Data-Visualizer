#pragma once
#include <string>

class Candlestick 
{
    public:
        std::string date;
        double open;
        double close;
        double high;
        double low;

        /*Gets values for the candlestick.*/
        Candlestick(std::string date, double open, double close, double high, double low);
};
