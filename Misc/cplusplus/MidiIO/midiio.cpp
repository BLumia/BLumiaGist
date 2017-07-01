#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <cstdio>
#include <cstdlib>
#include <vector>
#include "midiio.hpp"

using namespace MidiUtils;

MidiObject::~MidiObject() {

}

int MidiObject::trackReader(int fd, int trackid) {
	TrackTrunk aTrack;
	TrackHeader trackHeader;
	uint32_t chunkLength;
	off_t chunkStartPos;

	read(fd, &trackHeader, sizeof(trackHeader)); // TODO: check if failed
	if (strncmp((const char*)trackHeader.chunkType, "MTrk", 4) != 0) {
		cerr << "Midi file header not correct, should be \"MTrk\", not: " << trackHeader.chunkType <<endl;
		return -1;
	}
	chunkLength = byte4_to_uint32(trackHeader.trackLengh);
	chunkStartPos = lseek(fd, 0, SEEK_CUR);
	cout << trackHeader.chunkType << " " << chunkLength <<endl;

	// read midi events
	bool ALREADY_END_OF_TRACK = false;
	uint32_t eventDeltaTime, tmpCurTime = 0, eventStartTime;
	byte evtType, lastType;
	byte para1, para2;

    while ((lseek(fd, 0, SEEK_CUR) < chunkStartPos + chunkLength) && !ALREADY_END_OF_TRACK) {
        eventDeltaTime = readVariableLengthQuantity(fd);
        eventStartTime = tmpCurTime;
        tmpCurTime += eventDeltaTime;
        read(fd, &evtType, sizeof(evtType));

        if ((evtType & 0x80) == 0x00) {
            evtType = lastType;
            lseek(fd, -1, SEEK_CUR);
        } else {
            lastType = evtType;
        }
        //printf("event: %02X\n",evtType);

retry:
        switch(evtType & 0xF0) { //0xF0 = 11110000
        case NOTE_OFF:
        {
            read(fd, &para1, sizeof(para1));
            read(fd, &para2, sizeof(para2));
            MidiEvent midiEvt(NOTE_OFF,
                              low_half_byte_to_uint32(evtType),
                              eventStartTime,
                              para1, para2);
            addEventToTrack(trackid, midiEvt);
            break;
        }

        case NOTE_ON:
            read(fd, &para1, sizeof(para1));
            read(fd, &para2, sizeof(para2));
            if(para2) {
                noteCnt++;
                MidiEvent midiEvt(NOTE_ON,
                                  low_half_byte_to_uint32(evtType),
                                  eventStartTime,
                                  para1, para2);
                addEventToTrack(trackid, midiEvt);
                break;
            } else {
                // a NOTE_ON with zero velocity will be set as a NOTE_OFF
                MidiEvent midiEvt(NOTE_OFF,
                                  low_half_byte_to_uint32(evtType),
                                  eventStartTime,
                                  para1, para2);
                addEventToTrack(trackid, midiEvt);
                break;
            }
            break;
        case PROGRAM_CHANGE:
        case CHANNEL_PRESSURE:
            read(fd, &para1, sizeof(para1));
            break;
        case 0xF0:
            if ((evtType & 0x0F) == 0x0F) { //Meta Event
                byte metaType;
                byte tmp4ByteBuffer[4];
                read(fd, &metaType, sizeof(metaType));
                switch (metaType) {
                case END_OF_TRACK:
                    lseek(fd, 1, SEEK_CUR); //FF 2F [00]
                    ALREADY_END_OF_TRACK = true;
                    break;
                case SET_TEMPO:
                    lseek(fd, 1, SEEK_CUR); //FF 51 [03] tt tt tt
                    lseek(fd, 3, SEEK_CUR); // TODO
                    break;
                case TIME_SIGNATURE:
                    lseek(fd, 1, SEEK_CUR); // FF 58 [04] nn dd cc bb
                    lseek(fd, 4, SEEK_CUR); // TODO
                    break;
                case KEY_SIGNATURE:
                    lseek(fd, 1, SEEK_CUR); // FF 59 [02] sf mi
                    lseek(fd, 2, SEEK_CUR); // TODO
                    break;
                case TEXT_EVENT:
                case COPYRIGHT:
                case TRACK_NAME:
                case INSTRUMENT_NAME:
                case LYRIC:
                case MARKER:
                case CUE_POINT:
                case Sequencer_Specific_Meta_event:
                default: //Text-like Event
                    uint32_t len = readVariableLengthQuantity(fd);
                    byte* textBuffer = (byte*)malloc(len); // free within ~MidiEvent()
                    read(fd, textBuffer, len);
                    free(textBuffer);
                    break;
                }
            } else if((evtType&0x0F)==0x00||(evtType&0x0F)==0x07) { //SysEx
                uint32_t len = readVariableLengthQuantity(fd);
                lseek(fd, len, SEEK_CUR);
            } else {
                cout << "[Invalid Meta / SysEX Event!] Repeating last event" << tmpCurTime << evtType << endl;
            }
            break;
        default:
            printf("[Invalid Event!] evtType = %02X\n", evtType);
            //lseek(fd, -1, SEEK_CUR);
            evtType = lastType;
            //goto retry;
            break;
        }
        lastType = evtType;
    }


    // read done
	trackList.push_back(aTrack);
	lseek(fd, chunkStartPos + chunkLength, SEEK_SET);

	return 1;
}

bool MidiObject::loadFromMidiFile(const char* filename) {

	byte buffer[8];

	int filefd = open(filename, O_RDONLY);
	if (filefd == -1) {
		return false;
	}

	headerSize = read(filefd, &headerChunk, sizeof(HeaderChunk));
	if (strncmp((const char*)headerChunk.chunkType, "MThd", 4) != 0) {
		cerr << "Midi file header not correct, should be \"MThd\", not: " << headerChunk.chunkType <<endl;
		close(filefd); return false;
	}
	format = byte2_to_uint16(headerChunk.midiFormat);
	trackCnt = byte2_to_uint16(headerChunk.midiTrackCnt) - 1; // dont know why should -1
	cout << format << " " << trackCnt <<endl;

	for (uint32_t trackProcessed = 0; trackProcessed < trackCnt; /* nothing */) {
		int readResult = trackReader(filefd, trackProcessed); // provide a offset for file pointer?
		if (readResult > 0) trackProcessed += readResult;
		cout << "Now processed " << trackProcessed << " / " << trackCnt << " Tracks" <<endl;
	}

	close(filefd);
	return true;
}

bool MidiObject::addEventToTrack(int trackid, MidiEvent event) {
    EventType evtType = event.getEventType();
    switch(evtType) {
    case NOTE_ON:
        //cout << "[NOTE ON] inserted a note on event!" << endl;
        break;
    case NOTE_OFF:
        //cout << "[NOTE OFF] inserted a note off event!" << endl;
        break;
    default:
        printf("[Meta or Invalid Event!] evtType = %02X\n", evtType);
        break;
    }
    trackList[trackid].eventList.push_back(event);
    return true;
}

uint32_t MidiObject::getNoteCnt() {
    return noteCnt;
}
