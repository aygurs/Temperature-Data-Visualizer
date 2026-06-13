#include <vector>
#include <string>
#include <iostream>
#include <iomanip> //For data output tables
#include <cmath>
#include "RenderChart.h"

//Colours for candlestick chart
#define RED "\033[31m"
#define GREEN "\033[32m"
#define RESET "\033[0m"

RenderChart::RenderChart()
{

}

std::pair<int, std::vector<std::string>> RenderChart::generateXAxis(std::string timeframe, std::vector<Candlestick>& visibleCandles)
{
    //GENERATE x AXIS
    //Hold x labels for chart
    std::vector<std::string> x_axisLabels;
    
    //Extract timeframe letter
    char timeframeLetter = timeframe[1];

    //Hold column width so we can show full x axis label
    int colWidth = 1;

    //Calculate the column width based off of the timeframe letter
    switch(timeframeLetter) 
    {
        case 'd':
        case 'w':
            colWidth = 8;
            break;
        case 'm':
            colWidth = 5;
            break;
        case 'y':
            colWidth = 4;
            break;
        default:
            colWidth = 8;
            break;
    }

    //Loop through candlestick data to capture dates based off of timeframe letter
    for(const auto& candle : visibleCandles)
    {
        //Variable to assign label to
        std::string label;

        //Choose what date to extract based off of timeframe chosen
        switch(timeframeLetter) 
        {
            case 'd':
            case 'w':
                //Extract year, month, day
                label = candle.date.substr(2, 8);
                break;
            case 'm':
                //Extract year and month
                label = candle.date.substr(2, 5);
                break;
            case 'y':
                //Extract year
                label = candle.date.substr(0, 4);
                break;
            default:
                //Day is default
                label = candle.date.substr(5, 5);
                break;
        }

        //Put labels into x axis group
        x_axisLabels.push_back(label);
    }

    //Return calculated values
    return std::make_pair(colWidth, x_axisLabels);
};

std::tuple<double, double, double, std::vector<double>> RenderChart::generateYAxis(std::vector<Candlestick>& visibleCandles)
{
    //GENERATE Y AXIS
    //Variable for the groups highest value
    double groupHigh = -200;

    //Variable for the groups lowest value
    double groupLow = 200;

    //If when going through entries, a new high or low is encountered, assign to variable
    for(const auto& candle : visibleCandles) 
    {
        if (candle.high > groupHigh) groupHigh = candle.high;
        if (candle.low < groupLow) groupLow = candle.low;
    }
    
    //Convert to whole numbers for chart
    double yMax = std::ceil(groupHigh);
    double yMin = std::floor(groupLow);

    //How many values to have on y axis
    double noOfYValues = 30;

    //Calculate spacing for each y value
    double ySpacing = (yMax - (yMin)) / (noOfYValues - 1);

    //Create y axis vector to hold values
    std::vector<double> y_axisLabels;

    //Loop through the number of y values needed and create y values for chart
    for(int i = 0; i < noOfYValues; ++i) 
    {

        //Calculate y value
        double label = yMax - (i * ySpacing);

        //Push to y values vector
        y_axisLabels.push_back(label);
    }

    return std::make_tuple(noOfYValues, yMax, yMin, y_axisLabels);
};

int RenderChart::mapToRow(double value, double yMax, double yMin, int noOfYValues)
{
    //Calculate how far the value is from the max value
    double differenceFromTop = yMax - value;

    //Calculate full range of the y axis
    double yRange = yMax - yMin;

    //Calculate relative position on y axis as percentage
    double posYAxis = differenceFromTop / yRange;

    //Calculate closest row number to place value onto
    double rowNumber = posYAxis * (noOfYValues - 1);

    //Round the row number
    int roundedRowNumber = static_cast<int>(std::round(rowNumber));

    //Return row number
    return roundedRowNumber;
};

void RenderChart::drawCandlesticks(
            std::vector<Candlestick>& visibleCandles,
            std::vector<std::vector<char>>& createChart, 
            double yMax, 
            double yMin, 
            int noOfYValues)
{
    //Candlestick logic
    for(int column = 0; column < visibleCandles.size(); ++column) 
    {
        //Select a single candlestick from the candlesticks vector
        const auto& candlestick = visibleCandles[column];

        //Obtain each row number for each value of the candlestick
        int openRow  = mapToRow(candlestick.open, yMax, yMin, noOfYValues);
        int closeRow = mapToRow(candlestick.close, yMax, yMin, noOfYValues);
        int highRow  = mapToRow(candlestick.high, yMax, yMin, noOfYValues);
        int lowRow   = mapToRow(candlestick.low, yMax, yMin, noOfYValues);

        //Top of the candlestick body
        int top;

        //Bottom of the candlestick body
        int bottom;

        //Figure out which value is larger so we always draw the candlestick body correctly (top to bottom)
        if(openRow < closeRow) 
        {
            top = openRow;
            bottom = closeRow;
        } 
        else 
        {
            top = closeRow;
            bottom = openRow;
        }

        //Draw the wick from the highest row to the lowest row
        //Gets overriden by body drawing (after)
        for(int row = highRow; row <= lowRow; ++row)
        {
            createChart[row][column] = char(124);
        }

        //Draw the candle body from calculated top to bottom
        for(int row = top; row <= bottom; ++row)
        {
            createChart[row][column] = char(219);
        }
    }
};

