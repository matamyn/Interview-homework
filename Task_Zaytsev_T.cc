﻿/*
    Задание

    Указать те ошибки в коде, которые приводят к неправильной работе программы.
   При этом развернуто и подробно описать, в чем каждая из таких ошибок
   заключается, как проявляется, к каким именно проблемам приводит. Исправлять
   найденные ошибки не следует.  Про артефакты, которые не приводят к
   неправильной работе программы, писать не следует -- за спам снимаем баллы.
*/

/*
Ошибок из-за которых программа не выполняется по сути две:
Первая связана с особенностью static_cast из-за которого вызываются деструкторы
наследников, которые чистят указатель на интерфейсный касс LoggerBase. Можно
исправить убрав обнуление указателей LoggerBase *m_pOutput в деструкторе. Однако
что тогда делать с потенциальной утечной памяти, мне непонятно. Вторая связана с
особенностью lock_guard который теряет поток, если тот повторно вызывается через
ссылку на уже залоченый мьютекс. Исправить можно убрав передачу CritSec по
ссылке в LoggerImpl (соответственно изменив поле на объект)
*/
/*ошибка*/
#include <cstdio>
#include <exception>
#include <fstream>
#include <iostream>
#include <memory>
#include <mutex>
#include <sstream>
#include <string>
#include <thread>

