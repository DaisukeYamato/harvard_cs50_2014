//
// breakout.c
//
// Computer Science 50
// Problem Set 4
//

// standard libraries
#define _XOPEN_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Stanford Portable Library
#include "gevents.h"
#include "gobjects.h"
#include "gwindow.h"

// height and width of game's window in pixels
#define HEIGHT 600
#define WIDTH 400

// number of rows of bricks
#define ROWS 5

// number of columns of bricks
#define COLS 10

// radius of ball in pixels
#define RADIUS 10

// lives
#define LIVES 3

// paddle width, height, paddle loc (y)
double PAD_WIDTH = WIDTH / 10;
double PAD_HEIGHT = HEIGHT / 60;
double PAD_LOC_Y = 9.0/10 * HEIGHT;

// prototypes
void initBricks(GWindow window);
GOval initBall(GWindow window);
GRect initPaddle(GWindow window);
GLabel initScoreboard(GWindow window);
void updateScoreboard(GWindow window, GLabel label, int points);
GObject detectCollision(GWindow window, GOval ball);
string getMyColor(int i);

int main(void)
{
    // seed pseudorandom number generator
    srand48(time(NULL));

    // instantiate window
    GWindow window = newGWindow(WIDTH, HEIGHT);

    // instantiate bricks
    initBricks(window);

    // instantiate ball, centered in middle of window
    GOval ball = initBall(window);

    // instantiate paddle, centered at bottom of window
    GRect paddle = initPaddle(window);

    // instantiate scoreboard, centered in middle of window, just above ball
    GLabel label = initScoreboard(window);

    // number of bricks initially
    int bricks = COLS * ROWS;

    // number of lives initially
    int lives = LIVES;

    // number of points initially
    int points = 0;
    
    // circle velocity
    double vel_x = 0.5 + drand48();
    double vel_y = vel_x;

    // spawn ball flag
    bool spawn = false;
    
    // keep playing until game over
    while (lives > 0 && bricks > 0) {
      updateScoreboard(window, label, points);
      if ( !spawn ){
	// check for mouse event
	GEvent event = getNextEvent(MOUSE_EVENT);
	if ( event != NULL ){
	  if (getEventType(event) == MOUSE_CLICKED){
	    spawn = true;
	  }
	}
      }
      else {
	// move circle
	move(ball, vel_x, vel_y);
	// bounce
	if (getX(ball)+getWidth(ball) >= getWidth(window)){
	  vel_x = - vel_x;
	}
	if (getX(ball) <= 0){
	  vel_x = - vel_x;
	}
	if (getY(ball)+getHeight(ball)>=getHeight(window)){
	  vel_y = - vel_y;
	}
	if (getY(ball) <= 0){
	  vel_y = - vel_y;
	}
	
	// check for mouse event
	GEvent event = getNextEvent(MOUSE_EVENT);
	
	// if we heard one
	if (event != NULL){
	  
	  // if the event was movement
	  if (getEventType(event) == MOUSE_MOVED){
	    
	    // set paddle as mouse x-coord correctly
	    double x = getX(event) - getWidth(paddle)/2;
	    if (x <= 0){
	      x = 0;
	    }
	    else if (x >= WIDTH - getWidth(paddle)){
	      x = WIDTH - getWidth(paddle);
	    }
	    setLocation(paddle, x, PAD_LOC_Y);
	  }
	}
	// detect collisions
	GObject object = detectCollision(window, ball);
	if (object != NULL){
	  if (object == paddle){
	    vel_y = - vel_y;
	  }
	  else if (strcmp(getType(object), "GLabel") == 0){
	    // nothing to do
	  }
	  else if (strcmp(getType(object), "GRect") == 0){
	    // brick collision
	    removeGWindow(window, object);
	    vel_y = - vel_y;
	    points += 1;
	  }
	}
	
	// lose ball
	if (getY(ball) > PAD_LOC_Y + PAD_HEIGHT){
	  lives -= 1;
	  setLocation(ball, WIDTH/2 - 10, HEIGHT/2-10);
	  spawn = false;
	}
	// linger before moving again
	pause(5);
      }
    }
    
    // wait for click before exiting
    waitForClick();

    // game over
    closeGWindow(window);
    return 0;
}

