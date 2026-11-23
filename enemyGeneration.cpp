#include "enemyGeneration.h"
#include <fstream>
#include <sstream>
#include <map>
#include <algorithm>
#include "arcaneKnowledgeRepository.h"

// returns the value for a key as an int, or 0 if the key is missing/empty
// (data["X"] on a missing key gives "", and stoi("") would throw)
static int toInt(std::map<std::string, std::string>& data, const std::string& key)
{
    if (data.find(key) == data.end() || data[key].empty())
        return 0;
    return std::stoi(data[key]);
}

// !!! change these names to match your ATKTYPE enum in arcaneKnowledgeRepository.h !!!

// build one card from the collected key:value data
static card makeEnemy(std::map<std::string, std::string>& data)
{
    // file says TRUE/FALSE, so lowercase it before comparing
    std::string ex = data["Exclusive"];
    std::transform(ex.begin(), ex.end(), ex.begin(), ::tolower);

    return card(
        toInt(data, "ID"),
        data["Name"],
        toInt(data, "EnergyCost"),
        toInt(data, "AttackVal"),
        toInt(data, "AttackRange"),
        toAttackType(data["attackType"]),
        toInt(data, "PoisonVal"),
        toInt(data, "MadnessVal"),
        toInt(data, "BurningVal"),
        toInt(data, "BleedingVal"),
        toInt(data, "SpecialProperty"),
        toInt(data, "SpecialPropertyVal"),
        ex == "true"
    );
}

std::vector<enemy> loadEnemy(std::string filename)
{
    std::vector<card> cards;
    std::ifstream file(filename);

    // check file opened
    if (!file.is_open())
        return cards;

    std::string line;                          // stores each line from text file
    std::map<std::string, std::string> data;   // key:value pair. "ID: 1" etc

    while (getline(file, line))
    {
        // remove a trailing \r if the file has Windows line endings
        if (!line.empty() && line.back() == '\r')
            line.pop_back();

        // skip blank lines (otherwise they'd be stored as an empty key)
        if (line.empty())
            continue;

        // Start of a new card
        if (line == "[card]")
        {
            if (!data.empty())
            {
                cards.push_back(makeEnemy(data));   // add card object to cards vector
                data.clear();                      // clear data to move onto next card
            }
        }
        else
        {
            // Extract key and value
            std::stringstream ss(line);
            std::string key, value;

            getline(ss, key, ':');
            getline(ss, value);

            // Remove leading space
            if (!value.empty() && value[0] == ' ')
                value.erase(0, 1);

            data[key] = value;
        }
    }

    // Load the final card
    if (!data.empty())
        cards.push_back(makeEnemy(data));

    file.close();
    return cards;
}