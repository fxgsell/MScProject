#include <string>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>

#include "SDL/SDL_image.h"
#include "SDL.h"

#include "control.hpp"
#include "protocol.h"

int s;

bool init()
{
  if( SDL_Init( SDL_INIT_EVERYTHING ) == -1 )
    return false;
  screen = SDL_SetVideoMode( SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, SDL_SWSURFACE );
  if( screen == NULL )
    return false;
  if( SDL_NumJoysticks() < 1 )
    return false;
  stick = SDL_JoystickOpen( 0 );
  if( stick == NULL )
    return false;
  SDL_WM_SetCaption( "Move the Dot", NULL );
  return true;
}

void clean_up()
{
  close(s);
  SDL_FreeSurface( dot );
  SDL_JoystickClose( stick );
  SDL_Quit();
}


int main(int ac, char* av[])
{
    bool quit = false;
    Dot myDot;
    Timer fps;

    if (ac != 2) {
      fprintf(stderr, "Usage: ./control IP \n");
      return 1;
    }
    if (init() == false)
      return 1;
    s = initNet(av[1]);
    if (!s)
      return -1;

    if (load_files() == false)
      return 1;

    int i = 0;
    char *buf = "TEST\n";
    while (quit == false) {
        //Start the frame timer
        fps.start();

        //While there's events to handle
        while (SDL_PollEvent(&event)) {
            myDot.handle_input();
            if (event.type == SDL_QUIT)
              quit = true;
        }

        printf("Sending packet %d\n", i);
        if (send(s, buf, BUFLEN, 0) == -1)
          fprintf(stderr, "Error: packet %d\n", i);
        i++;
        printf("Hello\n");

        //Move the dot
        myDot.move();

        //Fill the screen white
        SDL_FillRect(screen, &screen->clip_rect, SDL_MapRGB(screen->format, 0xFF, 0xFF, 0xFF));

        //Show the dot on the screen
        myDot.show();

        //Update the screen
        if (SDL_Flip(screen) == -1)
          return 1;

        //Cap the frame rate
        if (fps.get_ticks() < 1000 / FRAMES_PER_SECOND)
          SDL_Delay((1000 / FRAMES_PER_SECOND) - fps.get_ticks());
    }

    clean_up();
    return 0;
}
