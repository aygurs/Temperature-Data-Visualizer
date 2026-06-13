#include "CandlestickGenerator.h"
#include <string>
#include <map>
#include <iostream>

std::vector<Candlestick> CandlestickGenerator::generateCandlestick(
            const std::vector<TemperatureEntry>& allTempData,
            const std::string country,
            const std::string timeframe,
            int startYear,
            int endYear)
{
    //Filter vector by country
    std::string countryFilter = interpretCountry(country);
    std::vector<TemperatureEntry> filteredCountryTemps = filterCountryChoice(allTempData, countryFilter);

    //Filter vector by data range
    std::vector<TemperatureEntry> filteredCountryAndDataRange = filterDataRangeChoice(filteredCountryTemps, 
                                                                                    startYear, 
                                                                                    endYear);

    //Filter vector by timeframe
    auto [timeframeNumber, timeframeLetter] = interpretTimeframe(timeframe);

    //Group data into candlestick groups
    std::vector<std::vector<TemperatureEntry>> groupedData = groupDataToCandlesticks(timeframeNumber, 
                                                                                    timeframeLetter, 
                                                                                    filteredCountryAndDataRange);

    //Calculate the candlestick data
    std::vector<Candlestick> candlesticks = calcCandlestickData(groupedData);

    //Return the vector
    return candlesticks;
};

std::string CandlestickGenerator::interpretCountry(std::string countryChoice)
{
    //Returns user choice as country matching dataset options
    return countryChoice + "_temperature";
};

std::vector<TemperatureEntry> CandlestickGenerator::filterCountryChoice(
    const std::vector<TemperatureEntry>& allTempData, 
    const std::string processedCountryChoice)
{   
    //Make vector to hold entries that match filter
    std::vector<TemperatureEntry> filteredByCountry;

    //Go through each entry in the main temp data
    for(const TemperatureEntry& entry : allTempData)
    {
        //If the country in the entry matches the chosen country by the user
        if (entry.country == processedCountryChoice)
        {
            //Add that entry to the filtered vector
            filteredByCountry.push_back(entry);
        }
    }

    //Return complete filtered vector
    return filteredByCountry;
};

std::vector<TemperatureEntry> CandlestickGenerator::filterDataRangeChoice(
            const std::vector<TemperatureEntry>& countryFilteredData, 
            int startYear,
            int endYear)
{
    //Make vector to hold entries that match filter
    std::vector<TemperatureEntry> filteredByDataRange;

    //Go through each entry in the main temp data
    for(const TemperatureEntry& entry : countryFilteredData)
    {
        //Variable to select year of entries
        int yearOfEntry = std::stoi(entry.date.substr(0, 4));

        if(yearOfEntry >= startYear && yearOfEntry <= endYear)
        {
            filteredByDataRange.push_back(entry);
        }
    }

    //Return complete filtered vector
    return filteredByDataRange;
};

std::pair<int, char> CandlestickGenerator::interpretTimeframe(std::string timeframeChoice)
{
    //Convert first character into a string, then into an int value
    int timeNumber = std::stoi(std::string(1, timeframeChoice[0]));
    //Second character (the letter) is already a character
    char timeLetter = timeframeChoice[1];

    //Return values as a pair to be used
    return std::make_pair(timeNumber, timeLetter);
};

