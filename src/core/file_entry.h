
#ifndef FILEENTRY_H
#define FILEENTRY_H

#include <string>

struct file_entry{
	int64_t size;
	std::string path;
	int64_t offset;
	int index;
};

#endif