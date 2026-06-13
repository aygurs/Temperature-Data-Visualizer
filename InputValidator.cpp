#include <string>
#include <vector>
#include <iostream>
#include <algorithm>
#include "InputValidator.h"

InputValidator::InputValidator()
{

}

bool InputValidator::isValidCountryChoice(const std::string userCountryChoice)
{
    //Valid choices for country
    const std::vector<std::string> countryOptions = {
        "AT", "BE", "BG", "CH", "CZ", "DE", "DK", "EE", "ES", "FI", "FR", "GB", "GR", "HR",
        "HU", "IE", "IT", "LT", "LU", "LV", "NL", "NO", "PL", "PT", "RO", "SE", "SI", "SK"
    };

    //Search for user input in countryOptions and return true if it is found
    return std::find(countryOptions.begin(), countryOptions.end(), userCountryChoice) != countryOptions.end();
}

bool InputValidator::isValidTimeframeChoice(const std::string userTimeframeChoice)
{
    //Check if length is equal to 2
    if(userTimeframeChoice.length() != 2)
    {
        std::cout << "InputValidator::isValidTimeframeChoice rejected your input: length is not 2." << std::endl;
        return false;
    }

    //Check if first character is a digit between 1 and 9
    char first = userTimeframeChoice[0];
    if(first < '1' || first > '9')
    {
        std::cout << "InputValidator::isValidTimeframeChoice rejected your input: " << std::endl;
        std::cout << "first char: " << first << ", is not between 1 and 9." << std::endl;
        return false;
    }

    // Check if second character is either d, w, m or y
    char second = userTimeframeChoice[1];
    if(second != 'd' && second != 'w' && second != 'm' && second != 'y')
    {
        std::cout << "InputValidator::isValidTimeframeChoice rejected your input: " << std::endl;
        std::cout << "second char: " << second << ", is not d, w, m or y." << std::endl;
        return false;
    }

    std::cout << "InputValidator::isValidTimeframeChoice accepts your input!" << std::endl;
    return true;
};

bool InputValidator::isValidDataRangeChoice(std::string userDataRangeChoice, std::string timeframe)
{
    //Number of timeframe (e.g. 1y = 1), converts to int
    int timeNum = timeframe[0] - '0';

    //Letter of timeframe (e.g. 1y = y)
    char timeLetter = timeframe[1];
    
    //Variable to check if user chose a yearly candlestick timeframe
    bool userChoseYear = false;

    //Remove spaces from string input
    userDataRangeChoice.erase(std::remove(userDataRangeChoice.begin(), userDataRangeChoice.end(), ' '), userDataRangeChoice.end());

    //Checks if the user chose a yearly timeframe for candlestick data
    if(timeLetter == 'y')
    {
        userChoseYear = true;
    }

    //If user types 'all', it is a valid choice
    if(userDataRangeChoice == "all")
    {
        std::cout << "InputValidator::isValidDataRangeChoice read: " << userDataRangeChoice << std::endl;
        std::cout << "InputValidator::isValidDataRangeChoice says: Valid data range! Please wait... "<< std::endl;
        return true;
    }

    //If user types a singular year (4 digits) and they are all numbers
    if(userDataRangeChoice.length() == 4 && std::all_of(userDataRangeChoice.begin(), userDataRangeChoice.end(), ::isdigit))
    {
        //If the year is between and including 1980 - 2019, it is valid
        if(std::stoi(userDataRangeChoice) >= 1980 && std::stoi(userDataRangeChoice) <= 2019)
        {
            //If the user did not choose a yearly timeframe
            if(!userChoseYear)
            {
                std::cout << "InputValidator::isValidDataRangeChoice says: Valid data range! Please wait... "<< std::endl;
                return true;
            }
            //If the user chose a yearly timeframe
            else{
                //As the user wrote a singular year as the data range,
                //if the timeframe is more than 1 year then the data range will not be big enough
                //to show desired candlestick data
                if(timeNum != 1)
                {
                    std::cout << "Candlestick timeframe choice: " << timeNum << std::endl;
                    std::cout << "Data range choice: " << userDataRangeChoice << " (1 year)" << std::endl;
                    std::cout << "InputValidator::isValidDataRangeChoice says: Candlestick timeframe is larger than data range!"<< std::endl;
                    return false;
                } 
                else 
                {
                    std::cout << "InputValidator::isValidDataRangeChoice says: Valid data range! Please wait... "<< std::endl;
                    return true;
                }
            }
        }
    }

    //If input contains a dash
    if(userDataRangeChoice.length() == 9 && userDataRangeChoice[4] == '-')
    {
        //Gathers the start year entered by the user
        std::string startYear = userDataRangeChoice.substr(0, 4);

        //Gathers the end year entered by the user
        std::string endYear = userDataRangeChoice.substr(5, 4);

        //If all years are digits
        if(std::all_of(startYear.begin(), startYear.end(), ::isdigit) &&
            std::all_of(endYear.begin(), endYear.end(), ::isdigit))
        {
            //Turn years into integers
            int start = std::stoi(startYear);
            int end = std::stoi(endYear);

            //If years are between min and max values and start is earlier than end
            if(start >= 1980 && end <= 2019 && start <= end)
            {
                //If the user did not choose a yearly timeframe
                if(!userChoseYear)
                {
                    std::cout << "InputValidator::isValidDataRangeChoice says: Valid data range! Please wait... "<< std::endl;
                    return true;
                }
                //If the user chose a yearly timeframe
                else{
                    //If the year range is larger than the number of years in the timeframe
                    //It is valid
                    if((end - start + 1) >= timeNum)
                    {
                        std::cout << "InputValidator::isValidDataRangeChoice says: Valid data range! Please wait... "<< std::endl;
                        return true;
                    }
                    else
                    {
                        std::cout << "InputValidator::isValidDataRangeChoice says: Candlestick timeframe is larger than data range!"<< std::endl;
                        return false;
                    }
                };
            };
        } else std::cout << "InputValidator::isValidDataRangeChoice read letters!"<< std::endl;
    };

    std::cout << "InputValidator::isValidDataRangeChoice says: You did not choose a valid data range."<< std::endl;
    return false;
};

bool InputValidator::isValidTempChoice(const std::string userTempChoice)
{
    //Valid choices for temperatures
    const std::vector<std::string> tempOptions = {"avg", "max", "min"};

    //Search for user input in tempOptions and return true if it is found
    return std::find(tempOptions.begin(), tempOptions.end(), userTempChoice) != tempOptions.end();
};

bool InputValidator::isValidYearChoice(const std::string userYearChoice)
{
    //Check if input is digits
    if(!std::all_of(userYearChoice.begin(), userYearChoice.end(), ::isdigit))
    {
        std::cout << "Please enter only numbers!" << std::endl;
        return false;
    }

    if(std::stoi(userYearChoice) >= 1981 && std::stoi(userYearChoice) <= 2020)
    {
        return true;
    } 
    else 
    {
        return false;
    }
};