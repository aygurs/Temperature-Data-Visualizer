#pragma once
#include <vector>
#include <string>
#include "Candlestick.h"
#include "TemperatureEntry.h"

class RenderChart
{
    public:
        RenderChart();

        /*Generate x axis for the candlestick chart.*/
        static std::pair<int, std::vector<std::string>> generateXAxis(std::string timeframe, std::vector<Candlestick>& visibleCandles);

        /*Generate the y axis for the candlestick chart.*/
        static std::tuple<double, double, double, std::vector<double>> generateYAxis(std::vector<Candlestick>& visibleCandles);

        /*Assigns temperatures from candlestick data to row numbers.*/
        static int mapToRow(double value, double yMax, double yMin, int noOfYValues);

        /*Calculates the monthly averages for the year, for the prediction function.*/
        static std::vector<double> calcMonthlyAverages(std::string country, std::string yearChoice, std::vector<TemperatureEntry> allTemperatureData);

        /*Generates how the candlesticks will look based off of candlestick data.*/
        static void drawCandlesticks(
            std::vector<Candlestick>& visibleCandles,
            std::vector<std::vector<char>>& createChart, 
            double yMax, 
            double yMin, 
            int noOfYValues);

        /*Prints the chart created using candlestick data.*/
        static void printChart(
            const std::vector<std::vector<char>>& createChart,
            const std::vector<double>& y_axisLabels,
            const std::vector<std::string>& x_axisLabels,
            const std::vector<Candlestick>& visibleCandles,
            int colWidth,
            int noOfYValues
        );

        /*Prints the text- based graph created using candlestick data.*/
        static void printGraph(
            std::vector<std::vector<char>>& graph,
            const std::vector<double>& selectedValues,
            const std::vector<std::string>& x_axisLabels,
            double yMax, 
            double yMin, 
            int noOfYValues,
            int colWidth
        );
};