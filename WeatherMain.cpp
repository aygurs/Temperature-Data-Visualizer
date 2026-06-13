#include "WeatherMain.h"
#include "CSVReader.h"
#include "CandlestickGenerator.h"
#include "UserInputHandler.h"
#include "InputValidator.h"
#include "RenderChart.h"
#include <iostream>
#include <vector>
#include <iomanip> //For data output tables
#include <limits> //For ignoring characters in output of charts

//Inspired by MerkleMain file
WeatherMain::WeatherMain()
{

};

void WeatherMain::init()
{
    //Holds user input from menu
    int input;

    //Shows user program is loading
    std::cout << "Loading program, please wait..." << std::endl;

    //Initialise temp data with file
    allTemperatureData = CSVReader::readCSV("dataset/temp_data_1980-2019.csv");
    std::cout << "Program loaded successfully! Thanks for waiting!" << std::endl;

    //Infinite loop
    while(true)
    {
        printMenu();
        input = UserInputHandler::getUserMenuChoice();
        processUserChoice(input);
    };
};

void WeatherMain::printMenu()
{
    //1 Print Welcome
    std::cout << "========Temperature App========" << std::endl;
    std::cout << "Welcome to the temperature application. Here, you can see historical temperature information displayed in various ways." << std::endl;
    std::cout << "Please choose what you would like to do:" << std::endl;

    //2 Print table of candlestick data
    std::cout << "1: View data in a table format" << std::endl;

    //3 Print plot of candlesticks
    std::cout << "2: View data in a plot format (with candlesticks)" << std::endl;

    //4 Print graph of candlestick data
    std::cout << "3: View data in a graph format" << std::endl;

    //5 Predict data
    std::cout << "4: Predict average temperature for each month of a year" << std::endl;

    //User option input
    std::cout << "==============================" << std::endl;
}

void WeatherMain::candlestickDataTable()
{
    std::string country = UserInputHandler::userCountryChoice();
    std::string timeframe = UserInputHandler::userTimeframeChoice();
    auto[startYear, endYear] = UserInputHandler::userDataRangeChoice(timeframe);
    
    //Generate candlestick data from info form user
    std::vector<Candlestick> candlesticks = CandlestickGenerator::generateCandlestick(
        allTemperatureData, country, timeframe, startYear, endYear
    );

    //Print candlestick data to console in table
    //Column headers
    std::cout << std::left
          << std::setw(12) << "Date"
          << std::setw(10) << "Open"
          << std::setw(10) << "High"
          << std::setw(10) << "Low"
          << std::setw(10) << "Close" << std::endl;

    //Fill table with data
    for(const auto& candle : candlesticks)
    {
        std::cout << std::left
              << std::setw(12) << candle.date
              //Set to 5 decimal places max
              << std::fixed << std::setprecision(5)
              << std::setw(10) << candle.open
              << std::setw(10) << candle.high
              << std::setw(10) << candle.low
              << std::setw(10) << candle.close << std::endl;
    }

    std::cout << "WeatherMain::candlestickDataTable says: Output complete!" << std::endl;
};

void WeatherMain::candlestickDataChart()
{
    std::string country = UserInputHandler::userCountryChoice();
    std::string timeframe = UserInputHandler::userTimeframeChoice();
    auto [startYear, endYear] = UserInputHandler::userDataRangeChoice(timeframe);
    
    //Generate candlestick data from info form user
    std::vector<Candlestick> candlesticks = CandlestickGenerator::generateCandlestick(
        allTemperatureData, country, timeframe, startYear, endYear
    );

    //CREATE CHART
    //Set max chart size by setting column numbers
    int MAX_VISIBLE = 15;

    //Start at 0 and count upwards
    int start = 0;

    //While we haven't shown all candlestick data, keep looping
    //Allows for chunks of data to be shown at a time in terminal
    while(start < candlesticks.size()) 
    {
        //Checks what is smaller, the size of the candlesticks left or the number of candlesticks to show next
        //Ensures we do not go out of bounds
        int end = std::min(start + MAX_VISIBLE, (int)candlesticks.size());

        //Slice the chunk of candlesticks to show to process them into chart
        std::vector<Candlestick> visibleCandles(candlesticks.begin() + start, candlesticks.begin() + end);

        //Generate x axis values (column width and x axis values)
        auto[colWidth, x_axisLabels] = RenderChart::generateXAxis(timeframe, visibleCandles);

        //Generate y axis values (number of y values, max & min y values, y axis values)
        auto[noOfYValues, yMax, yMin, y_axisLabels] = RenderChart::generateYAxis(visibleCandles);

        //Create the chart grid (2D grid)
        //Use a space: ' ' as a placeholder for the candlesticks
        //std::vector<std::char>(candlesticks.size(), ' ') = number of candlesticks (columns)
        //noOfYValues = rows
        std::vector<std::vector<char>> createChart(noOfYValues, std::vector<char>(visibleCandles.size(), ' '));

        RenderChart::drawCandlesticks(visibleCandles, createChart, yMax, yMin, noOfYValues);

        RenderChart::printChart(createChart, y_axisLabels, x_axisLabels, visibleCandles, colWidth, noOfYValues);

        //If we have shown all candlesticks in this chunk
        if(end < candlesticks.size())
        {
            std::cout << "-- Showing entries " << start + 1 << " to " << end << " of " << candlesticks.size() << ". Press Enter to continue --";

            //Waits for user input
            //Ignores max number of characters possible unless a new line is input (enter)
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }

        //Create new start position
        start += MAX_VISIBLE;
    }

    std::cout << "WeatherMain::candlestickDataChart says: Output complete!" << std::endl;
};

