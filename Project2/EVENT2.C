#include <windows.h>
#include <stdio.h>
#include <conio.h>

// Идентификаторы объектов-событий, которые используются
// для синхронизации задач, принадлежащих разным процессам
HANDLE hEvent;
HANDLE hEventTermination;

// Имя объекта-события для синхронизации ввода и отображения
CHAR lpEventName[] = 
  "$MyVerySpecialEventName$";

// Имя объекта-события для завершения процесса
CHAR lpEventTerminationName[] = 
  "$MyVerySpecialEventTerminationName$";

int main()
{
  CHAR chr;

  printf("Event demo application, slave process\n"
    "(C) A. Frolov, 1996, Email: frolov@glas.apc.org\n"
    "\n\nPress <ESC> to terminate...\n");
  
  // Открываем объект-событие для синхронизации 
  // ввода и отображения
  hEvent = OpenEvent(EVENT_ALL_ACCESS, FALSE, lpEventName);
  
  if(hEvent == NULL)
  {
    fprintf(stdout,"OpenEvent: Error %ld\n", 
      GetLastError());
    _getch();
    return 0;
  }

  // Открываем объект-событие для сигнализации о
  // завершении процесса ввода
  hEventTermination = OpenEvent(EVENT_ALL_ACCESS, 
    FALSE, lpEventTerminationName);
  
  if(hEventTermination == NULL)
  {
    fprintf(stdout,"OpenEvent (Termination): Error %ld\n", 
      GetLastError());
    _getch();
    return 0;
  }

  // Цикл ввода. Этот цикл завершает свою работу,
  // когда пользователь нажимает клавишу <ESC>, 
  // имеющую код 27
  while(TRUE)
  {
    // Проверяем код введенной клавиши
    chr = _getche();
    
    // Если нажали клавишу <ESC>, прерываем цикл
    if(chr == 27)
      break;

    // Устанавливаем объект-событие в отмеченное
    // состояние. В ответ на это процесс отображения
    // выведет на свою консоль символ '*'
    SetEvent(hEvent);
  }
 
  // После завершения цикла переключаем оба события
  // в отмеченное состояние для отмены ожидания в
  // процессе отображения и для завершения этого процесса
  SetEvent(hEvent);
  SetEvent(hEventTermination);
  
  // Закрываем идентификаторы объектов-событий
  CloseHandle(hEvent);
  CloseHandle(hEventTermination);

  return 0;
}

