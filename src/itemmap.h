#include <bot.h>

#include <string>
#include <cmath>

#ifndef ITEMMAP_H
#define ITEMMAP_H


class itemMap
{
private:
    /**
     * @brief itemMap
     * Private constructor for a detected object.
     * @param bot       Pointer to main bot object.
     * @param type      Predicted type determined from sensor data.
     * @param color     Color detected via OpenCV.
     * @param estAng    Estimated angle from front of bot.
     */
    itemMap(Bot* bot, const char& type, const std::string& color, const float estAng[]);

    /**
     * @brief estLoc
     * Estimated location of item on board (x,y).
     */
    float estLoc[2],
    /**
     * @brief estAng
     * Radial distance from bot (distance, c.c.w. angle from front).
     */
    estAng[2];

    /**
     * @brief color
     * Sensed color (RGB hex value).
     */
    std::string color;

    /**
     * @brief type
     * Item type name.
     */
    char type;

    /**
     * @brief bot
     * Pointer to main bot object.
     */
    Bot* bot;

public:
    /**
     * @brief itemMap
     * Default constructor.
     * @return Unidentified detected object.
     */
    itemMap();

    /**
     * @brief itemMap
     * Primary public constructor. Calls private constuctor with bot object pointer. Takes in all known parameters, unknowns may be left as null values.
     * @param type      Predicted type determined from sensor data.
     * @param color     Color detected via OpenCV.
     * @param estAng    Estimated angle from front of bot.
     * @return Item with known data points with unknowns left empty.
     */
    itemMap(const char& type, const char& color, const float estAng[]);

    /**
     *
     */
    void updateAng(const float botPos[2]);

    void updatePos(const float estAng[2]);

    void initBot(Bot& bot);
};

#endif // ITEMMAP_H
