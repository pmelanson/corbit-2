#ifndef CHARACTER_H
#define CHARACTER_H
#define MAX_X 620
#define MIN_X 0
#define MAX_Y 460
#define MIN_Y 0
#include <allegro.h>

class Character{

public:
       Character();

       int GetX();
       void SetX( int newValue, BITMAP* tempBitmap, BITMAP* spriteToDraw);
       
       int GetY();
       void SetY( int newValue, BITMAP* tempBitmap, BITMAP* spriteToDraw);
       
       void EraseOldSprite( BITMAP* tempBitmap);
       void DrawNewSprite( BITMAP* tempBitmap, BITMAP* spriteToDraw);
       
       bool IsDead();
       void SetDead( bool newValue, BITMAP* tempBitmap);

protected:
        int x;
        int y;
        
        bool visible;
        bool dead;

};

#endif
