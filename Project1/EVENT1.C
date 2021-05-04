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
  DWORD dwRetCode;

  printf("Event demo application, master process\n"
    "(C) A. Frolov, 1996, Email: frolov@glas.apc.org\n");
  
  // Создаем объект-событие для синхронизации 
  // ввода и отображения, выполняемого в разных процессах
  hEvent = CreateEvent(NULL, FALSE, FALSE, lpEventName);
  
  // Если произошла ошибка, получаем и отображаем ее код,
  // а затем завершаем работу приложения
  if(hEvent == NULL)
  {
    fprintf(stdout,"CreateEvent: Error %ld\n", 
      GetLastError());
    _getch();
    return 0;
  }

  // Если объект-событие с указанным именем существует,
  // считаем, что приложение EVENT уже было запущено
  if(GetLastError() == ERROR_ALREADY_EXISTS)
  {
    printf("\nApplication EVENT already started\n"
      "Press any key to exit...");
    _getch();
    return 0;
  }

  // Создаем объект-событие для определения момента
  // завершения работы процесса ввода
  hEventTermination = CreateEvent(NULL, 
    FALSE, FALSE, lpEventTerminationName);

  if(hEventTermination == NULL)
  {
    fprintf(stdout,"CreateEvent (Termination): Error %ld\n", 
      GetLastError());
    _getch();
    return 0;
  }
  
  // Цикл отображения. Этот цикл завершает свою работу
  // при завершении процесса ввода
  while(TRUE)
  {
    // Проверяем состояние объекта-события, отвечающего
    // за контроль завершения процесса ввода. Так как
    // указано нулевое время ожидания, такая проверка
    // не уменьшает заметно скорость работы приложения
    dwRetCode = WaitForSingleObject(hEventTermination, 0);

    // Если объект-событие перешел в отмеченное состояние,
    // если процесс ввода завершил свою работу, или
    // если при ожидании произошла ошибка,
    // останавливаем цикл отображения
    if(dwRetCode == WAIT_OBJECT_0  || 
       dwRetCode == WAIT_ABANDONED ||
       dwRetCode == WAIT_FAILED)
      break;

    // Выполняем ожидание ввода символа в процессе,
    // который работает с клавиатурой
    dwRetCode = WaitForSingleObject(hEvent, INFINITE);
    
    // При возникновении ошибки прерываем цикл
    if(dwRetCode == WAIT_FAILED || 
       dwRetCode == WAIT_ABANDONED)
      break;

    // В ответ на каждый символ, введенный процессом, который
    // работает с клавиатурой, отображаем символ '*'
    _putch('*');
  }
 
  // Закрываем идентификаторы объектов-событий  
  CloseHandle(hEvent);
  CloseHandle(hEventTermination);

  return 0;
}

