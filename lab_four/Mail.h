#pragma once

#include "utils.h"
#include "FileEditor.h"

TCHAR* input_line(int size);
u32 input_u32();
TCHAR* name_to_path(TCHAR* base_name);
bool was_created(TCHAR* path);
FileEditor open_mail(bool is_new);
u32 emails_count();
void print_info(FileEditor* editor);
void read_mail(FileEditor* editor);
void write_mail(FileEditor* editor);
void delete_mail(FileEditor* editor);
void delete_all_mail(FileEditor* editor);

