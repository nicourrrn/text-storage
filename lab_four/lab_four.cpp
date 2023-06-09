#include <iostream>
#include <tchar.h>
#include "Mail.h"


TCHAR menu[] = _T("\
\n������ ��:\n	1. �������� ��������\n	2. ³������ ��������\n\
	3. ���������� ��� �����\n	4. ��������� ����������\n\
	5. ������ ����������\n	6. �������� ����������\n\
	7. �������� �� �����������\n	8. ʳ������ ��������\n\
	9. �����\n��� ����: ");


int main() {

	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	setlocale(LC_ALL, "ukrainian");
	

	FileEditor editor;
	short int status = 1;
	u32 choise;
	while (status) {
		_tprintf(menu);
		choise = input_u32();
		switch (choise)
		{
		case 1:
			if (status == 2) {
				close_editor(&editor);
				status = 1;
			}
			editor = open_mail(true);
			if (editor.pos == 12) 
				status = 2;
			break;
		case 2:
			if (status == 2) {
				close_editor(&editor);
				status = 1;
			}
			editor = open_mail(false);
			if (editor.pos == 12)
				status = 2;
			break;
		case 8:
			_tprintf(_T("�������� ������� ��������: %d\n"), emails_count());
			break;
		case 3:
			if (status > 1) {
				print_info(&editor);
				break;
			}
		case 4:
			if (status > 1) {
				read_mail(&editor);
				break;
			}
		case 5:
			if (status > 1) {
				write_mail(&editor);
				break;
			}
		case 6:
			if (status > 1) {
				delete_mail(&editor);
				break;
			}
		case 7:
			if (status > 1) {
				delete_all_mail(&editor);
				break;
			}
		case -1:
			_tprintf(_T("����� �� ����� ����� �� �������\n"));
			break;

		case 9:
			status = 0;
			break;

		default:
			break;
		}

	}

	close_editor(&editor);
	
	return 0;
}

