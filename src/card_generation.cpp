#include "card_generation.h"
#include <fstream>
#include <sstream>
#include <map>
#include "arcaneKnowledgeRepository.h"

// using namespace std;

std::vector<card> loadCards(std::string filename)
{
    std::vector<card> cards;
    std::ifstream file(filename);

    //check file opened 
    if (!file.is_open()) 
        return cards;

    std::string line;              // stores each line from text file
    std::map<std::string, std::string> data; // key:value pair. "ID: 1" etc

/*  find new card definition
    fill in data map
    make card object 
*/
    while (getline(file, line))
    {
        // Start of a new card
        if (line == "[Card]")       
        {
            if (!data.empty())
            {
                // Create card object from collected data
                card c(
                    stoi(data["ID"]),   //string to int
                    data["Name"],   //"Name" is the key in the map, data["Name"] will be the value stored in the map for the key 
                    stoi(data["Cost"]),
                    data["Exclusive"] == "true"
                );

                // set card values 
                c.addAttack(stoi(data["Attack"])); 

                c.addDef(stoi(data["Defence"]));

                c.addEffect(
                    stoi(data["Poison"]), 
                    stoi(data["Heal"])
                );

                cards.push_back(c);     // add card object to cards vector 
                data.clear();           // clear data to move onto next card
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