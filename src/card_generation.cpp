#include "card_generation.h"
#include <fstream>
#include <sstream>
#include <map>

using namespace std;

vector<card> loadCards(string filename)
{
    vector<card> cards;
    ifstream file(filename);

    if (!file.is_open())
        return cards;

    string line;
    map<string, string> data;

    while (getline(file, line))
    {
        // Start of a new card
        if (line == "[Card]")
        {
            if (!data.empty())
            {
                // Create card from collected data
                card c(
                    stoi(data["ID"]),
                    data["Name"],
                    stoi(data["Cost"]),
                    data["Exclusive"] == "true"
                );



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