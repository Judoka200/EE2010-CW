#include "card_generation.h"
#include <fstream>
#include <sstream>
#include <map>
#include "arcaneKnowledgeRepository.h"

using namespace std;

vector<card> loadCards(string filename)
{
    vector<card> cards;
    ifstream file(filename);

    //check file opened 
    if (!file.is_open()) 
        return cards;

    string line;              // stores each line from text file
    map<string, string> data; // key:value pair. "ID: 1" etc


    while (getline(file, line))
    {
        // Start of a new card
        if (line == "[Card]")
        {
            if (!data.empty())
            {
                // Create card object from collected data
                card c(
                    stoi(data["ID"]),
                    data["Name"],
                    stoi(data["Cost"]),
                    data["Exclusive"] == "true"
                );

                c.addAttack(stoi(data["Attack"])); 

                c.addDef(stoi(data["Defence"]));

                c.addEffect(
                    stoi(data["Poison"]), 
                    stoi(data["Heal"])
                );

                cards.push_back(c);
                data.clear();
            }
        }
        else
        {
            // Extract key and value
            stringstream ss(line);
            string key, value;

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
    {
        card c(
            stoi(data["ID"]),
            data["Name"],
            stoi(data["Cost"]),
            data["Exclusive"] == "true"
        );

        c.addAttack(stoi(data["Attack"])); 

        c.addDef(stoi(data["Defence"]));

        c.addEffect(
            stoi(data["Poison"]),
            stoi(data["Heal"])
        );

        cards.push_back(c);
    }

    file.close();
    return cards;
}