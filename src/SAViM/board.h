#include <string>
#include <cmath>

#ifndef BOARD_H
#define BOARD_H

class Board
{
private:
    /**
     * @brief pos
     * Position of board region's center (x,y).
     */
    float pos[2],
    /**
     * @brief dim
     * Dimensions of board region (x,y).
     */
    dim[2];
    /**
     * @brief name
     * Name of region.
     */
    std::string name,
    /**
     * @brief color
     * Object color in hex.
     */
    color;
    /**
     * @brief isRound
     * Utility bool. Changes selected object definition to simplify creation of both rectangular and round regions.
     */
    bool isRound;

    // Private getter called methods
    /**
     * @brief getArea
     * Called by public getter. @see getArea()
     * @param isRound
     * @return Area of the region.
     */
    float getArea(const bool& isRound);
    /**
     * @brief getPerim
     * Called by public getter. @see getPerim()
     * @param isRound
     * @return Perimeter of the region.
     */
    float getPerim(const bool& isRound);

    // Derived setters
    /**
     * @brief findCenter
     * Gets the centerpoint of rectangles defined via corner coordinates.
     * @param pos   Position array argument to be modified.
     * @param posA  Coordinates array of one corner (x,y).
     * @param posB  Coordinates array of opposite corner (x,y).
     */
    void findCenter(float pos[2], const float posA[2], const float posB[2]);

public:
    // Constructors
    /**
     * @brief Board
     * Default constructor. Creates black base with full board dimensions.
     */
    Board();
    /**
     * @brief Board
     * Rectangular region constructor.
     * @param name  Region name.
     * @param posA  Coordinate array of one corner (x,y).
     * @param posB  Coordinate array of opposite corner(x,y).
     * @param color Region color as a hex value.
     */
    Board(const std::string& name, const float posA[2], const float posB[2], const std::string& color);
    /**
     * @brief Board
     * Circular region constructor.
     * @param isRound   Utility bool. Simplifies definition distinction.
     * @param name      Region name.
     * @param posCenter Coordinates of region's center.
     * @param radius    Array of inner and outer radii (inner, outer).
     * @param color     Region color as a hex value.
     */
    Board(const bool& isRound, const std::string& name, const float posCenter[2], const float radius[2], const std::string& color); //

    // Getter methods
    /**
     * @brief getName
     * @return Region's name.
     */
    std::string getName();
    /**
     * @brief getPos
     * Returns x- and y-coordinates to the arg. array.
     * @param output    Array to receive center coordinates.
     */
    void getPos(float output[2]);
    /**
     * @brief getArea
     * Gives region area, accounting for hollow centers.
     * @return Area of the region.
     */
    float getArea(){return getArea(isRound);}
    /**
     * @brief getPerim
     * @return Perimeter of the region.
     */
    float getPerim(){return getPerim(isRound);}
    /**
     * @brief getX
     * @return Length of region on x-axis.
     */
    float getX();
    /**
     * @brief getY
     * @return Width of region on y-axis.
     */
    float getY();
    /**
     * @brief round
     * @return True if circular, or false if rectangular.
     */
    bool round(){return isRound;};
};

#endif // BOARD_H
