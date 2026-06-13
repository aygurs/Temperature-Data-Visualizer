#pragma once
#include <string>

class UserInputHandler
{
    public:
        UserInputHandler();

        /*Gets the user's input choice for the menu and prints it back out to the user. */
        static int getUserMenuChoice();

        /*Obtains users country choice for filtering data.*/
        static std::string userCountryChoice();

        /*Obtains users timeframe choice for filtering data.*/
        static std::string userTimeframeChoice();

        /*Obtains temperature data the user would like to view for the graph only.*/
        static std::string userTempDataChoice();

        /*Obtains year choice for the predicting temperatures feature only.*/
        static std::string userYearChoice();

        /*Obtains users data range choice for filtering data.*/
        static std::pair<int, int> userDataRangeChoice(std::string timeframe);
};