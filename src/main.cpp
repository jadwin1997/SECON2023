/***********************************************************
 * SAViM - The SECON Active Virtual Mapping solution
 *
 *
 * Notes:
 *
 * All measurements are standardized as follows:
 *      Time            -   s
 *      Distance        -   in
 *      Speed           -   in/s
 *      Acceleration    -   in/s^2
 *      Angles          -   deg
 **********************************************************/

#include <core_bot_param.h>
#include <initializer.h>
#include <action_queue.h>
#include <vector>
#include <iostream>

using namespace std::chrono_literals;

Board board[14];
Bot bot;
std::vector<itemMap> items;
Action_Queue queue;
uint8_t actStatus = 0;

int main()
{
    initialize(board, bot);
    std::cout << "Initialization complete. Starting parameters:\n" << std::endl;


    float t_pos[2];
    bot.getPosXY(t_pos);

    std::cout << "Starting bot position: (" << t_pos[0] << ", " << t_pos[1] << ")" << std::endl;
    std::cout << "Starting bot angle: " << bot.getAngle() << " degrees" << std::endl;


    /**
     * @brief START_TIME
     * Initialization time for timer referencing.
     */
    const std::chrono::time_point START_TIME = std::chrono::system_clock::now();

    /**
     * @brief timeElapsed
     * Total time
     */
    std::chrono::time_point timePoint = START_TIME;

    while(true){
        // Start sensor frame

        ;

        // End sensor frame
        // Begin action frame
        actStatus = queue.act();

        // End action frame
        std::this_thread::sleep_until(START_TIME + (timePoint - START_TIME) + TIME_STEP);
        timePoint += TIME_STEP;

        // Check if timer is expired
        if(std::chrono::system_clock::now() >= START_TIME + 180s){
            queue.clrQueue();
            queue.queue(Action('s', NULL));
            while(queue.act() == 1){
                // Will loop until the bot is at a complete stop
            }
            break;
        }
        //if(((timePoint - START_TIME) / 1s) == 0)
        std::cout << floor((timePoint - START_TIME) / 60s) << ":" << floor((timePoint - START_TIME) / 1s) << std::endl;
    }
    return 0;
}