std::vector<std::vector<TemperatureEntry>> CandlestickGenerator::groupDataToCandlesticks(
    int timeframeNumber, 
    char timeframeLetter, 
    std::vector<TemperatureEntry> filteredCountryAndDataRange)
{
    //Holds all candlestick data groups
    std::vector<std::vector<TemperatureEntry>> groupedData;
    //Singular group for each individual candlestick
    std::vector<TemperatureEntry> currentGroup;

    //Stores a map of each month and their monthly data
    std::map<std::string, std::vector<TemperatureEntry>> monthMap;
    //Vector for each months data
    std::vector<std::vector<TemperatureEntry>> monthGroups;

    //Stores a map of each year and their yearly data
    std::map<std::string, std::vector<TemperatureEntry>> yearMap;
    //Vector for each years data
    std::vector<std::vector<TemperatureEntry>> yearGroups;

    //How to group entries based off of timeframe letter
    switch (timeframeLetter) 
    {
        //Day
        case 'd': 
        {
            //Number of entries that make up one day (data is hourly)
            int entriesPerGroup = timeframeNumber * 24;
            //Keep track of number of entries
            int count = 0;

            for(const TemperatureEntry& entry : filteredCountryAndDataRange) 
            {
                //Add the entry to the singular group vector
                currentGroup.push_back(entry);
                //Increase the count
                count++;

                //If we have reached number of days for 1 candlestick
                if(count == entriesPerGroup) 
                {
                    //Put the whole vector into the larger vector
                    groupedData.push_back(currentGroup);
                    //Clear the singular group
                    currentGroup.clear();
                    //Reset counter
                    count = 0;
                }
            }

            //Handle leftover entries (that cant form a full candle, but still include the data)
            if(!currentGroup.empty())
            {
                groupedData.push_back(currentGroup);
            }

            break;
        }
        //Week
        case 'w': 
        {
            //Number of entries that make one week (data is hourly)
            int entriesPerGroup = timeframeNumber * 7 * 24;
            //Keep track of number of entries
            int count = 0;

            for(const TemperatureEntry& entry : filteredCountryAndDataRange) 
            {
                //Add the entry to the singular group vector
                currentGroup.push_back(entry);
                //Increase the count
                count++;

                //If we have reached number of days for 1 candlestick
                if(count == entriesPerGroup) 
                {
                    //Put the whole vector into the larger vector
                    groupedData.push_back(currentGroup);
                    //Clear the singular group
                    currentGroup.clear();
                    //Reset counter
                    count = 0;
                }
            }

            //Handle leftover entries (that cant form a full candle, but still include the data)
            if(!currentGroup.empty())
            {
                groupedData.push_back(currentGroup);
            }

            break;
        }
        //Month
        case 'm': 
        {
            //Go through each entry
            for(const TemperatureEntry& entry : filteredCountryAndDataRange) 
            {
                //Obtain the month of the entry and assign it as the key (includes year)
                std::string monthKey = entry.date.substr(0, 7);

                //Put the data into the map with the entry and the key created
                //If month has not been seen before, it makes a new entry with the key
                monthMap[monthKey].push_back(entry);
            }

            //Go through each entry in the map
            for(const auto& [key, entries] : monthMap) 
            {
                //Group them into a vector of their specific month
                monthGroups.push_back(entries);
            }

            //Loop through the groups of the months by index
            for(int i = 0; i < monthGroups.size(); i += timeframeNumber)
            {

                //Temporary vector to hold data
                std::vector<TemperatureEntry> combinedGroup;

                //Checks for left over data
                //If i + timeframeNumber goes above available months left,
                //it stops at last index available
                int groupEnd = std::min(i + timeframeNumber, (int)monthGroups.size());

                //Combines sub groups (based off of timeframeNumber)
                for(int j = i; j < groupEnd; ++j)
                {
                    //Adds sub groups to temporary vector
                    combinedGroup.insert(combinedGroup.end(), monthGroups[j].begin(), monthGroups[j].end());
                }

                //Adds vectors of candlesticks to main vector group
                groupedData.push_back(combinedGroup);
            }

            break;
        }
        //Year
        case 'y': 
        {
            //Go through each entry
            for(const TemperatureEntry& entry : filteredCountryAndDataRange) 
            {
                //Obtain the year of the entry and assign it as the key
                std::string yearKey = entry.date.substr(0, 4);

                //Put the data into the map with the entry and the key created
                //If year has not been seen before, it makes a new entry with the key
                yearMap[yearKey].push_back(entry);
            }

            //Go through each entry in the map
            for(const auto& [key, entries] : yearMap) 
            {
                //Group them into a vector of their specific year
                yearGroups.push_back(entries);
            }

            //Loop through the groups of the months by index
            for(int i = 0; i < yearGroups.size(); i += timeframeNumber)
            {
                //Temporary vector to hold data
                std::vector<TemperatureEntry> combinedGroup;

                //Checks for left over data
                //If i + timeframeNumber goes above available months left,
                //it stops at last index available
                int groupEnd = std::min(i + timeframeNumber, (int)yearGroups.size());

                //Combines sub groups (based off of timeframeNumber)
                for(int j = i; j < groupEnd; ++j)
                {
                    //Adds sub groups to temporary vector
                    combinedGroup.insert(combinedGroup.end(), yearGroups[j].begin(), yearGroups[j].end());
                }

                //Adds vectors of candlesticks to main vector group
                groupedData.push_back(combinedGroup);
            }

            break;
        }
        //Default to reading daily values
        default:
        {
            //Number of entries that make up one day (data is hourly)
            int entriesPerGroup = timeframeNumber * 24;
            //Keep track of number of entries
            int count = 0;

            for(const TemperatureEntry& entry : filteredCountryAndDataRange) 
            {
                //Add the entry to the singular group vector
                currentGroup.push_back(entry);
                //Increase the count
                count++;

                //If we have reached number of days for 1 candlestick
                if(count == entriesPerGroup) 
                {
                    //Put the whole vector into the larger vector
                    groupedData.push_back(currentGroup);
                    //Clear the singular group
                    currentGroup.clear();
                    //Reset counter
                    count = 0;
                }
            }

            break;
        };
    };

    //Return grouped data
    return groupedData;
};

