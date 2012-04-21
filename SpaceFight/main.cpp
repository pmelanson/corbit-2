#include <allegro.h>
#include <cstdlib>
#include <time.h>
#include "Character.h"
#include "Ship.h"
#include "Laser.h"

Ship myShip;
Laser myLaser;
Character myEnemies[10];
Laser enemyLasers[10];

BITMAP* buffer;
BITMAP* ship;
BITMAP* enemy;
BITMAP* laser;
BITMAP* enemyLaser;

bool update = true;

void Draw(){

    acquire_screen();
    draw_sprite( screen, buffer, 0, 0);
    release_screen();

    update = false;
    
    rest(50);

}

void SetRandSeed(){

    time_t secs;
    time(&secs);
    srand( (unsigned int)secs);

}

void updateLives(){

    char tempStr[2];
    itoa(myShip.GetLives(), tempStr, 10);
    textout_ex( buffer, font, tempStr, 70, 460, makecol( 255, 0, 0), makecol( 0, 0, 0));

}

void SetupGame(){
     
    SetRandSeed();
     
    myLaser.SetDead( true, buffer);

    for( int i= 0; i <=9; i++){
        if( i < 5)
            myEnemies[i].SetY( 20, buffer, enemy);
        else
            myEnemies[i].SetY( 40, buffer, enemy);
        if( i < 5)
            myEnemies[i].SetX( ((20* i) + 240), buffer, enemy);
        else
            myEnemies[i].SetX( ((20* (i - 5)) + 240), buffer, enemy);
    }
    
    for( int i= 0; i <=9; i++){
    
        enemyLasers[i].SetDead(true, buffer);
    
    }
    
    myShip.SetX( ((640/2) - 40), buffer, ship);
    myShip.SetY( (480 - 40), buffer, ship);
    myShip.SetLives( 4);
    textout_ex( buffer, font, "Lives: ", 10, 460, makecol( 255, 0, 0), makecol( 0, 0, 0));
    updateLives();

    
    Draw();
       

}

void movePlayer(){

    if( key[KEY_RIGHT]){
    
        myShip.SetX( (myShip.GetX() + 20), buffer, ship);
        update = true;
        
    } else if( key[KEY_LEFT]){
    
        myShip.SetX( (myShip.GetX() - 20), buffer, ship);
        update = true;
        
    } else if( key[KEY_SPACE]){
    
        if( myLaser.IsDead()){
        
            myLaser.SetDead(false, buffer);
            myLaser.SetX( myShip.GetX(), buffer, laser);
            myLaser.SetY( (myShip.GetY() - 20), buffer, laser);
            update = true;
        
        }
    
    }

}

void moveLaser(){

    if( !(myLaser.IsDead()) ){
    
        myLaser.SetY( (myLaser.GetY() - 20), buffer, laser);
        
        myLaser.CheckCollision(myEnemies, buffer);
        
        update = true;
        
    }

}

void moveEnemy(){

    int shoot;

    for( int i = 0; i <= 9; i++){
    
        if(!myEnemies[i].IsDead()){
        
            shoot = rand() % 1000 + 1;
            
            if( shoot == 25){
            
                enemyLasers[i].SetDead( false, buffer);
                enemyLasers[i].SetX( myEnemies[i].GetX(),buffer, enemyLaser);
                enemyLasers[i].SetY( 60, buffer, enemyLaser);
                update = true;
            
            }
        
        }
    
    }

}

void moveEnemyLaser(){

    for( int i = 0; i <= 9; i++){
    
        if( !enemyLasers[i].IsDead()){
        
            enemyLasers[i].SetY( (enemyLasers[i].GetY() + 20), buffer, enemyLaser);
            if( (enemyLasers[i].CheckCollision(myShip, buffer))){
            
                myShip.SetLives(myShip.GetLives() - 1);
                updateLives();
                
                
            }
            update = true;
        
        }
    
    }

}

bool checkPlayerLives(){

    if ( myShip.GetLives() <= 0){
         
         textout_ex( screen, font, "GAME OVER", 320, 240, makecol( 255, 0, 0), makecol( 0, 0, 0));
         
         while(!key[KEY_ESC]){}
         
         return true;
    }
    
    return false;

}

bool checkEnemyLives(){

    int enemiesDead = 0;

    for( int i = 0; i <= 9; i++){
    
        if(myEnemies[i].IsDead())
            ++enemiesDead;
    
    }
    
    if(enemiesDead >= 10){
    
        textout_ex( screen, font, "YOU WIN!!!!", 320, 240, makecol( 255, 0, 0), makecol( 0, 0, 0));
        
         while(!key[KEY_ESC]){}
        
         return true;
    }
    
    return false;

}

int main(){

    allegro_init();
    install_keyboard();
    set_color_depth(16);
    set_gfx_mode( GFX_AUTODETECT, 640, 480, 0, 0);
    
    buffer = create_bitmap( 640, 480);
    ship = load_bitmap( "ship.bmp", NULL);
    enemy = load_bitmap( "enemy.bmp", NULL);
    laser = load_bitmap( "laser.bmp", NULL);
    enemyLaser = load_bitmap( "enemyLaser.bmp", NULL);
    
    SetupGame();

    while(!key[KEY_ESC]){
    
        movePlayer();
        moveLaser();
        moveEnemy();
        moveEnemyLaser();
        if(checkPlayerLives())
            break;
        if(checkEnemyLives())
            break;
        
        if(update)
            Draw();
                         
    }

    delete [] myEnemies;
    
    destroy_bitmap( buffer);
    destroy_bitmap( ship);
    destroy_bitmap( enemy);

    return 0;

}
END_OF_MAIN();
