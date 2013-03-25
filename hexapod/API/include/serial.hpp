#include <termios.h>
#include <string>

#define microdelay(X) do { struct timeval p__tv = {((X) / 1000000), ((X) % 1000000)}; select (0, NULL, NULL, NULL, &p__tv); } while (0);

class Serial {
  public:
    Serial(const char * fd);
    virtual ~Serial();

    int write(const std::string &);
    //int read(const char *);

  private:
    int		    fd;
    struct termios  saved_state;
};
  

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  

  

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  

  
  
  
