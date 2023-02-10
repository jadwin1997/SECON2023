#include <bot.h>

#ifndef ACTION_H
#define ACTION_H


class Action
{
private:


public:
    Action();

    /**
     * @brief Action
     * Standard constructor.
     * @param actionID  Action ID key char specifying task to be perform.
     * @param val       Array value containing data points for passing parameters.
     */
    Action(const char& actionID, const int32_t val[]);

    /**
     * @brief ~Action
     * Simple action item deconstructor. Deletes relevant pointers.
     */
    ~Action(){delete next;}

    /**
     * @brief actionID
     * ID of action to be performed. Current actions listed:
     *
     * m = move
     * s = stop
     * t = turn
     * 2 = moveTo
     *
     */
    char actionID;

    /**
     * @brief val
     * Stored data values.
     */
    uint16_t val[2] = {};

    /**
     * @brief next
     * Points to next action object.
     */
    Action* next;
};

#endif // ACTION_H
