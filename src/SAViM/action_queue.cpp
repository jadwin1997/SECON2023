#include "action_queue.h"

Action_Queue::Action_Queue(){
    head = nullptr;
    bot = nullptr;
}

Action_Queue::Action_Queue(Bot* bot){
    head = nullptr;
    this->bot = bot;
}

void Action_Queue::queue(Action action){

    if(head==nullptr){
        head = &action;
    }else{
        Action* last = head;
        while(last->next != nullptr){
            last = last->next;
        }
        last->next = &action;
    }
}

Action_Queue::~Action_Queue(){
    clrQueue();
    delete head;
}

uint8_t Action_Queue::act(){
    if(head == nullptr){
        return 0;
    }else if(head->actionID == '2'){    // Move To action
        float posXY[2] = {NULL, NULL};
        const uint16_t moveToPos[2] = {head->val[0], head->val[1]};

        bot->getPosXY(posXY);

        if(posXY[0] == moveToPos[0] && posXY[1] == moveToPos[1]){
            clrAction(0);
            return 2;
        }else{
            bot->moveTo(moveToPos);
            return 1;
        }
    }else if(head->actionID == 's'){    // Stop action
        if(bot->getSpeed() != 0){
            ;
        }
    }else if(head->actionID == 't'){    // Turn action
        ;
    }else if(head->actionID == 'm'){    // Move action
        ;
    }
    return 3;
}

void Action_Queue::makePriority(const uint8_t index){
    if(index > 0){
        Action* t_action = head;
        Action* t_last = nullptr;

        for(uint8_t i = 0; i < index; i++){
            t_last = t_action;
            t_action = t_action->next;
        }
        t_last->next = t_action->next;
        t_action->next = head;
        head = t_action;
    }
}

void Action_Queue::clrQueue(){
    if(head != nullptr){
        Action* t_action = head;
        Action* t_next = nullptr;

        while(t_action != nullptr){
            t_next = t_action->next;
            delete t_action;
            t_action = t_next;
        }
        delete t_action;
        delete t_next;
        head = nullptr;
    }
}
