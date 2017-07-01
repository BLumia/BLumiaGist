#include <cstdlib>
#include "midistruct.hpp"

using namespace MidiUtils;

uint32_t MidiEvent::getTickTime() {
    return this->time;
}

MidiEvent::MidiEvent(EventType evtType, uint32_t channelid, uint32_t evtTime, byte evtPara1, byte evtPara2, byte* data) {
    this->time = evtTime;
    this->type = evtType;
    this->para1 = evtPara1;
    this->para2 = evtPara2;
    if (data != nullptr) this->textLikeData = data;
}

MidiEvent::~MidiEvent() {
    if (this->textLikeData != nullptr) {
        free(this->textLikeData);
    }
}

EventType MidiEvent::getEventType() {
    return this->type;
}
