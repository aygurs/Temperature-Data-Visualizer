# Weather App - Temperature Data Analyzer

A C++ console application for analyzing and visualizing European temperature data (1980-2019) using candlestick charts, graphs, and data tables.

## Features

- **Candlestick Charts**: View temperature data in candlestick format (Open, High, Low, Close)
- **Data Visualization**: Display temperature trends as text-based charts and graphs
- **Temperature Prediction**: Predict monthly average temperatures based on historical data
- **Flexible Filtering**: 
  - Filter by country
  - Choose timeframe (daily, weekly, monthly, yearly)
  - Select custom date ranges
- **Multiple View Formats**:
  - Data table with precise values
  - Candlestick plot chart
  - Line graph visualization

## Project Structure

```
├── main.cpp                          # Entry point
├── WeatherMain.h/cpp                 # Main application controller
├── CSVReader.h/cpp                   # CSV file parsing
├── Candlestick.h/cpp                 # Candlestick data structure
├── CandlestickGenerator.h/cpp        # Generates candlestick data
├── RenderChart.h/cpp                 # Chart rendering logic
├── TemperatureEntry.h/cpp            # Temperature data structure
├── UserInputHandler.h/cpp            # User input management
├── InputValidator.h/cpp              # Input validation
├── .vscode/tasks.json                # Build configuration
└── dataset/
    └── temp_data_1980-2019.csv  # Source data
```

## Requirements

- C++17 or later
- MinGW-w64 compiler (g++) or equivalent

## Building

### Option 1: Using Visual Studio Code (Recommended)
Press **Ctrl+Shift+B** to build using the configured build task.

### Option 2: Manual Compilation
```bash
g++ -o weather_app.exe main.cpp Candlestick.cpp CandlestickGenerator.cpp \
    CSVReader.cpp InputValidator.cpp RenderChart.cpp TemperatureEntry.cpp \
    UserInputHandler.cpp WeatherMain.cpp
```

## Running the Application

```bash
./weather_app.exe
```

The application will load temperature data from the CSV file and present an interactive menu with the following options:

1. **View Data as Table**: Display temperature data in a formatted table
2. **View Data as Candlestick Chart**: Visualize temperature ranges using candlestick format
3. **View Data as Graph**: Plot temperature trends as a line graph
4. **Predict Temperatures**: Predict average monthly temperatures for a specific year

## Usage Flow

1. Start the application
2. Select a display option from the menu
3. Choose a country from the available list
4. Select a timeframe (daily, weekly, monthly, yearly)
5. Specify a date range
6. View the generated visualization

For the prediction feature, you'll select a country and year instead of a timeframe and date range.

## Data Source

- **Dataset**: European temperature data (1980-2019)
- **Format**: CSV with daily temperature readings for 28 European countries
- **File**: `dataset/temp_data_1980-2019.csv`

## Key Classes

- **WeatherMain**: Main application logic and menu system
- **CSVReader**: Parses CSV file and converts data to `TemperatureEntry` objects
- **CandlestickGenerator**: Generates candlestick data based on user filters
- **RenderChart**: Handles all chart and graph rendering
- **UserInputHandler**: Manages user input with prompts
- **InputValidator**: Validates user choices

## Architecture Notes

This project follows Object-Oriented Programming principles with:
- Static methods for utility functions
- Clear separation of concerns
- Exception handling for file I/O and data parsing
- Structured data using custom classes

## Troubleshooting

**"g++ is not recognized"**
- Ensure MinGW-w64 is installed
- Add `C:\mingw64\bin` to your Windows PATH environment variable
- Restart your terminal after updating PATH

**"Cannot open dataset file"**
- Ensure `dataset/temp_data_1980-2019.csv` exists in the project root
- Run the application from the project directory

## Future Enhancements

- Export data to file formats (CSV, JSON)
- Additional statistical analysis
- GUI interface
- Real-time data updates
- More visualization types
