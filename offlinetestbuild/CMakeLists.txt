cmake_minimum_required(VERSION 4.3)
project(offlinetestbuild)

set(CMAKE_CXX_STANDARD 20)

add_executable(offlinetestbuild main.cpp
        card.cpp
        card.h
        arcaneKnowledgeRepository.h
        enemy.cpp
        enemy.h
        info.cpp
        info.h
        player.cpp
        player.h
        playerDeck.cpp
        playerDeck.h)
