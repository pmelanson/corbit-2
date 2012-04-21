#include "Character.h"

Character::Character( ){

    x = 0;
    y = 0;
    
    visible = true;
    dead = false;

}

int Character::GetX(){

    return x;

}

int Character::GetY(){

    return y;

}

void Character::SetX( int newValue, BITMAP* tempBitmap, BITMAP* spriteToDraw){

    if( newValue <= MAX_X && newValue >= MIN_X && visible){
        EraseOldSprite( tempBitmap);
        x = newValue;
        DrawNewSprite( tempBitmap, spriteToDraw);
    } else {
    
        x = newValue;
           
    }

}

void Character::SetY( int newValue, BITMAP* tempBitmap, BITMAP* spriteToDraw){

    if( newValue <= MAX_Y && newValue >= MIN_Y && visible){
        EraseOldSprite( tempBitmap);
        y = newValue;
        DrawNewSprite( tempBitmap, spriteToDraw);
    } else {
    
        y = newValue;
           
    }

}

bool Character::IsDead(){

    return dead;
}

void Character::SetDead(bool newValue, BITMAP* tempBitmap){

    visible = (!newValue);
    dead = newValue;

    if(dead){
        EraseOldSprite( tempBitmap);
        SetY( 700, NULL, NULL);
        SetX( 700, NULL, NULL);
    }

}

void Character::EraseOldSprite( BITMAP* tempBitmap){

    rectfill( tempBitmap, GetX(), GetY(), GetX() + 20, GetY() + 20, makecol ( 0, 0, 0));

}

void Character::DrawNewSprite( BITMAP* tempBitmap, BITMAP* spriteToDraw){

    draw_sprite( tempBitmap, spriteToDraw, GetX(), GetY());

}
