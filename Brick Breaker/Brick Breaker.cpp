//Anmol Raina, 2011

#include <fstream>
#include <allegro.h>

using namespace std;

#define BRICK_WIDTH 92
#define BRICK_HEIGHT 30
#define BRICK_GAP 5

#define BRICKS_HORIZONTALLY 11
#define BRICKS_VERTICALLY 6

#define PADDLE_WIDTH 100
#define PADDLE_HEIGHT 20
#define PADDLE_POSITION_Y (SCREEN_H-50)

BITMAP *screenBuffer;

void updateScreen()
 {blit(screenBuffer, screen, 0, 0, 0, 0, SCREEN_W, SCREEN_H);
 }

class ball
 {public:
  int xv, yv;
  int x, y;
 }ball;

class score
{int tries;
 public:
        void init()
             {tries=0;
             }
        void chances()
             {tries++;
             }
        int output()
             {return tries;
             }
}P;
            
int playerPaddleEdgeX;
 
int bricks[BRICKS_VERTICALLY][BRICKS_HORIZONTALLY];

void makeScreenBlack()
 {clear_to_color(screenBuffer, makecol(0,0,0));
 }

void resetBricks()
 {for(int i=0;i<BRICKS_VERTICALLY;i++)
   {  for(int j=0;j<BRICKS_HORIZONTALLY;j++)
       {bricks[i][j] = 1;
       }
   }
}

int numberOfBricksRemaining() 
 {int numberOfBricksRemaining;
  numberOfBricksRemaining = 0;
  for(int i=0;i<BRICKS_VERTICALLY;i++)
   {for(int j=0;j<BRICKS_HORIZONTALLY;j++) 
     {if(bricks[i][j] != 0) 
       {numberOfBricksRemaining++;
       }
    }
  }
  return numberOfBricksRemaining;
}

void drawingThings()
 { makeScreenBlack();
   for(int i=0;i<BRICKS_VERTICALLY;i++)
     {for(int ii=0;ii<BRICKS_HORIZONTALLY;ii++) 
     {if(bricks[i][ii] != 0)
       {    rectfill(screenBuffer, 
            ii*BRICK_WIDTH, i*BRICK_HEIGHT, 
            (ii+1)*BRICK_WIDTH-BRICK_GAP, (i+1)*BRICK_HEIGHT-BRICK_GAP, 
            makecol(200, 120, 0));
        }
      }
    }
    // draw the player
    circlefill(screenBuffer,ball.x, ball.y,8, makecol(255, 255, 0));

    rectfill(screenBuffer,playerPaddleEdgeX, PADDLE_POSITION_Y,
             playerPaddleEdgeX+PADDLE_WIDTH,PADDLE_POSITION_Y+PADDLE_HEIGHT,
             makecol(255,180, 255));
}

void resetBall()
 {
  ball.x = SCREEN_W;
  ball.y = SCREEN_H/2;
  ball.xv = 4;
  ball.yv = 2;
 }

int doesOverlap(double objectX,double objectY,
                double areaLeft,double areaTop,
                double areaRight,double areaBottom)
 {
  if(ball.x > areaLeft && ball.x < areaRight && ball.y > areaTop && ball.y < areaBottom)
   {return 1; // 1 here means yes
   }
  else return 0; // 0 here means no
 }

void updatePaddlePosition()
 {playerPaddleEdgeX = mouse_x;
 }

