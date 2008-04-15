// sound.h
#include <mico/stream.h>

#ifdef _WIN32
#include <fstream>
#include <iostream>
#else
#include <fstream.h>
#include <iostream.h>
#endif

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include  <sys/audioio.h>
 
// simple class for sun audio device


extern int errno;

class SunAudio
{
 public:
  int _fd;

 public:
  SunAudio(){}
  ~SunAudio(){ close (_fd); }

  void open( void ){     
    _fd = ::open("/dev/audio", O_RDWR | O_NONBLOCK);
    static  struct audio_info sound_info;
    ioctl(_fd, AUDIO_GETINFO, &sound_info);

    sound_info.play.gain = 140;
    sound_info.record.gain = 150;
    sound_info.play.sample_rate = 8000;
    sound_info.record.sample_rate = 8000;
    sound_info.play.port = AUDIO_SPEAKER;
    sound_info.record.port = AUDIO_LINE_IN;
    ioctl(_fd, AUDIO_SETINFO, &sound_info);
  }

  void write(void *_buf, int _len){  ::write(_fd, _buf, _len ); }

  int read(void *_buf, int _len){ return (::read(_fd, _buf, _len) ); }

};
