#pragma once
#include "FileData.h"
#include "utils.h"

struct FileEditor {
	FileData file_data;
	File file;
	u32 pos;
};

FileEditor open_file_editor(TCHAR* name, u32 max_size);
FileEditor new_file_editor(FileData df, File file);

void update_file_data(FileEditor* editor);
bool write_message(FileEditor* editor, char* content, u32 message_size);
char *read_message(FileEditor* editor, u32 number);
void delete_all_messages(FileEditor* editor);
bool delete_message(FileEditor* editor, u32 number);
void close_editor(FileEditor* editor);