namespace TASK6044 {
typedef std::runtime_error Exception;

//! throws an OsError exception based on errno if bResult is zero
void Check_errno(bool bResult, const char *file, int line) {
  if (!bResult) {
    fprintf(stderr, "An error %d occurred", errno);
    std::ostringstream os;
    os << "Error with code " << errno << " occurred at " << file << "@" << line;
    throw Exception(os.str().c_str());
  };
};

//! check last error and throws an OsError exception if res is zero
#define TASK6044_CHK_ERRNO(res)                                                \
  TASK6044::Check_errno(!!(res), __FILE__, __LINE__)

//! throws an OsError exception based on errno if bResult is zero
void Assertion(bool bResult, const char *szText, const char *file, int line) {
  if (!bResult) {
    std::ostringstream os;
    os << "Assertion " << szText << " failed at " << file << "@" << line;
    throw Exception(os.str().c_str());
  };
};

//! throws an exception is condition is false
#define TASK6044_ASSERTION(res)                                                \
  TASK6044::Assertion(!!(res), #res, __FILE__, __LINE__)

//! Recursive synchronization object
typedef std::recursive_mutex CritSec;

//! RAII helper for the recursive synchronization object
typedef std::lock_guard<std::recursive_mutex> AutoCritSec;

//! Abstract control class
class IAsyncCallBase {
public:
  virtual void DoCall() = 0;
};

/*! Implementation of an async. call

    \tparam class that must support method 'void TT::Call()'
*/
template <class T> class AsyncCallImplT {
  AsyncCallImplT &operator=(const AsyncCallImplT &);
  AsyncCallImplT(const AsyncCallImplT &);

  //Метод Create() будет вызван раньше завершения конструирования объекта Т, в
  //данном случаи AsyncCall_* т.к. указатель LoggerBase *m_pOutput
  //инициализируется в конструкторе AsyncCall_*, поведение при обращении к нему
  //в созданном потоке метода thread_proc вызовет SIGSEGV.
public:
  AsyncCallImplT(IAsyncCallBase *pControl) : m_pControl(pControl) { /*ошибка*/
    Create();
  };

  ~AsyncCallImplT() { Destroy(); };

private:
  void Create() {
    m_pThread.reset(new std::thread(&AsyncCallImplT::thread_proc, this));
  };

  void Destroy() {
    if (m_pThread) {
      TASK6044_ASSERTION(m_pThread->joinable());
      m_pThread->join();
      TASK6044_ASSERTION(!m_pThread->joinable());
      m_pThread = nullptr;
    };
  };

  void thread_proc() {
    try {
      //приведение типа к AsyncCall_*, проблема восходящего преобразования,
      //может возникать непроделенное поведение в runtime.

      // static_cast имеет семантику move, в следствии чего будет вызван
      //деструктор класса TheFactoryImpl, в следствии чего будут вызваны
      //деструкторы наследуемых классов и композитных объектов, в частности
      //деструктор AsyncCall_*, который обнулит указатель m_pOutput
      //(из AsyncCall_ на абстрактный класс)
      //из за чего произойдет SIGSEGV на обращени к перегруженному методу
      // LogLine в методе Call.
      /*ошибка*/ static_cast<T *>(this)->Call();
      if (this->m_pControl)
        this->m_pControl->DoCall();
    } catch (const TASK6044::Exception &excpt) {
      std::cerr << "TASK6044::Exception occured: " << excpt.what();
    } catch (const std::exception &excpt) {
      std::cerr << "Unexpected exception: " << excpt.what();
    };
  };

private:
  std::unique_ptr<std::thread> m_pThread;
  IAsyncCallBase *m_pControl;
};

//! Abstract output class
class ILoggerBase {
public:
  virtual void LogLine(const char *szOut) = 0;
};

//! Async action
class AsyncCall_1 : public AsyncCallImplT<AsyncCall_1> {
public:
  AsyncCall_1(ILoggerBase *pOutput, IAsyncCallBase *pControl)
      : AsyncCallImplT<AsyncCall_1>(pControl) {
    /*потенциальная ошибка*/ m_pOutput = pOutput;
  };
  ~AsyncCall_1() { /*ошибка*/
    m_pOutput = nullptr;
  };

  virtual void Call() {
    for (size_t i = 0; i < 100u; ++i) {
      std::ostringstream os;
      os << "Test1 #" << (i + 1);
      m_pOutput->LogLine(os.str().c_str());
    };
  };

protected:
  ILoggerBase *m_pOutput;
};

//! Async action
class AsyncCall_0 : public AsyncCallImplT<AsyncCall_0> {
public:
  AsyncCall_0(ILoggerBase *pOutput, IAsyncCallBase *pControl)
      : AsyncCallImplT<AsyncCall_0>(pControl) {
    /*потенциальная ошибка*/ m_pOutput = pOutput;
  };

  virtual ~AsyncCall_0() { /*ошибка*/
    m_pOutput = nullptr;
  };

  void Call() {
    for (size_t i = 0; i < 1000u; ++i) {
      std::ostringstream os;
      os << "Test2 #" << (i + 1);
      m_pOutput->LogLine(os.str().c_str());
    };
  };
  // П.С.
  // Я не смог придумать как стоит поменять указатель на *m_pOutput,
  // что-бы он не обнулся до завершения работы потока.
  // При использовании умного указателя и переносе общего поля m_pOutput из
  // AsyncCall* в AsyncCallImplT позволит избавиться от необходимости
  // контролировать деструкторы класса AsyncCall_* и невилирует часть проблем с
  // восходящим преобразованием. Однако проблему не решит.
protected:
  ILoggerBase *m_pOutput;
};

class LoggerImpl : public ILoggerBase {
public:
  LoggerImpl(/*ошибка*/ CritSec &oRM) : m_pOutput(nullptr), m_oRM(oRM) {
    m_pOutput = fopen("result.log", "w");
    TASK6044_CHK_ERRNO(m_pOutput);
  };

  ~LoggerImpl() {
    if (m_pOutput) {
      fclose(m_pOutput);
      m_pOutput = nullptr;
    };
  };

  // ILoggerBase
  virtual void LogLine(const char *szOut) {
    //Деструктор класса TheFactoryImpl уже вызвал lock_guard с объектом CritSec,
    //который в данный момент находится в ожидании завершения.
    //Из за передачи CritSec по ссылке в новый lock_guard, произойдет
    //неотслеживаемое поведение

    //Деструктор AsyncCallImplT не дождется завершения потоков на.join() и уйдет
    //в вечное ожидание
    AutoCritSec arm(m_oRM);
    if (m_pOutput) {
      TASK6044_CHK_ERRNO(EOF != fputs(szOut, m_pOutput));
    };
  };

protected:
  CritSec /*ошибка*/ &m_oRM; // protects class variables
  FILE *m_pOutput;
};

class TheFactoryImpl : public LoggerImpl, public IAsyncCallBase {
public:
  TheFactoryImpl()
      : LoggerImpl(m_oRM), m_pAsyncCall1(nullptr), m_pAsyncCall0(nullptr) {
    Start();
  };

  ~TheFactoryImpl() { Destroy(); };

  void Start() {
    AutoCritSec arm(m_oRM);
    m_pAsyncCall1.reset(new AsyncCall_1(this, this));
    m_pAsyncCall0.reset(new AsyncCall_0(this, this));
  };

protected:
  void Destroy() {
    AutoCritSec arm(m_oRM);
    m_pAsyncCall1.reset();
    m_pAsyncCall0.reset();
  };

  // IAsyncCallBase
  virtual void DoCall() { ; };

protected:
  CritSec m_oRM; // protects class variables
  std::unique_ptr<AsyncCall_1> m_pAsyncCall1;
  std::unique_ptr<AsyncCall_0> m_pAsyncCall0;

  FILE *m_pOutput;
};
}; // namespace TASK6044

int main() {
  try {
    TASK6044::TheFactoryImpl oApp;
  } catch (const TASK6044::Exception &excpt) {
    std::cerr << "TASK6044::Exception occured: " << excpt.what();
    return 1;
  } catch (const std::exception &excpt) {
    std::cerr << "Unexpected exception: " << excpt.what();
    return 3;
  };
  return 0;
};
