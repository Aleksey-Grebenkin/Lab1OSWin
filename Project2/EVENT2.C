#include <windows.h>
#include <stdio.h>
#include <conio.h>

// �������������� ��������-�������, ������� ������������
// ��� ������������� �����, ������������� ������ ���������
HANDLE hEvent;
HANDLE hEventTermination;

// ��� �������-������� ��� ������������� ����� � �����������
CHAR lpEventName[] = 
  "$MyVerySpecialEventName$";

// ��� �������-������� ��� ���������� ��������
CHAR lpEventTerminationName[] = 
  "$MyVerySpecialEventTerminationName$";

int main()
{
  CHAR chr;

  printf("Event demo application, slave process\n"
    "(C) A. Frolov, 1996, Email: frolov@glas.apc.org\n"
    "\n\nPress <ESC> to terminate...\n");
  
  // ��������� ������-������� ��� ������������� 
  // ����� � �����������
  hEvent = OpenEvent(EVENT_ALL_ACCESS, FALSE, lpEventName);
  
  if(hEvent == NULL)
  {
    fprintf(stdout,"OpenEvent: Error %ld\n", 
      GetLastError());
    _getch();
    return 0;
  }

  // ��������� ������-������� ��� ������������ �
  // ���������� �������� �����
  hEventTermination = OpenEvent(EVENT_ALL_ACCESS, 
    FALSE, lpEventTerminationName);
  
  if(hEventTermination == NULL)
  {
    fprintf(stdout,"OpenEvent (Termination): Error %ld\n", 
      GetLastError());
    _getch();
    return 0;
  }

  // ���� �����. ���� ���� ��������� ���� ������,
  // ����� ������������ �������� ������� <ESC>, 
  // ������� ��� 27
  while(TRUE)
  {
    // ��������� ��� ��������� �������
    chr = _getche();
    
    // ���� ������ ������� <ESC>, ��������� ����
    if(chr == 27)
      break;

    // ������������� ������-������� � ����������
    // ���������. � ����� �� ��� ������� �����������
    // ������� �� ���� ������� ������ '*'
    SetEvent(hEvent);
  }
 
  // ����� ���������� ����� ����������� ��� �������
  // � ���������� ��������� ��� ������ �������� �
  // �������� ����������� � ��� ���������� ����� ��������
  SetEvent(hEvent);
  SetEvent(hEventTermination);
  
  // ��������� �������������� ��������-�������
  CloseHandle(hEvent);
  CloseHandle(hEventTermination);

  return 0;
}

