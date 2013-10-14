#include <AL/al.h>
#include <AL/alc.h>


int main(){
  ALboolean enumeration;

  enumeration = alcIsExtensionPresent(NULL, "ALC_ENUMERATION_EXT");
  if (enumeration == AL_FALSE){
        printf("Error");
 }
  
  ALCdevice *device;

  device = alcOpenDevice(NULL);
  if (!device){
  
  }
}
