#include <string>

#include "SDL/SDL_image.h"
#include "SDL.h"

#include "control.hpp"
#include "protocol.h"

//Screen attributes
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const int SCREEN_BPP = 32;

//The frame rate
const int FRAMES_PER_SECOND = 20;

//The dot dimensions
const int DOT_WIDTH = 20;
const int DOT_HEIGHT = 20;

//The surfaces
SDL_Surface *dot = NULL;
SDL_Surface *screen = NULL;

//The event structure
SDL_Event event;

//The joystick that will be used
SDL_Joystick *stick = NULL;

SDL_Surface *load_image( std::string filename )
{
  //The image that's loaded
  SDL_Surface* loadedImage = NULL;

  //The optimized surface that will be used
  SDL_Surface* optimizedImage = NULL;

  //Load the image
  loadedImage = IMG_Load( filename.c_str() );

  //If the image loaded
  if( loadedImage != NULL )
  {
    //Create an optimized surface
    optimizedImage = SDL_DisplayFormat( loadedImage );

    //Free the old surface
    SDL_FreeSurface( loadedImage );

    //If the surface was optimized
    if( optimizedImage != NULL )
    {
      //Color key surface
      SDL_SetColorKey( optimizedImage, SDL_SRCCOLORKEY, SDL_MapRGB( optimizedImage->format, 0, 0xFF, 0xFF ) );
    }
  }

  //Return the optimized surface
  return optimizedImage;
}

void apply_surface( int x, int y, SDL_Surface* source, SDL_Surface* destination, SDL_Rect* clip = NULL )
{
  //Holds offsets
  SDL_Rect offset;

  //Get offsets
  offset.x = x;
  offset.y = y;

  //Blit
  SDL_BlitSurface( source, clip, destination, &offset );
}

bool load_files()
{
    //Load the dot image
    dot = load_image( "asset/dot.bmp" );

    //If there was a problem in loading the dot
    if( dot == NULL )
      return false;

    //If everything loaded fine
    return true;
}

Dot::Dot()
{
  x = 0;
  y = 0;

  xVel = 0;
  yVel = 0;

  speed = 0;
  xOri = 0;
  yOri = 0;
  xAxe = 0;
  yAxe = 0;
}

void Dot::handle_input()
{
  if (event.type == SDL_JOYAXISMOTION) {
    if (event.jaxis.which == 0) { // joystick 0
      if (event.jaxis.axis == 3) {
        speed = ((event.jaxis.value * -1 + 32767)) / 1000;
      }
      else if (event.jaxis.axis == 0) { // X
        xAxe = event.jaxis.value / 1000;
      }
      else if (event.jaxis.axis == 1) { // Y
        yAxe = event.jaxis.value / 1000;
      }
      else if (event.jaxis.axis == 2) {  // pivot
        turn = event.jaxis.value;
        if (event.jaxis.value < 0)
          xOri += event.jaxis.value / 1000;
        else if (event.jaxis.value > 0)
          yOri += event.jaxis.value / 1000;
      }
    }
  }
}

void Dot::move()
{
  xVel = xOri * speed - yOri * speed;
  yVel = yOri * speed - xOri * speed;

  x += xAxe * speed / 10;
  y += yAxe * speed / 10;

  if (x + DOT_WIDTH > SCREEN_WIDTH)
    x = SCREEN_WIDTH - DOT_WIDTH;
  else if (x < 0)
    x = 0;
  if (y + DOT_HEIGHT > SCREEN_HEIGHT)
    y -= SCREEN_HEIGHT - DOT_HEIGHT;
  else if (y < 0)
    y = 0;
}

void Dot::show()
{
    //Show the dot
    apply_surface( x, y, dot, screen );
}

Timer::Timer()
{
    //Initialize the variables
    startTicks = 0;
    pausedTicks = 0;
    paused = false;
    started = false;
}

void Timer::start()
{
    //Start the timer
    started = true;

    //Unpause the timer
    paused = false;

    //Get the current clock time
    startTicks = SDL_GetTicks();
}

void Timer::stop()
{
    //Stop the timer
    started = false;

    //Unpause the timer
    paused = false;
}

void Timer::pause()
{
    //If the timer is running and isn't already paused
    if( ( started == true ) && ( paused == false ) )
    {
        //Pause the timer
        paused = true;

        //Calculate the paused ticks
        pausedTicks = SDL_GetTicks() - startTicks;
    }
}

void Timer::unpause()
{
    //If the timer is paused
    if( paused == true )
    {
        //Unpause the timer
        paused = false;

        //Reset the starting ticks
        startTicks = SDL_GetTicks() - pausedTicks;

        //Reset the paused ticks
        pausedTicks = 0;
    }
}

int Timer::get_ticks()
{
    //If the timer is running
    if( started == true )
    {
        //If the timer is paused
        if( paused == true )
        {
            //Return the number of ticks when the timer was paused
            return pausedTicks;
        }
        else
        {
            //Return the current time minus the start time
            return SDL_GetTicks() - startTicks;
        }
    }

    //If the timer isn't running
    return 0;
}

bool Timer::is_started()
{
    return started;
}

bool Timer::is_paused()
{
    return paused;
}

