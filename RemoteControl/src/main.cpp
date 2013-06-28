#include <string>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>

#include "SDL/SDL_image.h"
#include "SDL.h"

#include "control.hpp"
#include "protocol.h"

int s;
packet buf;

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

    if (ac != 2) {
      fprintf(stderr, "Usage: ./control IP \n");
      return 1;
    }
    if (init() == false)
      return 1;
    s = initNet(av[1]);

    long int i = 0;
    if (!s)
      return -1;
    if (send(s, "CLIENT", 6, 0) == -1)
      fprintf(stderr, "Error: packet %ld\n", i);

    packet last;
    bzero(&buf, sizeof(buf));
    while (quit == false) {

        memcpy(&last, &buf, sizeof(buf));

        //While there's events to handle
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
              quit = true;
              break;
            }
            myDot.handle_input();
        }

        buf.id = i;
        last.flags = last.flags & buf.flags ;
        last.id = buf.id;
        if (memcmp(&last, &buf, sizeof(buf))) {
          printf("Sending packet %ld:\tFlags=0x%8.8x\tY=%d\tX=%d\tTurn=%d\tHeight=%d\n", i, buf.flags, buf.y, buf.x, buf.turn, buf.height);
          if (send(s, &buf, sizeof(buf), 0) == -1)
            fprintf(stderr, "Error: packet %ld\n", i);
          i++;
        }
    }

    clean_up();
    return 0;
}
