#include "NEATDoubleBuffer.h"
#include "Utils.h"

NEATDoubleBuffer::NEATDoubleBuffer() : neats(&neatBuff1), oldNeats(&neatBuff2) {

}

void NEATDoubleBuffer::swapBuffers() {
    Utils::swap<std::vector<NEAT>*>(neats, oldNeats);
}

void NEATDoubleBuffer::setBuffSize(int numAI) {
    oldNeats->resize(numAI);
}