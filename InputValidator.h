#pragma once
#include <string>

class InputValidator
{
    public:
        InputValidator();

        /*Checks if the input by the user when choosing a temperature value is valid.*/
        static bool isValidTempChoice(const std::string userTempChoice);

        /*Checks if the input by the user when choosing a country is valid.*/
        static bool isValidCountryChoice(const std::string userCountryChoice);

        /*Checks if the input by the user when choosing a timeframe is valid.*/
        static bool isValidTimeframeChoice(const std::string userTimeframeChoice);

        /*Checks if the input by the user when choosing a data range is valid.*/
        static bool isValidDataRangeChoice(std::string userDataRangeChoice, std::string timeframe);

        /*Checks if the input by the user when choosing a year is valid.*/
        static bool isValidYearChoice(const std::string userYearChoice);
};