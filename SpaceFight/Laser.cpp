#include "Laser.h"
 
Laser::Laser(){
               
}

void Laser::CheckCollision(Character objectsToCheck[], BITMAP* tempBitmap){

    for( int i = 0; i <= 9; i++){
    
        if( GetX() == objectsToCheck[i].GetX() && GetY() == objectsToCheck[i].GetY()){
        
            SetDead( true, tempBitmap);
            objectsToCheck[i].SetDead( true, tempBitmap);
            break;
        
        } else if ( GetY() == MIN_Y ){
        
            SetDead( true, tempBitmap);
            break;
        }
        
    
    }
    

}

bool Laser::CheckCollision(Character player, BITMAP* tempBitmap){

    if( GetX() == player.GetX() && GetY() == player.GetY()){
    
        SetDead( true, tempBitmap);
        player.SetDead( true, tempBitmap);
        return true;
            
    } else if ( GetY() == MAX_Y){
    
        SetDead( true, tempBitmap);
        return false;
    
    }

    return false;
}
