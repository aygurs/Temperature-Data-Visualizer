#pragma once
#include "TemperatureEntry.h"
#include "Candlestick.h"
#include <vector>

class CandlestickGenerator 
{
    public:
        /*Generates candlestick data from the information given to it by the user.*/
        static std::vector<Candlestick> generateCandlestick(
            const std::vector<TemperatureEntry>& allTempData,
            const std::string country,
            const std::string timeframe,
            int startYear,
            int endYear
        );
    
    private:
        /*Converts the country choice from the user into a filtering option for the temperature data.*/
        static std::string interpretCountry(std::string countryChoice);

        /*Returns a vector filtered by the correct country choice as selected by the user.*/
        static std::vector<TemperatureEntry> filterCountryChoice(
            const std::vector<TemperatureEntry>& allTempData, 
            const std::string processedCountryChoice);

        /*Returns a vector filtered by the correct data frame choice as selected by the user.*/
        static std::vector<TemperatureEntry> filterDataRangeChoice(
            const std::vector<TemperatureEntry>& countryFilteredData, 
            int startYear,
            int endYear);

        /*Converts the timeframe choice from the user into a filtering option for the temperature data. 
        Returns a pair of data (number and letter for timeframe).*/
        static std::pair<int, char> interpretTimeframe(std::string timeframeChoice);

        /*Groups data into their correct candlestick timeframes.
        Each timeframe is in its own vector, within a larger vector.*/
        static std::vector<std::vector<TemperatureEntry>> groupDataToCandlesticks(
            int timeframeNumber, 
            char timeframeLetter, 
            std::vector<TemperatureEntry> filteredCountryAndDataRange);

        /*Converts grouped candlestick data into candlestick values like open, close, high and low.*/
        static std::vector<Candlestick> calcCandlestickData(std::vector<std::vector<TemperatureEntry>> groupedData);
};