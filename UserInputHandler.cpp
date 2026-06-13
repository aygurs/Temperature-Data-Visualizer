#include <string>
#include <iostream>
#include "UserInputHandler.h"
#include "InputValidator.h"

UserInputHandler::UserInputHandler()
{

};

int UserInputHandler::getUserMenuChoice()
{
    int userChoice = 0;
    std::string line;

    std::cout << "Choose an option (1-4)" << std::endl;
    //Get user choice and input into line variable
    std::getline(std::cin, line);
    
    try
    {
        //Convert string to int
        userChoice = std::stoi(line);
    }
    catch(const std::exception& e)
    {
        //Catches if user option is not a number
    }

    std::cout << "You chose: " << userChoice << std::endl;
    return userChoice;
};

std::string UserInputHandler::userCountryChoice()
{
    //Default country code
    std::string countryChoice = "AT";

    std::cout << "Which country would you like data for? Please choose out of the following options (uppercase only): " << std::endl;
    std::cout << "AT, BE, BG, CH, CZ, DE, DK, EE, ES, FI, FR, GB, GR, HR, " << std::endl;
    std::cout << "HU, IE, IT, LT, LU, LV, NL, NO, PL, PT, RO, SE, SI or SK." << std::endl;

    //Variable to hold valid or not boolean
    bool isValid;
    
    do
    {
        //Get user input
        std::getline(std::cin, countryChoice);

        isValid = InputValidator::isValidCountryChoice(countryChoice);

        //Check if it is a valid choice
        if(!isValid)
        {
            std::cout << "Invalid country code. Please try again: " << std::endl;
        }
    //Continue looping until valid country is chosen
    } while (!isValid);

    std::cout << "You chose: " << countryChoice << std::endl;
    return countryChoice;
};

std::string UserInputHandler::userTimeframeChoice()
{
    //Default timeframe
    std::string timeframeChoice = "1y";

    std::cout << "What timeframe would you like to see data for?" << std::endl;
    std::cout << "Please type a number (between 1-9) and letter to represent timeframe (either d, w, m, y)." << std::endl;
    std::cout << "For example: 1d OR 1w OR 1m OR 1y:" << std::endl;

    //Variable to hold valid or not boolean
    bool isValid;

    do
    {
        //Get user input
        std::getline(std::cin, timeframeChoice);

        isValid = InputValidator::isValidTimeframeChoice(timeframeChoice);

        //Check if it is a valid choice
        if(!isValid)
        {
            std::cout << "You chose: " << timeframeChoice << std::endl;
            std::cout << "Invalid timeframe choice. Please try again: " << std::endl;
        }
    //Continue looping until valid country is chosen
    } while (!isValid);

    std::cout << "You chose: " << timeframeChoice << std::endl;
    return timeframeChoice;
};

std::string UserInputHandler::userYearChoice()
{
    std::cout << "Which year would you like to predict for? Choose between 1981 - 2020 (inclusive): " << std::endl;

    //Default year choice
    std::string yearChoice = "1981";

    //Variable to hold valid or not boolean
    bool isValidYear;

    do
    {
        //Get user input
        std::getline(std::cin, yearChoice);

        isValidYear = InputValidator::isValidYearChoice(yearChoice);

        //Check if it is a valid choice
        if(!isValidYear)
        {
            std::cout << "Invalid year choice. Please try again: " << std::endl;
        }
    //Continue looping until valid year is chosen
    } while (!isValidYear);

    std::cout << "You chose: " << yearChoice << std::endl;
    return yearChoice;
};

std::string UserInputHandler::userTempDataChoice()
{
    //Default temp choice
    std::string tempChoice = "avg";

    std::cout << "Please choose what temperature data you would like to view on the chart." << std::endl;
    std::cout << "You can choose either: avg (average temperatures), max (maximum temperatures) or min (minimum temperatures): " << std::endl;

    //Variable to hold valid or not boolean
    bool isValid;

    do
    {
        //Get user input
        std::getline(std::cin, tempChoice);

        isValid = InputValidator::isValidTempChoice(tempChoice);

        //Check if it is a valid choice
        if(!isValid)
        {
            std::cout << "Invalid temperature choice. You must either choose avg, min or max. Please try again: " << std::endl;
        }
    //Continue looping until valid temperature option is chosen
    } while (!isValid);

    std::cout << "You chose: " << tempChoice << std::endl;
    return tempChoice;
};

std::pair<int, int> UserInputHandler::userDataRangeChoice(std::string timeframe)
{
    //Hold user input
    std::string dataRangeChoice;

    //Default values (all)
    int startYear = 1980;
    int endYear = 2019;

    //Checks for dash in input
    bool afterDash = false;

    std::cout << "What data range would you like to view data between?" << std::endl;
    std::cout << "Please enter a year (e.g. 1980), a year range (e.g. 1980-1990) or type 'all' to use all years of data (1980-2019):" << std::endl;

    //Variable to hold valid or not boolean
    bool isValid;

    do
    {
        std::getline(std::cin, dataRangeChoice);
        std::cout << "You chose: " << dataRangeChoice << std::endl;

        //Assign variable to valid check
        isValid = InputValidator::isValidDataRangeChoice(dataRangeChoice, timeframe);

        //Check if it is a valid choice
        if (!isValid)
        {
            std::cout << "Invalid data range choice. Please try again: " << std::endl;
        }
    }
    while (!isValid);
    

    //If user types 'all'
    if(dataRangeChoice == "all")
    {
        startYear = 1980;
        endYear = 2019;
    }
    //If user types a range
    else if(dataRangeChoice.find('-') != std::string::npos)
    {
        //Variables to collect numbers
        std::string startNum = "";
        std::string endNum = "";

        //Loop through input
        for(char c : dataRangeChoice)
        {
            //If a dash is present
            if(c == '-')
            {
                afterDash = true;
                continue;
            }

            //Add numbers before the dash to startNum (will be startYear)
            if(!afterDash)
                startNum += c;
            //Add numbers after the dash to endNum (will be endYear)
            else
                endNum += c;
        }

        //Convert results to int and assign to original variables
        startYear = std::stoi(startNum);
        endYear = std::stoi(endNum);
    }
    //If user types a singular year
    else
    {
        startYear = std::stoi(dataRangeChoice);
        endYear = startYear;
    }

    return std::make_pair(startYear, endYear);
};