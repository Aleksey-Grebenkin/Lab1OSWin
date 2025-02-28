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
  DWORD dwRetCode;

  printf("Event demo application, master process\n"
    "(C) A. Frolov, 1996, Email: frolov@glas.apc.org\n");
  
  // ������� ������-������� ��� ������������� 
  // ����� � �����������, ������������ � ������ ���������
  hEvent = CreateEvent(NULL, FALSE, FALSE, lpEventName);
  
  // ���� ��������� ������, �������� � ���������� �� ���,
  // � ����� ��������� ������ ����������
  if(hEvent == NULL)
  {
    fprintf(stdout,"CreateEvent: Error %ld\n", 
      GetLastError());
    _getch();
    return 0;
  }

  // ���� ������-������� � ��������� ������ ����������,
  // �������, ��� ���������� EVENT ��� ���� ��������
  if(GetLastError() == ERROR_ALREADY_EXISTS)
  {
    printf("\nApplication EVENT already started\n"
      "Press any key to exit...");
    _getch();
    return 0;
  }

  // ������� ������-������� ��� ����������� �������
  // ���������� ������ �������� �����
  hEventTermination = CreateEvent(NULL, 
    FALSE, FALSE, lpEventTerminationName);

  if(hEventTermination == NULL)
  {
    fprintf(stdout,"CreateEvent (Termination): Error %ld\n", 
      GetLastError());
    _getch();
    return 0;
  }
  
  // ���� �����������. ���� ���� ��������� ���� ������
  // ��� ���������� �������� �����
  while(TRUE)
  {
    // ��������� ��������� �������-�������, �����������
    // �� �������� ���������� �������� �����. ��� ���
    // ������� ������� ����� ��������, ����� ��������
    // �� ��������� ������� �������� ������ ����������
    dwRetCode = WaitForSingleObject(hEventTermination, 0);

    // ���� ������-������� ������� � ���������� ���������,
    // ���� ������� ����� �������� ���� ������, ���
    // ���� ��� �������� ��������� ������,
    // ������������� ���� �����������
    if(dwRetCode == WAIT_OBJECT_0  || 
       dwRetCode == WAIT_ABANDONED ||
       dwRetCode == WAIT_FAILED)
      break;

    // ��������� �������� ����� ������� � ��������,
    // ������� �������� � �����������
    dwRetCode = WaitForSingleObject(hEvent, INFINITE);
    
    // ��� ������������� ������ ��������� ����
    if(dwRetCode == WAIT_FAILED || 
       dwRetCode == WAIT_ABANDONED)
      break;

    // � ����� �� ������ ������, ��������� ���������, �������
    // �������� � �����������, ���������� ������ '*'
    _putch('*');
  }
 
  // ��������� �������������� ��������-�������  
  CloseHandle(hEvent);
  CloseHandle(hEventTermination);

  return 0;
}