void WeatherMain::candlestickDataGraph()
{
    std::string country = UserInputHandler::userCountryChoice();
    std::string timeframe = UserInputHandler::userTimeframeChoice();
    auto[startYear, endYear] = UserInputHandler::userDataRangeChoice(timeframe);
    std::string tempDataChoice = UserInputHandler::userTempDataChoice();

    //Generate candlestick data from info form user
    std::vector<Candlestick> candlesticks = CandlestickGenerator::generateCandlestick(
        allTemperatureData, country, timeframe, startYear, endYear
    );

    //CREATE CHART
    //Set max chart size by setting column numbers
    int MAX_VISIBLE = 15;

    //Start at 0 and count upwards
    int start = 0;

    //While we haven't shown all candlestick data, keep looping
    //Allows for chunks of data to be shown at a time in terminal
    while(start < candlesticks.size()) 
    {
        //Checks what is smaller, the size of the candlesticks left or the number of candlesticks to show next
        //Ensures we do not go out of bounds
        int end = std::min(start + MAX_VISIBLE, (int)candlesticks.size());

        //Slice the chunk of candlesticks to show to process them into chart
        std::vector<Candlestick> visibleCandles(candlesticks.begin() + start, candlesticks.begin() + end);

        //Vector to store values to plot to graph
        std::vector<double> selectedValues;

        //Loop through candlestick temp data
        for(const auto& candle : visibleCandles)
        {
            //If user chooses average temp, collect close data
            if (tempDataChoice == "avg") selectedValues.push_back(candle.close);
            //If user chooses maximum temps, collect high data
            else if (tempDataChoice == "max") selectedValues.push_back(candle.high);
            //If user chooses minimum temps, collect low data
            else if (tempDataChoice == "min") selectedValues.push_back(candle.low);
        }

        //Collect y axis values
        double yMax = *std::max_element(selectedValues.begin(), selectedValues.end());
        double yMin = *std::min_element(selectedValues.begin(), selectedValues.end());
        int noOfYValues = 30;

        //Collect x axis values
        auto [colWidth, x_axisLabels] = RenderChart::generateXAxis(timeframe, visibleCandles);

        //Create 2D grid for graph
        std::vector<std::vector<char>> graph(noOfYValues, std::vector<char>(selectedValues.size(), ' '));

        //Print the graph
        RenderChart::printGraph(graph, selectedValues, x_axisLabels, yMax, yMin, noOfYValues, colWidth);
    
        //If we have shown all candlesticks in this chunk
        if(end < candlesticks.size())
        {
            std::cout << "-- Showing entries " << start + 1 << " to " << end << " of " << candlesticks.size() << ". Press Enter to continue --";

            //Waits for user input
            //Ignores max number of characters possible unless a new line is input (enter)
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }

        //Create new start position
        start += MAX_VISIBLE;
    }

    std::cout << "WeatherMain::candlestickDataGraph says: Output complete!" << std::endl;
};

void WeatherMain::predictTempData()
{
    std::cout << "This feature will predict the average temperature of each month in a year and display it on a bar chart!" << std::endl;

    //Obtain country choice
    std::string country = UserInputHandler::userCountryChoice();

    //Obtain year choice to predict
    std::string yearChoice = UserInputHandler::userYearChoice();

    std::cout << "Predicting temperatures for your chosen year. Please wait..." << std::endl;

    //Obtain vector of monthly averages
    std::vector<double> monthlyAverages = RenderChart::calcMonthlyAverages(country, yearChoice, allTemperatureData);

    //Collect y axis values
    double yMax = *std::max_element(monthlyAverages.begin(), monthlyAverages.end());
    double yMin = *std::min_element(monthlyAverages.begin(), monthlyAverages.end());
    int noOfYValues = 30;

    //Column width
    int colWidth = 4;

    //Set up x-axis labels for months manually
    std::vector<std::string> x_axisLabels = {
        "Jan", "Feb", "Mar", "Apr", "May", "Jun",
        "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"
    };

    //Create 2D chart grid
    std::vector<std::vector<char>> graph(noOfYValues, std::vector<char>(monthlyAverages.size(), ' '));

    //Print the graph
    RenderChart::printGraph(graph, monthlyAverages, x_axisLabels, yMax, yMin, noOfYValues, colWidth);

    //Calculate how many years of data was used
    int prevYears = std::stoi(yearChoice) - 1980;

    std::cout << "WeatherMain::predictTempData says: Prediction chart complete!" << std::endl;
    std::cout << "This used " << prevYears << " year(s) of historical data!" << std::endl;
};

void WeatherMain::printInvalidInput(){
    std::cout << "Invalid input. Please choose a number from 1-4" << std::endl;
};

void WeatherMain::processUserChoice(int userChoice){
    switch(userChoice){
        case 1:
            candlestickDataTable();
            break;

        case 2:
            candlestickDataChart();
            break;

        case 3:
            candlestickDataGraph();
            break;
        
        case 4:
            predictTempData();
            break;

        default:
            printInvalidInput();
            break;
    };
};