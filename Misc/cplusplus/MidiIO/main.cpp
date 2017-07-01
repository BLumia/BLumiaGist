#include <iostream>
#include "midistruct.hpp"
#include "midiio.hpp"

using namespace std;
using namespace MidiUtils;

int main(int argc, char** argv) {

	MidiObject a;
	bool result = a.loadFromMidiFile("./test.mid");
	cout << a.getNoteCnt() << endl;

	return 0;
}
