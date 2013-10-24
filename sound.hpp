#ifndef SOUND_HPP_INCLUDED
#define SOUND_HPP_INCLUDED

#include <audio/wave.h>
#include <iostream>
#include <cstdlib>

#include <AL/al.h>
#include <AL/alc.h>

static void check_errors(const char* a=0);
static inline ALenum to_al_format(short channels, short samples);


static ALCdevice *device;
static ALfloat listenerOri[] = { 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f };
static ALuint source[5];
static ALuint buffer[16];
static WaveInfo *wave;
static void *bufferData,*bufferData1,*bufferData2,*bufferData3,*bufferData4,*bufferData5,*bufferData6,*bufferData7,*bufferData8,*bufferData9;
static unsigned int ret;
static ALint source_state;
static ALCcontext *context;
void init();
void play(int=0);
void clean();


using namespace std;


////Sprawdzenie błędów
static void check_errors(const char* a)
{
    ALCenum error=alGetError();
    if (error != AL_NO_ERROR)
    {
        cout<<a<<endl;
    }

}

////Ustalanie ilosci kanałów i próbkowania?????
/// TO mozna na sztywno przypisac ale na podstawie posiadanych plikow .wav
static inline ALenum to_al_format(short channels, short samples)
{

    bool stereo = (channels > 1);

    switch (samples)
    {
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

void init()
{

    device = alcOpenDevice(alcGetString(NULL, ALC_DEFAULT_DEVICE_SPECIFIER));
    if (!device)
    {
        check_errors("Wybranie urządzenia");
    }
    cout<<"Urządzenie: "<< alcGetString(device, ALC_DEVICE_SPECIFIER)<<endl;


    context = alcCreateContext(device, NULL);
    if (!alcMakeContextCurrent(context))
    {
        check_errors("Tworzenie Contex'u");
    }
    alListener3f(AL_POSITION, 0, 0, 1.0f);
    alListener3f(AL_VELOCITY, 0, 0, 0);
    alListenerfv(AL_ORIENTATION, listenerOri);

    check_errors("Dodanie Słuchacza");

    alGenSources((ALuint)5, source);
    check_errors("Wygenerowanie źródła dzwięku");
    alSourcef(source[0], AL_PITCH, 1);
    alSourcef(source[0], AL_GAIN, 1);
    alSource3f(source[0], AL_POSITION, 0, 0, 0);
    alSource3f(source[0], AL_VELOCITY, 0, 0, 0);
    alSourcei(source[0], AL_LOOPING, AL_FALSE);

        for(int i=1;i<5;i++){
            alSourcef(source[i], AL_PITCH, 1);
            alSourcef(source[i], AL_GAIN, 1);
            alSource3f(source[i], AL_POSITION, 0, 0, 0);
            alSource3f(source[i], AL_VELOCITY, 0, 0, 0);
            alSourcei(source[i], AL_LOOPING, AL_FALSE);
        }


    check_errors("Dodanie źródeł");


    alGenBuffers((ALuint)16, buffer);
    check_errors("Generowanie buforów");


    wave = WaveOpenFileForReading("/home/marek/Gra_SDL/sounds/buja.wav");
    if (!wave)
    {
        cout<< "Nie znaleziono pliku dzwiękowego"<<endl;
        return;
    }

    ret = WaveSeekFile(0, wave);
    if (ret)
    {
        cout<<"Nie moze przeszukac pliku"<<endl;
        return;
    }

    bufferData = malloc(wave->dataSize);
    if (!bufferData)
    {
        cout<<"Problem z alokacją pamięci"<<endl;
        return;
    }

    ret = WaveReadFile((char*)bufferData, wave->dataSize, wave);
    if (ret != wave->dataSize)
    {
        cout<<"Bufor za krótki chyba"<<endl;
        return;
    }
    alBufferData(buffer[0], to_al_format(wave->channels, wave->bitsPerSample),bufferData, wave->dataSize, wave->sampleRate);
    check_errors("Kopiowanie do bufora");


    delete wave;
    wave = WaveOpenFileForReading("/home/marek/Gra_SDL/sounds/nalewanie_piwa.wav");
    if (!wave)
    {
        cout<< "Nie znaleziono pliku dzwiękowego"<<endl;
        return;
    }

    ret = WaveSeekFile(0, wave);
    if (ret)
    {
        cout<<"Nie moze przeszukac pliku"<<endl;
        return;
    }

    bufferData1 = malloc(wave->dataSize);
    if (!bufferData1)
    {
        cout<<"Problem z alokacją pamięci"<<endl;
        return;
    }

    ret = WaveReadFile((char*)bufferData1, wave->dataSize, wave);
    if (ret != wave->dataSize)
    {
        cout<<"Bufor za krótki chyba"<<endl;
        return;
    }
    alBufferData(buffer[1], to_al_format(wave->channels, wave->bitsPerSample),bufferData1, wave->dataSize, wave->sampleRate);
    check_errors("Kopiowanie do bufora");






}

void play(int a)
{


    bool search=true;
    int nr=0;
    while(search){
        alGetSourcei(source[nr], AL_SOURCE_STATE, &source_state);
        if(source_state==AL_PLAYING){
                nr++;
        }else{
            search=false;
            alSourcei(source[nr], AL_BUFFER, buffer[a]);
            alSourcePlay(source[nr]);

        }
    }

    //alSourcei(source[0], AL_BUFFER, buffer[a]);
    //alSourcePlay(source[0]);
}

void clean()
{
   /* while (source_state == AL_PLAYING){

        alGetSourcei(source, AL_SOURCE_STATE, &source_state);
        check_errors("Pętla czekajaca na koniec odtworzenia");

    }*/
    alDeleteSources(5, source);
    alDeleteBuffers(16, buffer);
    device = alcGetContextsDevice(context);
    alcMakeContextCurrent(NULL);
    alcDestroyContext(context);
    alcCloseDevice(device);

}



#endif // SOUND_HPP_INCLUDED
