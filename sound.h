#ifndef sound_H
#define sound_H

////From libaudio
#include <audio/wave.h>

/// From OpenAL
#include <AL/al.h>
#include <AL/alc.h>

  static void check_errors(const char* a=0);
  static inline ALenum to_al_format(short channels, short samples);

  static ALboolean enumeration;
  static   ALCdevice *device;
  static ALfloat listenerOri[] = { 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f };
  static ALuint source;
  static ALuint buffer;
  static WaveInfo *wave;
  static void *bufferData;
  static int ret;
  static ALint source_state;
  static ALCcontext *context;
  static ALboolean loop = AL_FALSE;
  void init();
  void play(int=0);
  void clean();

#endif
