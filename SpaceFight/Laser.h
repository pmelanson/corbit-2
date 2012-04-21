#ifndef LASER_H
#define LASER_H 1
#include "Character.h"
#include "Ship.h"

class Laser: public Character{

public:
       Laser();
       void CheckCollision(Character objectsToCheck[], BITMAP* tempBitmap);
       bool CheckCollision(Character player, BITMAP* tempBitmap);
       

};

#endif
