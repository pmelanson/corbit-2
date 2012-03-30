#ifndef SHIP_H
#define SHIP_H
#include "Character.h"

class Ship: public Character{

public:
       Ship();
       
       int GetLives();
       void SetLives( int newValue);
       

private:
        int lives;

};

#endif
