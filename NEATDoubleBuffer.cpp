#include "NEATDoubleBuffer.h"
#include "Utils.h"

NEATDoubleBuffer::NEATDoubleBuffer() : neats(&neatBuff1), oldNeats(&neatBuff2) {}

void NEATDoubleBuffer::prepareNewGeneration() {
    oldNeats->clear();
    oldNeats->reserve(neats->size());
}

void NEATDoubleBuffer::swapBuffers() {
    Utils::swap<std::vector<NEAT>*>(neats, oldNeats);
}