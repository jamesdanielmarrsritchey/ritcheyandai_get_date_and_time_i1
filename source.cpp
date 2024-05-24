#include <ctime>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <string>
#include <map>

int main(int argc, char* argv[]) {
    // Default values for command line arguments
    std::map<std::string, bool> args = {
        {"--year", false},
        {"--month", false},
        {"--day", false},
        {"--hour", false},
        {"--minute", false},
        {"--second", false},
        {"--ampm", false},
        {"--delimiter", false}
    };

    std::string delimiter = ""; // default delimiter is nothing

    // Check if the user has specified any arguments
    bool anyDateTimeArgPassed = false;
    for (int i = 1; i < argc; i += 2) {
        std::string arg = argv[i];
        if (arg == "--delimiter") {
            delimiter = argv[i+1];
            if (delimiter == "\\n") {
                delimiter = "\n";
            } else if (delimiter == "\\t") {
                delimiter = "\t";
            }
        } else if (args.count(arg) > 0) {
            std::string value = argv[i+1];
            args[arg] = value == "true";
            anyDateTimeArgPassed = true;
        }
    }

    // If no arguments are passed, set all arguments to true
    if (argc == 1) {
        for (auto& arg : args) {
            arg.second = true;
        }
    } else if (!anyDateTimeArgPassed) {
        // If only a delimiter is passed, set all other arguments to true
        for (auto& arg : args) {
            if (arg.first != "--delimiter") {
                arg.second = true;
            }
        }
    } else {
        // If any arguments besides a delimiter are passed, set any remaining arguments to false
        for (auto& arg : args) {
            if (arg.first != "--delimiter" && !arg.second) {
                arg.second = false;
            }
        }
    }

    // Get the current date and time
    std::time_t now = std::time(nullptr);
    std::tm* localTime = std::localtime(&now);

    // Break it down into separate variables
    int year = localTime->tm_year + 1900;
    int month = localTime->tm_mon + 1;
    int day = localTime->tm_mday;
    int hour = localTime->tm_hour;
    int minute = localTime->tm_min;
    int second = localTime->tm_sec;

    // Determine AM or PM
    std::string am_pm = hour < 12 ? "AM" : "PM";

    // Adjust hour to 12-hour clock
    if(hour > 12) hour -= 12;
    if(hour == 0) hour = 12;

    // Format hour, minute, second without leading zeros
    std::ostringstream oss_hour, oss_minute, oss_second;
    oss_hour << hour; // remove leading space
    oss_minute << (minute == 0 ? "00" : std::to_string(minute));
    oss_second << (second == 0 ? "00" : std::to_string(second));

    std::string str_hour = "H" + oss_hour.str();
    std::string str_minute = "M" + oss_minute.str();
    std::string str_second = "S" + oss_second.str();

    // Build the output string
    std::ostringstream output;
    if (args["--year"]) output << "Y" << year << delimiter;
    if (args["--month"]) output << "M" << month << delimiter;
    if (args["--day"]) output << "D" << day << delimiter;
    if (args["--hour"]) output << str_hour << delimiter;
    if (args["--minute"]) output << str_minute << delimiter;
    if (args["--second"]) output << str_second << delimiter;
    if (args["--ampm"]) output << am_pm;

    // Get the output string
    std::string output_str = output.str();

    // Remove the trailing delimiter if it exists
    if (!output_str.empty() && output_str.back() == delimiter[0]) {
        output_str.pop_back();
    }

    // Print the output string
    std::cout << output_str << std::endl;

    return 0;
}