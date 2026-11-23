#include "enemyGeneration.h"
#include <fstream>
#include <sstream>
#include <map>
#include <algorithm>


// returns the value for a key as an int, or 0 if the key is missing/empty
// (data["X"] on a missing key gives "", and stoi("") would throw)
static int toInt(std::map<std::string, std::string>& data, const std::string& key)
{
    if (data.find(key) == data.end() || data[key].empty())
        return 0;
    return std::stoi(data[key]);
}

// build one card from the collected key:value data
static enemy makeEnemy(std::map<std::string, std::string>& data)
{
    // file says TRUE/FALSE, so lowercase it before comparing
    std::string ex = data["Exclusive"];
    std::transform(ex.begin(), ex.end(), ex.begin(), ::tolower);

    return enemy(
        data["NAME"], // adds name
        toInt(data, "HEALTH"), // finds health
        data["FILENAME"] // fills out cards with internal processes
    );
}

std::vector<enemy> loadEnemy(std::string filename)
{
    std::vector<enemy> enemies;
    std::ifstream file(filename);

    // check file opened
    if (!file.is_open())
        return enemies;

    std::string line;                          // stores each line from text file
    std::map<std::string, std::string> data;   // key:value pair. "ID: 1" etc

    while (getline(file, line))// goes through each
    {
        // remove a trailing \r if the file has Windows line endings
        if (!line.empty() && line.back() == '\r')
            line.pop_back();

        // skip blank lines and EOF (otherwise they'd be stored badly)
        if (line.empty() || line == "EOF")
            continue;

        //  checks if a new Enemy has started
        if (line == "[ENEMY]")
        {
            if (!data.empty())
            {
                enemies.push_back(makeEnemy(data));   // create enemy based on data
                data.clear(); // clears data collected
            }
        }
        else
        {
            // Extract key and value
            std::stringstream ss(line);
            std::string key, value;

            getline(ss, key, ':');// grabs everything before :
            getline(ss, value); // grabs data

            // Remove leading space
            if (!value.empty() && value[0] == ' ')
                value.erase(0, 1);

            data[key] = value; // adds to data
        }
    }

    // Load the final card
    if (!data.empty())
        enemies.push_back(makeEnemy(data));

    file.close();
    return enemies;
}