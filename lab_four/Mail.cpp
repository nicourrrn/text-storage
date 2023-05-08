#include <tchar.h>
#include <iostream>

#include "Mail.h"

TCHAR* input_line(int size) {
	TCHAR* buf = (TCHAR*)malloc(size * sizeof(TCHAR));
	for (int i = 0; i < size; i++) {
		buf[i] = _T('\0');
	}

	int i = 0, ch = _gettchar();
	while (i < size - 1 && ch != EOF && ch != _T('\n')) {
		buf[i] = ch;
		i++;
		ch = _gettchar();
	}

	return buf;
}

u32 input_u32() {
	TCHAR buf[16];
	int i = 0;
	for (; i < 16; i++) {
		buf[i] = _T('\0');
	}
	i--;

	int ch = _gettchar();
	while (i > 0 && ch != EOF && ch != _T('\n')) {
		buf[i] = ch;
		i--;
		ch = _gettchar();
	}
	i++;
	u32 result = 0, num_size = i;
	for (; i < 16; i++) {
		result += (buf[i] - _T('0')) * pow(10, i - num_size);
	}
	return result;

}

TCHAR* name_to_path(TCHAR* base_name) {
	u32 size = 0;
	for (; *(base_name + size) != _T('\0'); size++);
	TCHAR* name = (TCHAR*)malloc((12 + size) * sizeof(TCHAR));
	TCHAR start_name[] = _T("emails/");
	TCHAR end_name[] = _T(".bin");

	for (int i = 0; i < 7; i++) {
		*(name + i) = start_name[i];
	}

	for (int i = 0; i < size; i++) {
		*(name + 7 + i) = *(base_name + i);
	}

	for (int i = 0; i < 5; i++) {
		*(name + 7 + size + i) = end_name[i];
	}
	return name;
}

bool was_created(TCHAR* path) {
	WIN32_FIND_DATA data;
	auto founded = FindFirstFile(path, &data);
	bool result = founded != INVALID_HANDLE_VALUE;
	FindClose(founded);
	return result;
}

FileEditor open_mail(bool is_new) {
	CreateDirectory(L"emails", 0);

	_tprintf(_T("Назва скриньки: "));
	TCHAR* base_name = input_line(64);
	TCHAR* name = name_to_path(base_name);
	free(base_name);
	bool created = was_created(name);
	if ((is_new && created) || (!is_new && !created)) {
		free(name);
		_tprintf(_T("Оберіть іншу операцію для скриньки з цим ім'ям\n"));
		return FileEditor{};
	}

	u32 max_size = 0;
	if (is_new) {
		_tprintf(_T("Максимальний розмір(в байтах): "));
		max_size = input_u32();
	}

	return open_file_editor(name, max_size);
}

u32 emails_count() {
	WIN32_FIND_DATA data;
	auto founded = FindFirstFile(_T("emails/*.bin"), &data);
	if (founded == INVALID_HANDLE_VALUE) {
		return 0;
	}
	u32 counter = 0;
	do {
		_tprintf(_T("Знайдена скринька: %s\n"), data.cFileName);
		counter++;
	} while (FindNextFile(founded, &data));
	FindClose(founded);
	return counter;
}

void print_info(FileEditor* editor) {
	_tprintf(_T("-------------------- Mail Info --------------------\n"));

	_tprintf(_T("На ящику %d повідомлень\n"), editor->file_data.count);
	_tprintf(_T("Зайнято %d/%d байтів (%d%%)\n"),
		editor->file_data.size, editor->file_data.max_size,
		editor->file_data.size * 100 / editor->file_data.max_size
	);

	_tprintf(_T("---------------------------------------------------\n"));
}

void read_mail(FileEditor* editor) {
	_tprintf(_T("Оберіть номер повідомлення від 0 до %d: "), editor->file_data.count);
	u32 choise = input_u32();

	char* message = read_message(editor, choise);

	_tprintf(_T("Ваше повідомлення:\n	%s\n"), (TCHAR*)message);
	free(message);

	_tprintf(_T("Чи треба його видалити?\n(y/N): "));
	char answer = getchar();
	getchar();
	if (answer == 'y' || answer == 'Y') {
		delete_message(editor, choise);
	}
}

void write_mail(FileEditor* editor) {
	u32 can_write_bytes = editor->file_data.max_size - editor->file_data.size;
	_tprintf(_T("Ви можете ввести %d символів. Ваше повідомлення:\n	"),
		can_write_bytes / 2);

	TCHAR* message = input_line(can_write_bytes / 2);

	u32 size = 0;
	for (; *(message + size) != _T('\0'); size++);
	write_message(editor, (char*)message, size * sizeof(TCHAR));
	free(message);
}

void delete_mail(FileEditor* editor) {
	_tprintf(_T("Оберіть номер повідомлення від 0 до %d: "), editor->file_data.count);
	delete_message(editor, input_u32());
}

void delete_all_mail(FileEditor* editor) {
	_tprintf(_T("Ви впевнені?\n(y/N): "));

	char choise = getchar();
	getchar();
	if (choise == 'y' || choise == 'Y') {
		delete_all_messages(editor);
	}
}
