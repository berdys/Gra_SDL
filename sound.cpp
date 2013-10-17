#include <iostream>
#include <cstdlib>

#include "sound.h"


using namespace std;


////Sprawdzenie błędów
static void check_errors(const char* a){
	ALCenum error=alGetError();
        if (error != AL_NO_ERROR){
  		cout<<a<<endl;
        }
        
}

////Ustalanie ilosci kanałów i próbkowania?????
/// TO mozna na sztywno przypisac ale na podstawie posiadanych plikow .wav
static inline ALenum to_al_format(short channels, short samples){
	
        bool stereo = (channels > 1);

        switch (samples) {
        case 16:
                if (stereo)
                        return AL_FORMAT_STEREO16;
                else
                        return AL_FORMAT_MONO16;
        case 8:
                if (stereo)
                        return AL_FORMAT_STEREO8;
                else
                        return AL_FORMAT_MONO8;
        default:
                return -1;
        }
        
}

void init(){
	
  device = alcOpenDevice(alcGetString(NULL, ALC_DEFAULT_DEVICE_SPECIFIER));
  if (!device){
	check_errors("Wybranie urządzenia");
  }
  cout<<"Urządzenie: "<< alcGetString(device, ALC_DEVICE_SPECIFIER)<<endl;
  
  
  context = alcCreateContext(device, NULL);
  if (!alcMakeContextCurrent(context)){
        check_errors("Tworzenie Contex'u");
  }
   alListener3f(AL_POSITION, 0, 0, 1.0f);
   alListener3f(AL_VELOCITY, 0, 0, 0);
   alListenerfv(AL_ORIENTATION, listenerOri);
   
   check_errors("Dodanie Słuchacza");
   alGenSources((ALuint)1, &source);
   check_errors("Wygenerowanie źródła dzwięku");	
   alSourcef(source, AL_PITCH, 1);
   alSourcef(source, AL_GAIN, 1);
   alSource3f(source, AL_POSITION, 0, 0, 0);
   alSource3f(source, AL_VELOCITY, 0, 0, 0);
   alSourcei(source, AL_LOOPING, AL_FALSE);
   check_errors("Dodanie źródeł");
   
   
   alGenBuffers((ALuint)1, &buffer);
   check_errors("Generowanie bufora");
   
   wave = WaveOpenFileForReading("test.wav");
   if (!wave) {
        cout<< "Nie znaleziono pliku dzwiękowego"<<endl;
        return;
   }

   ret = WaveSeekFile(0, wave);
   if (ret) {
       cout<<"Nie moze przeszukac pliku"<<endl;
       return;
   }

   bufferData = malloc(wave->dataSize);
   if (!bufferData) {
       cout<<"Problem z alokacją pamięci"<<endl;
       return;
   }

   ret = WaveReadFile((char*)bufferData, wave->dataSize, wave);
   if (ret != wave->dataSize) {
       cout<<"Bufor za krótki chyba"<<endl;
       return;
   }
   alBufferData(buffer, to_al_format(wave->channels, wave->bitsPerSample),bufferData, wave->dataSize, wave->sampleRate);
   check_errors("Kopiowanie do bufora");
   
   alSourcei(source, AL_BUFFER, buffer);
   check_errors("???");
}

void play(int a){
   switch(a){

	case 0:

   		alSourcePlay(source);
   		check_errors("Odtworzenie");

   		alGetSourcei(source, AL_SOURCE_STATE, &source_state);
   		check_errors("Pobranie stanu");
   		while (source_state == AL_PLAYING) {
        		alGetSourcei(source, AL_SOURCE_STATE, &source_state);
        		check_errors("Pętla czekajaca na koniec odtworzenia");
   		}
   	break;
   	case 1:
   	
   	break;
   	
   	case 2:
   	
   	break;
   	
   	default:
   		cout<<"Błędne wywołanie"<<endl;
   	;
   }
}

void clean(){
   
   alDeleteSources(1, &source);
   alDeleteBuffers(1, &buffer);
   device = alcGetContextsDevice(context);
   alcMakeContextCurrent(NULL);
   alcDestroyContext(context);
   alcCloseDevice(device);

}
