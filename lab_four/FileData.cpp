#include <iostream>
#include <windows.h>
#include "FileData.h"

FileData from_max_size(u32 max_size) {
	return FileData{ 0, 0 , max_size };
}

FileData from_file(File file) {
	SetFilePointer(file, 0, NULL, FILE_BEGIN);
	FileData fd;
	ReadFile(file, &fd.count, 4, NULL, 0);
	ReadFile(file, &fd.size, 4, NULL, 0);
	ReadFile(file, &fd.max_size, 4, NULL, 0);
	return fd;
}

