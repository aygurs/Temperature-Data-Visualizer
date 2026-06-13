#pragma once
#include <vector>
#include <string>
#include "TemperatureEntry.h"

class CSVReader
{
    public:
        CSVReader();

        /*Opens and reads CSV file for temperature data.*/
        static std::vector<TemperatureEntry> readCSV(std::string csvFile);

        /*Splits each line in CSV into a vector of separate strings.*/
        static std::vector<std::string> tokenise(std::string csvLine, char separator);

    private:
        /*Puts tokens of data from a line of the CSV file into organised lines of data, with their date and country.*/
        static std::vector<TemperatureEntry> tokensToTempData(const std::vector<std::string> tokens, const std::vector<std::string> countryNames, const std::string line);

        /*Gets country names from the first row of the file (Assuming first column says 'date')*/
        static std::vector<std::string> getCountryNames(std::ifstream& csvFile);
};