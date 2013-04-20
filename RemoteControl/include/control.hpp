#pragma once

#include "../../Protocol/protocol.h"

int initNet(char *av);
bool load_files();

//Screen attributes
extern const int SCREEN_WIDTH;
extern const int SCREEN_HEIGHT;
extern const int SCREEN_BPP;
 
//The frame rate
extern const int FRAMES_PER_SECOND;

//The dot dimensions
extern const int DOT_WIDTH;
extern const int DOT_HEIGHT;

//The surfaces
extern SDL_Surface *dot;
extern SDL_Surface *screen;

//The event structure
extern SDL_Event event;

//The joystick that will be used
extern SDL_Joystick *stick;


//The dot
class Dot
{
  private:
    //The offsets of the dot
    int x, y;

    //The velocity of the dot
    int speed;
    int xVel, yVel;

    int xOri, yOri;
    int xAxe, yAxe;

    int turn;

  public:
    packet last;

  public:
    //Initializes
    Dot();

    void SetBuf(packet *buf) {
       buf->speed = speed;
       buf->x = xAxe;
       buf->y = yAxe;
       buf->turn = turn;
    };

    //Handles joystick
    void handle_input();

    //Moves the dot
    void move();

    //Shows the dot
    void show();
};

//The timer
class Timer
{
  private:
    //The clock time when the timer started
    int startTicks;

    //The ticks stored when the timer was paused
    int pausedTicks;

    //The timer status
    bool paused;
    bool started;

  public:
    //Initializes variables
    Timer();

    //The various clock actions
    void start();
    void stop();
    void pause();
    void unpause();

    //Gets the timer's time
    int get_ticks();

    //Checks the status of the timer
    bool is_started();
    bool is_paused();
};