/**
 * Initializes window with a grid of bricks.
 */
void initBricks(GWindow window)
{
  GRect bricks[ROWS][COLS];
  double brick_interval = 2;
  double ceil_interval = 10;
  double brick_width = (WIDTH - brick_interval * (COLS+1))/COLS;
  double brick_height = 5;
  string color_list[ROWS];
  
  for (int row=0; row<ROWS; row++){
    for (int col=0; col<COLS; col++){
      bricks[row][col] = newGRect(brick_interval * (col+1) + col*brick_width,
				  ceil_interval + row * (brick_height + brick_interval),
				  brick_width, brick_height);
      setColor(bricks[row][col], getMyColor(row));
      setFilled(bricks[row][col], true);
      add(window, bricks[row][col]);
    }
  }
}

/**
 * Instantiates ball in center of window.  Returns ball.
 */
GOval initBall(GWindow window)
{
  GOval oval = newGOval(WIDTH/2 - 10, HEIGHT/2-10, 20, 20);
  setColor(oval, "BLACK");
  setFilled(oval, true);
  add(window, oval);
  
  return oval;
}

/**
 * Instantiates paddle in bottom-middle of window.
 */
GRect initPaddle(GWindow window)
{
  GRect pad = newGRect(WIDTH/2-PAD_WIDTH/2, PAD_LOC_Y,
		       PAD_WIDTH, PAD_HEIGHT);
  setColor(pad, "BLACK");
  setFilled(pad, true);
  add(window, pad);
  
  return pad;
}

/**
 * Instantiates, configures, and returns label for scoreboard.
 */
GLabel initScoreboard(GWindow window)
{
  GLabel score = newGLabel("0");
  setFont(score, "SansSerif-36");
  setColor(score, "GRAY");
  add(window, score);
  return score;
}

/**
 * Updates scoreboard's label, keeping it centered in window.
 */
void updateScoreboard(GWindow window, GLabel label, int points)
{
    // update label
    char s[12];
    sprintf(s, "%i", points);
    setLabel(label, s);

    // center label in window
    double x = (getWidth(window) - getWidth(label)) / 2;
    double y = (getHeight(window) - getHeight(label)) / 2;
    setLocation(label, x, y);
}

/**
 * Detects whether ball has collided with some object in window
 * by checking the four corners of its bounding box (which are
 * outside the ball's GOval, and so the ball can't collide with
 * itself).  Returns object if so, else NULL.
 */
GObject detectCollision(GWindow window, GOval ball)
{
    // ball's location
    double x = getX(ball);
    double y = getY(ball);

    // for checking for collisions
    GObject object;

    // check for collision at ball's top-left corner
    object = getGObjectAt(window, x, y);
    if (object != NULL)
    {
        return object;
    }

    // check for collision at ball's top-right corner
    object = getGObjectAt(window, x + 2 * RADIUS, y);
    if (object != NULL)
    {
        return object;
    }

    // check for collision at ball's bottom-left corner
    object = getGObjectAt(window, x, y + 2 * RADIUS);
    if (object != NULL)
    {
        return object;
    }

    // check for collision at ball's bottom-right corner
    object = getGObjectAt(window, x + 2 * RADIUS, y + 2 * RADIUS);
    if (object != NULL)
    {
        return object;
    }

    // no collision
    return NULL;
}

// get my color
string getMyColor(int i){
  if (i==0){
    return "RED";
  }
  else if (i==1){
    return "BLUE";
  }
  else if (i==2){
    return "GREEN";
  }
  else if (i==3){
    return "ORANGE";
  }
  else if (i==4){
    return "YELLOW";
  }
  
  return "BLACK";
}
