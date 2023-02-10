#include <bot.h>
#include <action.h>


#ifndef ACTION_QUEUE_H
#define ACTION_QUEUE_H


class Action_Queue
{
private:

    /**
     * @brief head
     * Head of the queue.
     */
    Action* head;

    /**
     * @brief bot
     * Pointer to main bot object.
     */
    Bot* bot;

public:
    /**
     * @brief Action_Queue
     * Default constructor. Sets head and bot pointers to nullptr.
     */
    Action_Queue();

    /**
     * @brief Action_Queue
     * Main constructor. Takes in pointer to main bot object.
     * @param bot
     */
    Action_Queue(Bot* bot);

    /**
     * @brief queue
     * Queues an action.
     * @param action    Action object with predetermined ID and data value parameters.
     */
    void queue(Action action);

    /**
     * @brief ~Action_Queue
     * Deconstructor. Clears queue (and their subsequent, respective pointers) then deletes head pointer.
     */
    ~Action_Queue();

    /**
     * @brief act
     * Pops and performs next action in queue.
     * @return 8-bit u-int, where 0 == empty queue, 1 == action function called, and 2 == task completed/action cleared
     */
    uint8_t act();

    /**
     * @brief makePriority
     * Moves the action at the given index to the head of the queue.
     * @param index The index of the desired action to prioritize.
     */
    void makePriority(const uint8_t index);

    /**
     * @brief clrAction
     * Deconstructs and removes the action at the given index from the queue.
     * @param index
     */
    void clrAction(const uint8_t index);

    /**
     * @brief clrQueue
     * Deconstructs and removes all action objects from the queue. Does NOT delete the queue itself.
     */
    void clrQueue();
};

#endif // ACTION_QUEUE_H
