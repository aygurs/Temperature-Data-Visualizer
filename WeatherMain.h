#pragma once
#include <string>
#include <vector>
#include <algorithm>
#include "TemperatureEntry.h"
#include "Candlestick.h"

class WeatherMain
{
    public:
        WeatherMain();

        /*Starts the temperature simulator.*/
        void init();

        /*Stores all temperature data from file.*/
        std::vector<TemperatureEntry> allTemperatureData;

    private:

        /*Prints the option menu for the user.*/
        void printMenu();

        /*Prints an invalid message if user does not choose a number between 1-4. */
        void printInvalidInput();

        /*Prompts user to choose filtering options before displaying data in a table.*/
        void candlestickDataTable();

        /*Prompts user to choose filtering options before displaying data in a plot chart.*/
        void candlestickDataChart();

        /*Prompts user to choose filtering options before displaying data in a graph.*/
        void candlestickDataGraph();

        /*Makes a prediction of the temperature of a specific month and year based off of historical data.*/
        void predictTempData();

        /*Processes the user choice and handles the response appropriately. */
        void processUserChoice(int userChoice);
};