std::vector<Candlestick> CandlestickGenerator::calcCandlestickData(std::vector<std::vector<TemperatureEntry>> groupedData)
{
    //Hold the candlestick vectors
    std::vector<Candlestick> candlesticks;

    //Calculate candlestick data based off of grouped data
    for(size_t i = 0; i < groupedData.size(); ++i)
    {
        //Select current group
        const auto& currentGroup = groupedData[i];

        //If a group is empty, skip
        if(currentGroup.empty())
        {
            std::cout << "CandlestickGenerator::generateCandlestick encountered an empty group!" << std::endl;
            std::cout << "Group index " << i << " caused the error." << std::endl;
            continue;
        };

        //Hold the sum when calculating mean
        double sum = 0;

        //Set high and low temps to something impossible
        //This helps to show errors and ensure sorting works correctly
        double high = -200;
        double low = 200;

        for(const auto& entry : currentGroup)
        {

            //Add the temp value to the sum
            sum += entry.value;

            //Track highest value
            if(entry.value > high)
            {
                high = entry.value;
            };

            //Track lowest value
            if(entry.value < low)
            {
                low = entry.value;
            };
        };

        //Calculate the close value
        //Mean of this CURRENT timeframe
        double close = sum / currentGroup.size();

        //Calculate the open value
        //Mean of the PREVIOUS timeframe
        double open;

        //If i is 0, that means there is no previous group for data to calculate mean
        //This will be a flat candle as open and close will be the same value
        if(i == 0)
        {
            open = close;
        }
        //If there IS a previous group
        else
        {
            //Select previous group
            const auto& prevGroup = groupedData[i - 1];
            //Variable to hold sum for previous group
            double prevSum = 0;

            //Loop over previous entries
            for(const auto& prevEntry : prevGroup)
            {
                //Add all temps from previous entries into variable
                prevSum += prevEntry.value;
            };

            //Calculate mean value
            open = prevSum / prevGroup.size();
        };

        //Get the date from the first member of each group (not including time)
        std::string date = currentGroup.front().date.substr(0, 10);

        //Push values back to candlesticks vector
        candlesticks.emplace_back(date, open, close, high, low);
    };

    std::cout << "Total candlesticks returned: " << candlesticks.size() << std::endl;
    return candlesticks;
};