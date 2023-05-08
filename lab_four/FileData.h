#pragma once
#include "utils.h"

struct FileData {
	u32 count, size, max_size;
};

FileData from_max_size(u32 max_size);

FileData from_file(File file);
