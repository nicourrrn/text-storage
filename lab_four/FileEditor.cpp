#include <iostream>
#include <tchar.h>
#include "FileEditor.h"
#include "utils.h"

FileEditor new_file_editor(FileData df, File file) {
	return FileEditor{ df, file, 0 };
}

FileEditor open_file_editor(TCHAR* name, u32 max_size) {
	auto file = CreateFile(name, GENERIC_ALL, FILE_SHARE_READ, 0, OPEN_ALWAYS, 0, 0);
	free(name);
	DWORD size_low, size_high;
	size_low = GetFileSize(file, &size_high);
	FileData fd;
	if (size_low == 0 && size_high == 0) {
		fd = from_max_size(max_size);
	}
	else {
		fd = from_file(file);
	}
	auto editor = new_file_editor(fd, file);
	editor.pos = 12;
	printf("DEGUB open_file -> editor.size: %d", editor.file_data.size);
	update_file_data(&editor);

	return editor;
}

void update_file_data(FileEditor *editor) {
	SetFilePointer(editor->file, 0, NULL, FILE_BEGIN);
	WriteFile(editor->file, &editor->file_data.count, 4, NULL, 0);
	WriteFile(editor->file, &editor->file_data.size, 4, NULL, 0);
	WriteFile(editor->file, &editor->file_data.max_size, 4, NULL, 0);
}


bool write_message(FileEditor* editor, char* content, u32 message_size) {
	bool highest_than_max = message_size + editor->file_data.size > editor->file_data.max_size;
	if (highest_than_max) {
		return false;
	}
	editor->file_data.count++;
	editor->file_data.size += message_size;
	
	update_file_data(editor);
	SetFilePointer(editor->file, 0, NULL, FILE_END);
	editor->file;
	WriteFile(editor->file, &message_size, 4, NULL, 0);
	WriteFile(editor->file, content, message_size, NULL, 0);

	return true;
}

char *read_message(FileEditor *editor, u32 number) {
	if (number > editor->file_data.count) {
		return 0;
	}
	SetFilePointer(editor->file, 12, NULL, FILE_BEGIN);
	u32 buf = 0;
	for (u32 i = 0; i < number - 1; i++) {
		ReadFile(editor->file, &buf, 4, NULL, 0);
		SetFilePointer(editor->file, buf, NULL, FILE_CURRENT);
	}
	ReadFile(editor->file, &buf, 4, NULL, 0);
	char* content = (char*)malloc(buf + 2);
	*(content + buf) = 0;
	*(content + buf + 1) = 0;
	ReadFile(editor->file, content, buf, NULL, 0);

	return content;

}


bool delete_message(FileEditor* editor, u32 number) {
	if (editor->file_data.count < number) {
		return false;
	}

	SetFilePointer(editor->file, 12, NULL, FILE_BEGIN);
	editor->pos = 12;
	u32 buf = 0;
	for (u32 i = 0; i < number - 1; i++) {
		ReadFile(editor->file, &buf, 4, NULL, 0);
		SetFilePointer(editor->file, buf, NULL, FILE_CURRENT);
		editor->pos += 4 + buf;
	}
	ReadFile(editor->file, &buf, 4, NULL, 0);
	SetFilePointer(editor->file, -4, NULL, FILE_CURRENT);

	char* text_buf = (char*)malloc(buf);

	while (editor->pos + buf * 2 < editor->file_data.size) {
		SetFilePointer(editor->file, buf, NULL, FILE_CURRENT);
		ReadFile(editor->file, &text_buf, buf, NULL, 0);
		SetFilePointer(editor->file, -2 * buf, NULL, FILE_CURRENT);
		WriteFile(editor->file, &text_buf, buf, NULL, 0);
		editor->pos += buf;
	}
	free(text_buf);
	char char_buf;

	while (editor->pos + buf < editor->file_data.size) {
		SetFilePointer(editor->file, buf, NULL, FILE_CURRENT);
		ReadFile(editor->file, &char_buf, 1, NULL, 0);
		SetFilePointer(editor->file, -1 * (long int) buf - 1, NULL, FILE_CURRENT);
		WriteFile(editor->file, &char_buf, 1, NULL, 0);
		editor->pos ++;
	}

	editor->file_data.count--;
	editor->file_data.size -= buf;
	SetEndOfFile(editor->file);
	update_file_data(editor);
	
	return true;
}

void delete_all_messages(FileEditor* editor) {
	editor->file_data.count = 0;
	editor->file_data.size = 0;
	update_file_data(editor);
	SetEndOfFile(editor->file);
}


void close_editor(FileEditor* editor) {
	update_file_data(editor);
	CloseHandle(editor->file);
}