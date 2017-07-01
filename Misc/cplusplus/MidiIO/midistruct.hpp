#ifndef __MIDI_FILESTRUCT
#define __MIDI_FILESTRUCT

#include <iostream>
#include <list>
#include <stdint.h>

typedef unsigned char byte;

using namespace std;

namespace MidiUtils {

    enum EventType {
        E_INVALID = -1,
        NOTE_OFF = 0x80,           //1000xxxx 8x (Key is pressed)
        NOTE_ON = 0x90,            //1001xxxx 9x (Key is released)
        KEY_PRESSURE = 0xA0,       //1010xxxx Ax (aka. Key after-touch)
        CONTROL_CHANGE = 0xB0,     //1011xxxx Bx
        PROGRAM_CHANGE = 0xC0,     //1100xxxx Cx (aka. Patch change)
        CHANNEL_PRESSURE = 0xD0,   //1101xxxx Dx (any midifile use it?)
        PITCH_WHEEL_CHANGE = 0xE0, //1110xxxx Ex
        META,
        SYSEX
    };

    enum MetaType {
        M_INVALID = -1,
        SEQUENCE_NUM = 0x00,
        TEXT_EVENT = 0x01,
        COPYRIGHT = 0x02,
        TRACK_NAME = 0x03,
        INSTRUMENT_NAME = 0x04,
        LYRIC = 0x05,
        MARKER = 0x06,
        CUE_POINT = 0x07,
        CHANNEL_PREFIX = 0x20,
        END_OF_TRACK = 0x2F,
        SET_TEMPO = 0x51,
        SMTPE_OFFSET = 0x54,
        TIME_SIGNATURE = 0x58,
        KEY_SIGNATURE = 0x59,
        Sequencer_Specific_Meta_event = 0x7F
    };

	class MidiEvent {
    private:
        EventType type;
        uint32_t channelid;
        uint32_t time;
        byte para1, para2;
        byte* textLikeData = nullptr;
    public:
        uint32_t getTickTime();
        EventType getEventType();
        MidiEvent(EventType evtType, uint32_t channelid, uint32_t evtTime, byte evtPara1, byte evtPara2, byte* data = nullptr);
        ~MidiEvent();
    };

	//Header Chunk size: 14 byte
    struct HeaderChunk {
        byte chunkType[4];  //MThd
        byte headerLengh[4];//Always be 6
        byte midiFormat[2]; //type will be 0, 1, or 2
        byte midiTrackCnt[2];
        byte midiDeltaTime[2];
    };

    //Track(Chunk) Header size: 8 byte
    struct TrackHeader {
        byte chunkType[4];  //MTrk
        byte trackLengh[4];
    };

    class TrackTrunk {
    public:
		TrackHeader trackHeader;
		list<MidiEvent> eventList;
		//byte*
	};
}

#endif
