/**
 * @file PS3Controller.h
 * @brief Connects PS3 controller and gets input from it
 */

#include <PS3BT.h>

#define STATE_ARRAY_SIZE 1

#ifndef PS3CONTROLLER_H
#define PS3CONTROLLER_H


class PS3Controller {
    private:
    //! Boolean that stores current controller connection state.
    bool connected; 

    /**
     * @brief 8-bit integer array that stores the controller state.
     * 
     * Stores the controller's current inputs in an array whose size is defined
     * at the top of the file by STATE_ARRAY_SIZE. The values are stored as
     * 8-bit integers, which, in this case, come directly from the controller.
     * 
     * As of 0.2, the array's corresponding values are:
     * [0] : Throttle servo
     * [1] : Steering servo
     * 
     */
    uint8_t controller_state[STATE_ARRAY_SIZE];

    //! USB port object.
    USB Usb;

    //! Bluetooth dongle object.
    BTD Btd;

    //! PS3 bluetooth connection object.
    PS3BT PS3;
    
    public:
    //! Construct a new PS3Controller object.
    PS3Controller();

    /**
     * @brief Returns controller connection state.
     * 
     * @return true Controller is connected.
     * @return false Controller is disconnected.
     */
    bool isConnected();

    //! Updates the controller_state array.
    void updateControllerState();

    /**
     * @brief Returns controller_state array as pointer.
     * @return uint8_t* 8-bit array for controller inputs.
     */
    uint8_t* getControllerState();

    /**
     * @brief Returns controller_state size.
     * @return uint8_t Size of controller_state.
     */
    uint8_t getControllerStateSize();

    /**
     * @brief Setup function for PS3Controller.
     * 
     * Sets up the PS3Controller for use. Returns true if the USB port
     * successfully initializes, returns false if not
     * 
     * @return true USB initialized successfully
     * @return false USB failed to initialize
     */
    bool setupPS3Controller();
    
    /**
     * @brief PS3Controller loop task.
     * 
     * PS3Controller loop task keeps the USB port open and checks if the
     * controller is connected, updating the controller's 'connected' value.
     * 
     */
    void loopTask();
};

#endif // PS3CONTROLLER_H
