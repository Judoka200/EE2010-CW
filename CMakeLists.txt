cmake_minimum_required(VERSION 4.3)
project(EE2010_CW)

set(CMAKE_CXX_STANDARD 20)

add_executable(EE2010_CW main.cpp
        arcaneKnowledgeRepository.h
        card.cpp
        card.h
        cardGeneration.cpp
        cardGeneration.h
        enemy.cpp
        enemy.h
        info.cpp
        info.h
        Player.cpp
        Player.h
        playerDeck.cpp
        playerDeck.h
        uiBlocks.h
        enemyGeneration.cpp
        enemyGeneration.h
)