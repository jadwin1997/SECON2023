#include "action.h"

Action::Action()
{
    actionID = NULL;
    for(uint8_t i = 0; i < sizeof(&val)/sizeof(int32_t); i++){
        val[i] = NULL;
    }
}

Action::Action(const char& actionID, const int32_t val[]){
    this->actionID = actionID;

    for(uint8_t i = 0; i < sizeof(&this->val)/sizeof(int32_t); i++){
        this->val[i] = NULL;
    }

    for(uint8_t i = 0; i < sizeof(&val)/sizeof(int32_t); i++){
        this->val[i] = val[i];
    }

}
