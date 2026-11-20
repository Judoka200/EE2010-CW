cmake_minimum_required(VERSION 4.3)
project(week7sys)

set(CMAKE_CXX_STANDARD 20)

add_executable(EE2010-CW main.cpp
        arcaneKnowledgeRepository.h
        card.cpp
        card.h
        enemy.cpp
        enemy.h
        info.cpp
        info.h
        Player.cpp
        Player.h
        playerDeck.cpp
        playerDeck.h
)