void moveBall()
 {// update the ball's position for the next frame
  ball.x += ball.xv;
  ball.y += ball.yv;
  
  // if the ball is overlapping the rectangle
  if(ball.yv > 0)
   { if(doesOverlap(ball.x,ball.y,
                 playerPaddleEdgeX + (PADDLE_WIDTH*0.0),
                 PADDLE_POSITION_Y,
                 playerPaddleEdgeX + (PADDLE_WIDTH*0.25),
                 PADDLE_POSITION_Y + PADDLE_HEIGHT)==1)
                  {
      ball.xv = -5;
      ball.yv = -3;
    }
    if(doesOverlap(ball.x,ball.y,
                 playerPaddleEdgeX + (PADDLE_WIDTH*0.25),
                 PADDLE_POSITION_Y,
                 playerPaddleEdgeX + (PADDLE_WIDTH*0.5),
                 PADDLE_POSITION_Y + PADDLE_HEIGHT)==1) {
      ball.xv = -3;
      ball.yv = -5;
    }
    if(doesOverlap(ball.x,ball.y,
                 playerPaddleEdgeX + (PADDLE_WIDTH*0.5),
                 PADDLE_POSITION_Y,
                 playerPaddleEdgeX + (PADDLE_WIDTH*0.75),
                 PADDLE_POSITION_Y + PADDLE_HEIGHT)==1) {
      ball.xv = 3;
      ball.yv = -5;
    }
    if(doesOverlap(ball.x,ball.y,
                 playerPaddleEdgeX + (PADDLE_WIDTH*0.75),
                 PADDLE_POSITION_Y,
                 playerPaddleEdgeX + (PADDLE_WIDTH*1.0),
                 PADDLE_POSITION_Y + PADDLE_HEIGHT)==1) {
      ball.xv = 5;
      ball.yv = -3;
    }
  }
 // draw the bricks
  for(int i=0;i<BRICKS_VERTICALLY;i++)
   {for(int ii=0;ii<BRICKS_HORIZONTALLY;ii++)
    {if(bricks[i][ii] != 0)
     {  if(doesOverlap(ball.x,ball.y,            // is the brick still here?
             ii*BRICK_WIDTH, i*BRICK_HEIGHT, 
             (ii+1)*BRICK_WIDTH-BRICK_GAP, 
             (i+1)*BRICK_HEIGHT-BRICK_GAP)==1)
              {ball.yv = -ball.yv;               // reverse ball's vertical direction
               bricks[i][ii] = 0;                // erase the brick
              }              
      }
    }
  }
 // bounce off edges of screen
  if(ball.x > SCREEN_W)
   {ball.xv = -ball.xv;
   }
  if(ball.x < 0)
   {ball.xv = -ball.xv;
   }

  if(ball.y < 0)
   {ball.yv = -ball.yv;
   }
  
  //reset ball if it goes off bottom of screen
  if(ball.y > SCREEN_H)
   {  if(mouse_b)
    { resetBall();     // makes the ball reappear
      P.chances(); 
    }
  }
}

void gameSetup()
 {resetBricks();
  resetBall();
  ball.y = SCREEN_H + 50;  // start with the ball off the bottom of the screen
}
// program code aways begins at the top of main()
int main()
{ 
  allegro_init(); 
  set_color_depth(32);
  set_gfx_mode(GFX_AUTODETECT, 1024, 768, 0, 0);
  acquire_screen();
  screenBuffer = create_bitmap(SCREEN_W, SCREEN_H);
  install_keyboard();
  install_mouse(); 
  gameSetup(); 
    
  while(!key[KEY_ESC])
  {  fstream file("Brick Breaker Scores.DAT", ios::out|ios::binary|ios::app);
     updatePaddlePosition();
     moveBall();
     if(numberOfBricksRemaining() == 0)
     {file.write((char*)&P,sizeof(P));
      textout_centre_ex(screen, font, "GAME OVER",
                        SCREEN_W / 2, (SCREEN_H / 2)-40,
                        makecol(0, 0, 255), makecol(255, 0, 0));                
       
      textprintf_ex(screen, font, (SCREEN_W / 2)-60, SCREEN_H / 2, 
                    makecol(0, 0, 255),makecol(255, 0, 0), "Chances Taken: %d",  P.output());    
      
      textout_centre_ex(screen, font, "Press 'R' to start again or 'Esc' to exit.",
                        SCREEN_W / 2, (SCREEN_H / 2)+40,
                        makecol(0, 0, 255), makecol(255, 0, 0));
      
      file.close();                  
                 
      if (key[KEY_R])
          { resetBricks();
            P.init();
          }      
     }      
     drawingThings();
     updateScreen();
     rest(0);
  }
   release_screen();
   return 0;
} 
END_OF_MAIN()