std::vector<double> RenderChart::calcMonthlyAverages(std::string country, std::string yearChoice, std::vector<TemperatureEntry> allTemperatureData)
{
    //Hold averages of temp data
    std::vector<double> monthlyAverages;

    //Loop through each month
    for(int month = 1; month <= 12; ++month)
    {
        //Convert month to string and add a 0 if under 10
        //Used to access data set
        std::string monthStr = (month < 10 ? "0" : "") + std::to_string(month);

        //Hold temps of each month
        std::vector<double> temps;

        //Loop through every entry in the temperature data
        for(const auto& entry : allTemperatureData)
        {
            //Obtain year, month and country
            std::string entryYear = entry.date.substr(0, 4);
            std::string entryMonth = entry.date.substr(5, 2);
            std::string entryCountry = entry.country.substr(0, 2);

            //If the same country and month and year of entry is less than year chosen
            if(entryCountry == country && entryMonth == monthStr && std::stoi(entryYear) < std::stoi(yearChoice))
            {
                //Add temperature to temps vector
                temps.push_back(entry.value);
            }
        }

        //If temps vector is not empty
        if(!temps.empty())
        {
            //Variable to hold calculation
            double sum = 0.0;

            //For each temp in temperature vector
            for(double temp : temps)
            {
                //Add them together in sum
                sum += temp;
            }

            //Calculate average and add to monthly avg vector
            double calculatedAvg = sum / temps.size();
            monthlyAverages.push_back(calculatedAvg);
        }
        else
        {
            std::cout << "WeatherMain::predictTempData says: Could not calculate average temp of month: " << month << std::endl;
            std::cout << "Month average will be 0." << std::endl;
            monthlyAverages.push_back(0);
        }
    }

    return monthlyAverages;
};

void RenderChart::printChart(
            const std::vector<std::vector<char>>& createChart,
            const std::vector<double>& y_axisLabels,
            const std::vector<std::string>& x_axisLabels,
            const std::vector<Candlestick>& visibleCandles,
            int colWidth,
            int noOfYValues
        )
{
    //Print the chart (top to bottom, row by row)
    //Print y axis by iterating through rows and printing y value on each one
    for(int row = 0; row < noOfYValues; ++row) 
    {
        //Print y axis with values
        std::cout << 
            std::setw(6) << 
            std::fixed << 
            std::setprecision(1) << 
            y_axisLabels[row] << " " << char(186); //Line for y axis

        //Print candlestick column data
        //Loop through the rows
        for(int column = 0; column < createChart[row].size(); ++column) {

            //Capture symbol assigned to the column previously
            const auto& symbol = createChart[row][column];

            //If it is a candlestick body
            if(symbol == char(219))
            {
                //Capture the candlestick
                const auto& candlestick = visibleCandles[column];

                //Check if it is bullish (green, true) or bearish (red, false)
                bool bullish = candlestick.close > candlestick.open;

                //Depending on bullish boolean, print green or red then reset colour for future outputs
                std::cout << (bullish ? GREEN : RED) << std::setw(colWidth) << symbol << RESET << ' ';

            } 
            //If it is not a wick or a candlestick body, print a blank space
            else 
            {
                std::cout << std::setw(colWidth) << symbol << ' ';
            }
        }

        //End the line
        std::cout << std::endl;
    }

    //Print x axis with values
    //Add spacing before beginning of line (to line up with y axis)
    std::cout << "       + ";

    //Loop through the x axis vector and print a line firstly
    for(int i = 0; i < x_axisLabels.size(); ++i)
    {
        for(int j = 0; j < (colWidth+1); ++j)
        {
            std::cout << char(205);
        }
    }
        
    std::cout << std::endl;

    //Print x axis values
    //Print spacing to align properly
    std::cout << "        ";

    //For each label in the vector
    for(const auto& label : x_axisLabels)
    {
        //print the label
        std::cout << std::setw(colWidth) << label << ' ';
    }

    std::cout << std::endl;
};

void RenderChart::printGraph(
            std::vector<std::vector<char>>& graph,
            const std::vector<double>& selectedValues,
            const std::vector<std::string>& x_axisLabels,
            double yMax, 
            double yMin, 
            int noOfYValues,
            int colWidth
        )
{
    //Plot data points on graph
    //Loop through number of values we have
    for(int i = 0; i < selectedValues.size(); ++i)
    {
        //Map the value to a row in the graph
        int row = RenderChart::mapToRow(selectedValues[i], yMax, yMin, noOfYValues);

        //Assign it a value to display (instead of empty space)
        for(int r = row; r < noOfYValues; ++r)
        {
            graph[r][i] = '*';
        }
    }

    //Calculate spacing between y values
    double ySpacing = (yMax - (yMin)) / (noOfYValues - 1);

    //Print graph top to bottom, row by row
    //Loop over number of y values (rows)
    for(int row = 0; row < noOfYValues; ++row)
    {
        //Calculate y values using correct spacing
        double yValues = yMax - ySpacing * row;

        //Print y axis with values
        std::cout << 
            std::setw(6) << 
            std::fixed << 
            std::setprecision(1) << 
            yValues << " | "; //Bar for y axis

        //Go through the temp data
        for(int col = 0; col < selectedValues.size(); ++col)
        {
            //Print the appropriate symbols
            std::cout << std::setw(colWidth) << graph[row][col] << ' ';
        }

        std::cout << std::endl;
    }

    //Printing the x axis line
    //Starting place to align with y axis
    std::cout << "       + ";
    //Loop over the values obtained
    for(int i = 0; i < selectedValues.size(); ++i)
    {
        //Add a dash for each value for the x axis
        for (int j = 0; j < (colWidth+1); ++j)
        {
            std::cout << '-';
        }
    }
    std::cout << std::endl;

    //Printing x axis values
    //Empty space to align properly
    std::cout << "        ";
    
    //Loop through values
    for(int i = 0; i < selectedValues.size(); ++i)
    {
        //Add a date for each value
        std::cout << std::setw(colWidth) << std::right << x_axisLabels[i] << ' ';
    }
    std::cout << std::endl;
};