#ifndef __MIDI_FILEIO
#define __MIDI_FILEIO

#include <iostream>
#include <list>
#include <vector>
#include <stdint.h>
#include <unistd.h>
#include "midistruct.hpp"

typedef unsigned char byte;

using namespace std;

namespace MidiUtils {

	static uint16_t byte2_to_uint16(byte* buffer) {
	    return ((uint16_t)(buffer[0]) << 8) | (uint16_t)(buffer[1]);
	}

	static uint32_t byte4_to_uint32(byte* buffer) {
	    return ((uint32_t)(buffer[0]) << 24) | ((uint32_t)(buffer[1]) << 16) |
	        ((uint32_t)(buffer[2]) << 8) | (uint32_t)(buffer[3]);
	}

	static uint32_t low_half_byte_to_uint32(byte buffer) {
        uint8_t ret = buffer & 0x0f;
        return ret;
	}

	static uint32_t byte1_to_uint32(byte buffer) {
        return 0 | buffer;
	}

    static uint32_t readVariableLengthQuantity(int fd) {
        byte bt;
        uint32_t ret = 0;

        // `bt` less than 128 then read new one
        do {
            read(fd, &bt, sizeof(bt));
            ret = (ret << 7) | (bt & 0x7F);
        } while ((bt & 0x80) == 0x80);

        return ret;
    }

	class MidiObject {
    private:
        uint32_t format, trackCnt;
        int32_t headerSize;
        int32_t currectPos;
        uint32_t noteCnt;
        HeaderChunk headerChunk;
        vector<TrackTrunk> trackList;
        int trackReader(int fd, int trackid);
    public:
    	~MidiObject();
    	uint32_t getNoteCnt();
        bool loadFromMidiFile(const char* filename);
        bool addEventToTrack(int trackid, MidiEvent event);
    };
}

#endif
