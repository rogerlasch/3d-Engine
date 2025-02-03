
#include <chrono>
#include"../types.h"
#include"../time.h"
#include "FileData.h"

using namespace std;

namespace gpp{

// Implementação de FileDataReference
FileDataReference::FileDataReference(FileData* hfile) : hfile(hfile) {
    if (this->hfile != nullptr) {
        this->hfile->adopt();
    }
}

FileDataReference::~FileDataReference() {
release();
}

bool FileDataReference::empty() const {
    return ((hfile == nullptr) || (hfile->data.empty()));
}

void FileDataReference::release(){
    if (hfile) {
        hfile->release();
    }
    hfile = nullptr;
}

string FileDataReference::getFileName() const {
    return (hfile != nullptr ? hfile->filename : "");
}

void* FileDataReference::getData() const {
    return (hfile != nullptr ? (void*)hfile->data.data() : nullptr);
}

uint64 FileDataReference::getSize() const {
    return (hfile != nullptr ? hfile->data.size() : 0);
}

// Implementação de FileData
FileData::FileData() {
    refcount.store(0);
    lastTime.store(get_timestamp_sec());
}

FileData::~FileData() {
    filename.clear();
    data.clear();
}

bool FileData::canRemove(int64 hseconds) const {
    if (refcount.load() > 0) {
        return false;
    }

    int64 now = get_timestamp_sec();
    int64 elapsed = now - lastTime.load();
    return elapsed > hseconds;
}

void FileData::cleanup() {
    filename.clear();
    data.clear();
    refcount.store(0);
    lastTime.store(get_timestamp_sec());
}

FileDataRef FileData::getReference() {
    return make_shared<FileDataReference>(this);
}

void FileData::adopt() {
    refcount.fetch_add(1, memory_order_relaxed);
}

void FileData::release() {
    if (refcount.fetch_sub(1, memory_order_acq_rel) > 0) {
        lastTime.store(get_timestamp_sec(), memory_order_relaxed);
    }
}
}
