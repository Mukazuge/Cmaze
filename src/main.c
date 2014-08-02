#define SDL_MAIN_HANDLED
#include <stdbool.h>
#include <time.h>

#ifdef __MINGW32__
  #include "windows.h"
#else
  #include "unix.h"
#endif
#include "enemies.h"
#include "animation.h"
#include "items.h"
#include "maze.h"

Map maze;
int level = 0; 
bool nextlevel = false;
bool alive = true, start = true, finished = false;
int RIGHT = 0, BOT = 0, MAXMOBS = 0;
int MAXHP = 3;

bool cellEmpty(Enemy enemies[], int newX, int  newY){
    bool empty = true;
    int i;
     for(i = 0;i<MAXMOBS;i++){
        if(enemies[i].x == newX && enemies[i].y == newY){
            empty = false;
        }
    }

    return empty;
}

int main( int argc, char* args[] )
{
    init();
    srand(time(NULL));
    SDL_Texture *tileSet = loadBMP( "sprites/tileset.bmp" );
    SDL_Texture *heroSprite = loadBMP( "sprites/hero.bmp" );
    SDL_Texture *healthSprite = loadBMP( "sprites/hp.bmp" );
    SDL_Texture *defeatSprite = loadBMP("sprites/defeat.bmp");

    SDL_Rect tileGrass = {830, 190, 64, 64};
    SDL_Rect tileWall = {830, 32, 64, 128};
    SDL_Rect tileArmor = {960, 1470, 32, 32};
    SDL_Rect tileTeddy = {960, 1630, 32, 32};
    SDL_Rect tileShield = {896, 1600, 32, 32};
    SDL_Rect tileWhip = {704, 1860, 32, 32};
    SDL_Rect tileStart = {160,1952, 32, 32};
    SDL_Rect tileEnd = {10, 1590, 48, 48};
    SDL_Rect defeatArea = {0, 0, 800, 600};
    SDL_Rect heartArea = {0, 4, 20, 20};

    do{
        nextlevel = false;
        level++;
        RIGHT = 11 + (level * 4);
        BOT = 11 + (level * 2);
        MAXMOBS = 1 + (level * 2);  
        resizeWindow(RIGHT*32, BOT*32);
        int direction = 0;
        int i,j;
        int takenCheckpoints = 0;
        start = true;
        alive = true;
        Player player;
        player.hp = MAXHP++;
        player.frame = 0;
        Enemy enemies[MAXMOBS];
        Item items[MAXITEMS];
        Item endGate;
        Item checkpoints[MAXCHECKPOINTS];
        Item hearts[MAXHP];

        createMaze(&maze, RIGHT, BOT);
       
        //player spawn
        do{
            player.x = rand()%(RIGHT);
            player.y = rand()%(BOT);      
            }while(getCell(maze, player.x, player.y) != FLOOR);
        
         //healthpoints spawn
         for(i = 0;i < MAXHP;i++){    
                hearts[i].visible = true;
        }
        //mob spawn
        for(i = 0;i<MAXMOBS;i++){
            do{
                enemies[i].x = rand()%(RIGHT);
                enemies[i].y = rand()%(BOT);
                enemies[i].steps = 0;
                enemies[i].direction = 0;         
            }while(getCell(maze, enemies[i].x, enemies[i].y) != FLOOR);
            
            setCell(&maze, enemies[i].x, enemies[i].y, ITEM); 
        }
        //checkpoints spawn
         for(i = 0;i<MAXCHECKPOINTS;i++){ 
            do{ 
                checkpoints[i].x = rand()%(RIGHT);
                checkpoints[i].y = rand()%(BOT);
                checkpoints[i].visible=true;
            }while(getCell(maze, checkpoints[i].x,checkpoints[i].y) != FLOOR);
            
            switch(i){
                case 0:
                    setCell(&maze, checkpoints[i].x, checkpoints[i].y, SHIELD);           
                    break;
                case 1:
                    setCell(&maze, checkpoints[i].x, checkpoints[i].y,TEDDY);           
                    break;
                case 2:
                    setCell(&maze, checkpoints[i].x, checkpoints[i].y,ARMOR);           
                    break;
                default:
                    setCell(&maze, checkpoints[i].x, checkpoints[i].y,WHIP);           
                    break;     
            }
        }
        //gates spawn
            do{ 
                endGate.x = rand()%(RIGHT);
                endGate.y = rand()%(BOT);
            }while(getCell(maze, endGate.x, endGate.y) != FLOOR);
            setCell(&maze, endGate.x, endGate.y, GOAL);

        do {
            SDL_WaitEvent(&event);
           //enemy movement
            if((event.type == SDL_KEYDOWN || start) && alive) {
                start = false;
                for(i = 0;i<MAXMOBS;i++){
                    if(enemies[i].steps < 1){
                        enemies[i].direction = rand()%4;
                        enemies[i].steps = 2+(rand()%10);
                    }
                    enemies[i].steps--;
                    
                    switch(enemies[i].direction){
                        case 0:
                            if(enemies[i].y < BOT-1 && (cellEmpty(enemies,enemies[i].x,enemies[i].y+1))){
                                if (isWalkable(maze, enemies[i].x, enemies[i].y+1)) enemies[i].y++;    
                            }
                            else enemies[i].steps = 0;
                            break;
                        case 1:
                            if(enemies[i].x < RIGHT-1&& (cellEmpty(enemies,enemies[i].x+1,enemies[i].y))){
                                if (isWalkable(maze, enemies[i].x+1, enemies[i].y)) enemies[i].x++;
                            }
                            else enemies[i].steps = 0;
                            break;
                        case 2:
                            if(enemies[i].y > TOP&& (cellEmpty(enemies,enemies[i].x,enemies[i].y-1))){
                                if (isWalkable(maze, enemies[i].x, enemies[i].y-1)) enemies[i].y--;
                            }
                            else enemies[i].steps = 0;
                            break;
                        case 3:
                            if(enemies[i].x > LEFT&& (cellEmpty(enemies,enemies[i].x-1,enemies[i].y))){
                                if (isWalkable(maze, enemies[i].x-1, enemies[i].y)) enemies[i].x--;
                            }
                            else enemies[i].steps = 0;
                            break;
                    }
                }
                //player movement
                switch( event.key.keysym.sym ) {
                    case SDLK_DOWN:
                        direction = 0;  
                        if (isWalkable(maze, player.x, player.y+1)) animateTo(&player,0,1,0);
                        break;
                    case SDLK_LEFT:
                        direction = 3;
                        if (isWalkable(maze, player.x-1, player.y))animateTo(&player,-1,0,0);
                        break;
                    case SDLK_RIGHT:
                        direction = 1;
                        if (isWalkable(maze, player.x+1, player.y)) animateTo(&player,1,0,0);
                        break;
                    case SDLK_UP:
                        direction = 2;
                        if (isWalkable(maze, player.x, player.y-1)) animateTo(&player,0,-1,0);
                        break;
                        //RESTART
                    case SDLK_SPACE:
                        level = 0; 
                        RIGHT = 0, BOT = 0, MAXMOBS = 0;
                        MAXHP = 3;
                        nextlevel = true;
                        break;
                } 
                //player border validation
                if (player.x > RIGHT-1) player.x = RIGHT-1;
                if (player.x <1) player.x = 0;
                if (player.y > BOT-1) player.y = BOT-1;
                if (player.y <1) player.y = 0;
                //checkpoint colition validation
                for(i = 0;i<MAXCHECKPOINTS;i++){
                    if((checkpoints[i].x == player.x) && (checkpoints[i].y == player.y) && (checkpoints[i].visible)){
                    checkpoints[i].visible = false;
                    takenCheckpoints++;
                    }
                }
                //end gate colition validation
                if((endGate.x == player.x) && (endGate.y == player.y) && takenCheckpoints == 4) {
                    nextlevel = true;
                }
                //item spawn  validation
                for(i = 0;i<MAXITEMS;i++){
                    if((items[i].x == player.x) && (items[i].y == player.y) &&(items[i].visible)) items[i].visible = false;   
                }
                //player and mob colition validation
                for(i = 0;i<MAXMOBS;i++){
                    if((enemies[i].x == player.x ) && (enemies[i].y == player.y)){
                       player.hp--;
                    } 
                }    
                clear();
                //map render
                for(i = 0;i < maze.width; i++){
                    for(j = 0; j < maze.height; j++){
                        SDL_Rect tilePaste = { i*32, j*32, 32, 32 };
                        switch (getCell(maze, i, j)){
                            case FLOOR://floor
                            case ITEM://item or enemy
                            case SHIELD://chechpoint
                            case WHIP://chechpoint
                            case ARMOR://chechpoint
                            case TEDDY://chechpoint
                            case START://Start
                            case GOAL://End
                                SDL_RenderCopy(renderer, tileSet, &tileGrass, &tilePaste);
                                break;
                            case WALL_HORZ:
                            case WALL_VERT:
                            case WALL_CROS:
                                SDL_RenderCopy(renderer, tileSet, &tileWall, &tilePaste);
                                break;    
                        }
                    }
                }  
                //checkpoints render
                for(i = 0;i<MAXCHECKPOINTS;i++){ 
                    if(checkpoints[i].visible){    
                        SDL_Rect checkpointPaste = { (checkpoints[i].x*32), (checkpoints[i].y*32), 32, 32};
                        
                        switch (getCell(maze, checkpoints[i].x, checkpoints[i].y)){
                            case ARMOR:
                                SDL_RenderCopy(renderer, tileSet, &tileArmor, &checkpointPaste);
                                break;
                            case TEDDY:
                                SDL_RenderCopy(renderer, tileSet, &tileTeddy, &checkpointPaste);
                                break;
                            case SHIELD:
                                SDL_RenderCopy(renderer, tileSet, &tileShield, &checkpointPaste);
                                break;
                            case WHIP:
                                SDL_RenderCopy(renderer, tileSet, &tileWhip, &checkpointPaste);
                                break;
                        }
                    }
                }       
                //gates render
                    SDL_Rect endgatePaste = { (endGate.x*32), (endGate.y*32), 32, 32};
                    SDL_RenderCopy(renderer, tileSet, &tileEnd, &endgatePaste);
                            
                //mobs render
                for(i = 0;i<MAXMOBS;i++){
                        enemies[i].frame++;
                        if(enemies[i].frame>2) enemies[i].frame = 0;
                        SDL_Rect areaEnemy = {(96* enemies[i].direction)+ (32*enemies[i].frame), 224, 32, 32}; 
                        SDL_Rect enemyPaste = { (enemies[i].x*32), (enemies[i].y*32), 32, 32};
                        SDL_RenderCopy(renderer, heroSprite, &areaEnemy, &enemyPaste);
                    
                }
                //player render
                player.frame++;
                if (player.frame >2) player.frame = 0;

                SDL_Rect playerCut = { (96* direction)+ (32*player.frame), 32, 32, 32};
                SDL_Rect playerPaste = { player.x*32, player.y*32, 32, 32 };
                SDL_RenderCopy(renderer, heroSprite, &playerCut, &playerPaste);
                //defeat render
                if(player.hp<=0){
                    SDL_Rect defeatPaste = {0, 0, RIGHT*32, BOT*32};
                    SDL_RenderCopy(renderer, defeatSprite, &defeatArea, &defeatPaste);
                    alive = false;
                }
                //heart render
                for(i = 0;i<player.hp;i++){ 
                    if(hearts[i].visible){    
                        SDL_Rect heartPaste = {i*32, 0, 32, 32};
                        SDL_RenderCopy(renderer, healthSprite, &heartArea, &heartPaste);
                    }
                }
                render();
            }
        if( event.type == SDL_QUIT){
            finished = true;
        }
        }while((!finished) && nextlevel == false);
    }while(alive && (!finished));

    SDL_DestroyTexture(heroSprite);
    SDL_DestroyTexture(tileSet);
    SDL_DestroyTexture(healthSprite);

    quit();    
    return 0;
}