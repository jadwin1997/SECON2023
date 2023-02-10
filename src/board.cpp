#include "board.h"

Board::Board() // Defaults to full board size (minus perimeter wall width)
{
    isRound = false;
    name = "mainBoard";
    dim[0] = (96 - 1 - 3); dim [1] = (48 - 2*(11/16));
    color = "000000";
    float posA[2] = {0 + 1, 0 + 1}, posB[2] = {(96 - 8),(48 - 1)};
    pos[0] = 48; pos[1] = 24;
}

Board::Board(const std::string& name, const float posA[2], const float posB[2], const std::string& color) // Defines board regions via two xy-coordinates and a paint color
{
    // Setting private variable values
    isRound = false;
    this->name = name;
    dim[0] = std::abs(posA[0]-posB[0]); // x dimension
    dim[1] = std::abs(posA[1]-posB[1]); // y dimension
    this->color = color;

    // Calculating center point coordinates (x,y)
    findCenter(pos, posA, posB);
}

Board::Board(const bool& isRound, const std::string& name, const float posCenter[2], const float radius[2], const std::string& color) // Defines circular board regions via center coordinates, an inner and outer radius, and a paint color
{
    // Setting private variable values
    this->isRound = isRound;
    this->name = name;
    pos[0] = posCenter[0]; pos[1] = posCenter[1];
    this->color = color;

    if(isRound) // Is a circle
    {
        if(radius[0]>radius[1]){
            dim[0] = radius[0]; dim[1] = radius[1];
        }else{
            dim[0] = radius[1]; dim[1] = radius[0];
        }
    }
}




// Methods for derived values

void Board::findCenter(float pos[2], float const posA[2], float const posB[2]) // Generates center point value for region position
{
    if(posA[0]<posB[0]) // when x1 < x2
    {
        pos[0] = posA[0]+(dim[0]/2); // x-pos = x1 + length/2
        if(posA[1]<posB[1]){
            pos[1] = posA[1]+(dim[1]/2); // y-pos = y1 + width/2
        }else{
            pos[1] = posB[1]+(dim[1]/2); // y-pos = y2 + width/2
        }
    }
    else // when x1 > x2
    {
        pos[0] = posB[0]+(dim[0]/2); // x-pos = x2 + length/2
        if(posA[1]<posB[1]){
            pos[1] = posA[1]+(dim[1]/2); // y-pos = y1 + width/2
        }else{
            pos[1] = posB[1]+(dim[1]/2); // y-pos = y2 + width/2
        }
    }
}

// Private getter called methods

float Board::getArea(const bool& isRound)
{
    if(!isRound){
        return dim[0]*dim[1];
    }else{
        return 3.141592653*(pow(dim[0],2)-pow(dim[1],2));
    }
}

float Board::getPerim(const bool& isRound)
{
    if(!isRound){
        return 2*dim[0] + 2*dim[1];
    }else{
        return 2*3.141592653*dim[0];
    }
}

// Public getters

std::string Board::getName(){return name;}

void Board::getPos(float output[2]){output[0] = pos[0]; output[1] = pos[1];}

float Board::getX(){return pos[0];}

float Board::getY(){return pos[1];}
