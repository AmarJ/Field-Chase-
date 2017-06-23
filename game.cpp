/*******************************************************************************************
Amar Jasarbasic
Field Chase game
********************************************************************************************/
#include <allegro.h>
#include <time.h>

const int scrx = 512;		//window width-
const int scry = 480;		// window height

//init bitmaps
BITMAP* playerBit;
BITMAP* monsterBit;
BITMAP* background;
BITMAP* message;
BITMAP* enemyBit;
BITMAP *lostMessage;

//functions
void spawnMonster();
void moveEnemy();
void movePlayer();   
int checkCollision(int x1, int y1, int w1, int h1, int x2, int y2, int w2, int h2);  

//global variables
int borderSize = 25;
int RandomX;
int RandomY;
int score = 0;
bool lost = false;

struct character {
  int x;
  int y;
  float speed;
  int size[2];  
};

struct character player;
struct character enemy;
struct character monster;

int main() {
   
   //allegro init stuff
   allegro_init();  

   install_timer();
   install_keyboard();
   install_mouse();

   set_color_depth(32);
   
   set_gfx_mode(GFX_AUTODETECT_WINDOWED, scrx, scry, 0, 0);
   
   BITMAP *playerBit = load_bitmap("images/hero.bmp", NULL);
   BITMAP *monsterBit = load_bitmap("images/monster.bmp", NULL);
   BITMAP *background = load_bitmap("images/background.bmp", NULL);
   BITMAP *message = load_bitmap("images/message.bmp", NULL);
   BITMAP *enemyBit = load_bitmap("images/enemy.bmp", NULL);
   BITMAP *lostMessage = load_bitmap("images/loser.bmp", NULL);
   BITMAP *buffer=create_bitmap(scrx, scry);
    
   //init for character size, speed and position  
   player.size[0]=32;
   player.size[1]=32;
   monster.size[0]=32;
   monster.size[1]=32;
   enemy.size[0]=32;
   enemy.size[1]=32;
   //position     
   player.x = 32;
   player.y = 32;
   //speed
   player.speed = 3;
   enemy.speed = 1;
   
   //spawns monster randomly before game starts
   spawnMonster();
   enemy.x = 300;
   enemy.y = 300;

   //game loop
   while (lost == false && !key[KEY_ESC]) {

       draw_sprite( buffer, background, 0, 0);
       draw_sprite( buffer, playerBit, player.x, player.y);
       draw_sprite( buffer, monsterBit, monster.x, monster.y);
       draw_sprite( buffer, enemyBit, enemy.x, enemy.y);

       movePlayer();  
       moveEnemy();    
       
       //checks for collision between monster(butterfly) and adds to score
       if (checkCollision(player.x, player.y, player.size[0], player.size[1], monster.x, monster.y, monster.size[0], monster.size[1]) == 1){
           //prints message, when user 'catches' monster
           draw_sprite( buffer, message, 0, 0); 
           draw_sprite(screen, buffer, 0,0);
           rest(300);
           spawnMonster();
           score++;
       }
       
       //checks if the user collides with enemy and ends the game
       if (checkCollision(player.x, player.y, player.size[0], player.size[1], enemy.x, enemy.y, enemy.size[0], enemy.size[1]) == 1){
           lost = true;
       }
        
       //prints current score, updates everytime game loop, loops  
       textprintf_ex(buffer, font, 35, 35, makecol(255, 255, 255),  -1, "Score: %d", score);
       
       if (score < 4)
            textprintf_ex(buffer, font, 35, 435, makecol(255, 255, 255),  -1, "[Move: with W-A-S-D] [Auto catch: with space bar]");
       else 
            textprintf_ex(buffer, font, 35, 435, makecol(255, 255, 255),  -1, "Created by: Amar JasarbasicS");
             
       draw_sprite(screen, buffer, 0,0);

   }

//after player lost, checks when ESC is pressed to exit game
while(!key[KEY_ESC]) {
      draw_sprite( buffer, lostMessage, 0, 0); 
      draw_sprite(screen, buffer, 0,0);
}  
    
//clears bitmaps    
clear_bitmap(buffer); 
destroy_bitmap(playerBit);
destroy_bitmap(background);
destroy_bitmap(buffer);
return 0;
}
END_OF_MAIN()

//function that spawn monster randomly on screen within specific borders
void spawnMonster() {
    //will spawn mosnter, 96 pixels inside curret screen boarder
    RandomX = scrx - 96;
    RandomY = scry - 96;
    monster.x = rand() % RandomX + borderSize;
    monster.y = rand() % RandomY + borderSize;
}

void moveEnemy(){
   //makes enemy chase player, by following y,x coordinates of player
   if (player.x > enemy.x)
      enemy.x += enemy.speed;
   else if (player.x < enemy.x)
      enemy.x -= enemy.speed;
   if (player.y > enemy.y)
      enemy.y += enemy.speed;
   else if (player.y < enemy.y)
      enemy.y -= enemy.speed; 
}

void movePlayer() {
    //moves player with keys pressed by user
    if (key[KEY_W])      
        player.y -= player.speed;              
    if (key[KEY_S])                  
        player.y += player.speed;             
    if (key[KEY_D]) 
        player.x += player.speed;                      
    if (key[KEY_A]) 
        player.x -= player.speed;                               
    
    //makes a virtual border, not letting player move beyond specific points
    if (player.x+player.size[0] > scrx - borderSize) 
        player.x -= player.speed;
    if (player.y+player.size[1] > scry - borderSize)
        player.y -= player.speed;
    if (player.x < borderSize)
        player.x += player.speed;
    if (player.y < borderSize) 
        player.y += player.speed;
    
    //auto follow: automatically makes player move to monster(similer 
    //code t 0 moveEnemy() function)
    if (key[KEY_SPACE]) {
        if (player.x < monster.x)
            player.x += player.speed;
        else if (player.x > monster.x)
            player.x -= player.speed;
        if (player.y < monster.y)
            player.y += player.speed;
        else if (player.y > monster.y)
            player.y -= player.speed; 
    }    
}

//function that checks collision returning 1:true 0:false
int checkCollision(int x1, int y1, int w1, int h1, int x2, int y2, int w2, int h2){
    if((x1+w1-2)>=x2+1 && (x2+w2-1)>=x1+2 && (y1+h1-2)>=y2+1 && (y2+h2-3)>=y1+7)
        return 1;
    else
        return 0;
}
