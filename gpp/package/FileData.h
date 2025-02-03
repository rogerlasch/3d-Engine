
#ifndef FILEDATA_H
#define FILEDATA_H

#include <string>
#include <vector>
#include <atomic>
#include <memory>

namespace gpp{
class FileData;

class FileDataReference {
private:
    FileData* hfile;

public:
    FileDataReference(FileData* hfile);
    FileDataReference(const FileDataReference& sf) = delete;
    FileDataReference& operator=(const FileDataReference& sf) = delete;
    ~FileDataReference();

    bool empty() const;
void release();
    std::string getFileName() const;
    void* getData() const;
    uint64 getSize() const;
};

using FileDataRef = std::shared_ptr<FileDataReference>;

class FileData {
private:
    std::string filename;
    std::vector<int8> data;
    std::atomic<uint32> refcount;
    std::atomic<int64> lastTime;

public:
    FileData();
    FileData(const FileData& fs) = delete;
    FileData& operator=(const FileData& fs) = delete;
    ~FileData();

    bool canRemove(int64 hseconds) const;
    void cleanup();
    FileDataRef getReference();
    void adopt();
    void release();

    friend class FileDataReference;
friend class PackageManager;
};
}
#endif
