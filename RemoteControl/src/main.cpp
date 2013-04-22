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
  if( SDL_NumJoysticks() < 1 )
    return false;
  stick = SDL_JoystickOpen( 0 );
  if( stick == NULL )
    return false;
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

    long int i = 0;
    packet buf;
    bzero(&buf, sizeof(buf));
    while (quit == false) {
        //Start the frame timer
        fps.start();

        memcpy(&myDot.last, &buf, sizeof(buf));
        bzero(&buf, sizeof(buf));

        //While there's events to handle
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
              quit = true;
              break;
            }
            if (!myDot.handle_input())
              continue;
            bzero(&buf, sizeof(buf));
            myDot.SetBuf(&buf);
        }

        buf.id = i;
        myDot.last.flags = myDot.last.flags & buf.flags ;
        myDot.last.id = buf.id;
        if (bcmp(&myDot.last, &buf, sizeof(buf))) {
          printf("Sending packet %ld\n", i);
          if (send(s, &buf, sizeof(buf), 0) == -1)
            fprintf(stderr, "Error: packet %ld\n", i);
          i++;
          //Dot.move();
        }

        //Cap the frame rate
        if (fps.get_ticks() < 1000 / FRAMES_PER_SECOND)
          SDL_Delay((1000 / FRAMES_PER_SECOND) - fps.get_ticks());
    }

    clean_up();
    return 0;
}
