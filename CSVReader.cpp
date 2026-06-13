//Base code taken from the Merkle trader simulator
#include "CSVReader.h"
#include "TemperatureEntry.h"
#include <iostream>
#include <fstream>

CSVReader::CSVReader()
{

}

std::vector<std::string> CSVReader::tokenise(std::string csvLine, char separator)
{
    //Stores the tokens
    std::vector<std::string> tokens;

    //Find position of words in line
    signed int start, end;

    //Store individual token
    std::string token;

    //Find first char that isn't separator
    start = csvLine.find_first_not_of(separator, 0);

    do
    {
        //Find the first occurrence of separator after the start char
        end = csvLine.find_first_of(separator, start);

        //Line has ended
        if(start == csvLine.length() || start == end) break;

        //Found separator
        if(end >= 0) token = csvLine.substr(start, end-start);
        else token = csvLine.substr(start, csvLine.length() - start);

        //Push individual token to tokens
        tokens.push_back(token);

        //Move to next token
        start = end + 1;
    }
    while(end != std::string::npos);

    return tokens;
};

std::vector<std::string> CSVReader::getCountryNames(std::ifstream& csvFile)
{
    //Holds line of data from file
    std::string headerLine;

    //Stores country names from first row
    std::vector<std::string> countryNames;

    //Get first row of data from CSV file
    if(std::getline(csvFile, headerLine))
    {
        //Split first row into tokens
        countryNames = tokenise(headerLine, ',');

        //Remove first token from vector (first token = utc_timestamp, not a country)
        countryNames.erase(countryNames.begin());
    }

    return countryNames;
};

std::vector<TemperatureEntry> CSVReader::tokensToTempData(const std::vector<std::string> tokens, const std::vector<std::string> countryNames, const std::string line)
{
    //Holds all data for this line
    //Wil include temp data for all 28 countries for this time stamp
    std::vector<TemperatureEntry> tempDataForThisLine;

    //Checks if there are at least 29 tokens
    //1 = date, 28 = countries temp data
    if(tokens.size() != 29)
    {
        std::cout << "Error! CSVReader::tokensToTempData says there are not enough tokens on line: " << line << std::endl;

        //Throw an error
        //Function that calls this function has to deal with this exception
        throw std::exception{};
    };

    //Sets the date as the first token value collected
    std::string date = tokens[0];

    //Using iterator loop as missing first token of line out
    for(size_t i = 1; i < tokens.size(); ++i)
    {
        try
        {
            //Each country name is one iteration back
            std::string country = countryNames[i - 1];

            //Get temperature value
            double tempValue = std::stod(tokens[i]);

            //Adds data to temps vector
            tempDataForThisLine.push_back(TemperatureEntry(date, country, tempValue));
        }

        //If error is thrown, there is bad data on the line
        catch(const std::exception& e)
        {
            std::cout << "Error! Bad data on line: " << line << std::endl;
            continue;
        };
    };

    return tempDataForThisLine;
};

std::vector<TemperatureEntry> CSVReader::readCSV(std::string csvFileName)
{
    //Vector of temperature data
    std::vector<TemperatureEntry> allTempData;

    //Opens data file
    std::ifstream csvFile{csvFileName};

    //Holds one line of data
    std::string line;

    //Check CSV file opened successfully
    if(csvFile.is_open())
    {
        //Get country names
        std::vector<std::string> countryNames = getCountryNames(csvFile);

        //While there are still lines of data to be read
        //Loop over each line
        while(std::getline(csvFile, line))
        {
            try
            {
                //Convert line data into useable data
                //Returns a vector of lines of date, country and temp
                std::vector<TemperatureEntry> tempDataForTimeStamp = tokensToTempData(tokenise(line, ','), countryNames, line);
                
                //Loops over the vector and adds each line of the vector returned into the final temps vector
                for(const TemperatureEntry& entry : tempDataForTimeStamp)
                {
                    allTempData.push_back(entry);
                }
            }
            catch(const std::exception& e)
            {
                std::cout << "CSVReader::readCSV read bad data! Moving to next line... " << std::endl;
            }
            
        };
    }
    else
    {
        std::cout << "CSVReader::readCSV could not open file! " << std::endl;
    }

    //Show how many lines of data read
    std::cout << "CSVReader::readCSV read: " << allTempData.size() << " temperature entries.\n";

    //Return vector of temps
    return allTempData;
};
