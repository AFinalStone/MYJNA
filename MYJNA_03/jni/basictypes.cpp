#include "basictypes.h"

int32 GetRandomTime(void){
    int seed;
    static bool dev_urandom_exists =true;
    if(dev_urandom_exists){
        FILE* dev_urandom;
        do{
            errno = 0;
            dev_urandom = fopen("/dev/urandom","rb");
        }while(errno==EINTR);

        if(dev_urandom){
            int r;
            setvbuf(dev_urandom,NULL,_IONBF,0);
            do{
                errno =0;
                r = fread(&seed,sizeof(seed),1,dev_urandom);
            }while(errno==EINTR);
            if(r!=1)
                dev_urandom_exists = false;
        }
    }
 
    return seed;
}

void LogAssert(const char * function, const char * file, int line, const char * expression) {
  // TODO - if we put hooks in here, we can do a lot fancier logging
  fprintf(stderr, "%s(%d): %s @ %s\n", file, line, expression, function);
}