/**
 *
 * INIT_HEADER:
 *
 * Loads up predefined competition parameters to be used by
 * bot systems as constants and references. Also defines the
 * {@link Initializer#buildBot} method to call the bot
 * contructor and actually initialize the bot object in the
 * {@link Initializer#initialize} method to be called in
 * main.
 *
 */


#include <board.h>
#include <itemmap.h>
#include <sensor.h>
#include <bot.h>

#include <random>

#ifndef INITIALIZER_H
#define INITIALIZER_H

// Seed number for random item distribution
//int seedNum = 0;
// Random number generator for item distribution
//std::minstd_rand0 gen;


/**
 * @brief buildBoard
 * @param board
 * Initializer for board regions
 */
void buildBoard(Board board[14]){
    float t_posA[2], t_posB[2];

    board[0] = Board(); // Creates main board background (with walls space handled)

    t_posA[0] = 1; t_posA[1] = 1; t_posB[0] = 10; t_posB[1] = 13;
    board[1] = Board("alligatorAquarium", t_posA, t_posB, "ff0000");

    t_posA[0] = 1; t_posA[1] = 35; t_posB[0] = 10; t_posB[1] = 47;
    board[2] = Board("manateeAquarium", t_posA, t_posB, "00ff00");

    t_posA[0] = 11.5; t_posA[1] = 3; t_posB[0] = 12.5; t_posB[1] = 11;
    board[3] = Board("whiteCrossBottomVert", t_posA, t_posB, "ffffff");

    t_posA[0] = 10; t_posA[1] = 6.5; t_posB[0] = 14; t_posB[1] = 7.5;
    board[4] = Board("whiteCrossBottomHoriz", t_posA, t_posB, "ffffff");

    t_posA[0] = 11.5; t_posA[1] = 37; t_posB[0] = 12.5; t_posB[1] = 45;
    board[5] = Board("whiteCrossTopVert", t_posA, t_posB, "ffffff");

    t_posA[0] = 10; t_posA[1] = 40.5; t_posB[0] = 14; t_posB[1] = 41.5;
    board[6] = Board("whiteCrossTopHoriz", t_posA, t_posB, "ffffff");

    t_posA[0] = 33; t_posA[1] = 7; t_posB[0] = 2.5; t_posB[1] = 2;
    board[7] = Board(true, "leftCircle", t_posA, t_posB, "ffffff");

    t_posA[0] = 42; t_posA[1] = 1; t_posB[0] = 54; t_posB[1] = 13;
    board[8] = Board("startZone", t_posA, t_posB, "ffffff");

    t_posA[0] = 48; t_posA[1] = 35; t_posB[0] = 7; t_posB[1] = 2.5;
    board[9] = Board(true, "pond", t_posA, t_posB, "000099");

    t_posA[0] = 48; t_posA[1] = 35; t_posB[0] = 2.5; t_posB[1] = 2;
    board[10] = Board(true, "pondInnerCircle", t_posA, t_posB, "ffffff");

    t_posA[0] = 63; t_posA[1] = 7; t_posB[0] = 2.5; t_posB[1] = 2;
    board[11] = Board(true, "rightCircle", t_posA, t_posB, "ffffff");

    t_posA[0] = 72; t_posA[1] = 1; t_posB[0] = 88; t_posB[1] = 10;
    board[12] = Board("recycleLower", t_posA, t_posB, "777777");

    t_posA[0] = 72; t_posA[1] = 38; t_posB[0] = 88; t_posB[1] = 47;
    board[13] = Board("recycleUpper", t_posA, t_posB, "777777");
}

/**
 * @brief buildBot
 * @param size
 * @param sensors
 * @return
 */
Bot buildBot(const float size[3], Sensor sensors[]){
    return Bot(size, sensors);
}

/**
 * @brief initialize
 * @param board
 * @param bot
 */
void initialize(Board board[14], Bot& bot){
    buildBoard(board);
    bot = buildBot(botSize, sensorList);
}

#endif // INITIALIZER_H
