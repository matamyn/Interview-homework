/*
    Задание

    Указать те ошибки в коде, которые приводят к неправильной работе программы.
   При этом развернуто и подробно описать, в чем каждая из таких ошибок
   заключается, как проявляется, к каким именно проблемам приводит. Исправлять
   найденные ошибки не следует.  Про артефакты, которые не приводят к
   неправильной работе программы, писать не следует -- за спам снимаем баллы.
*/

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
class AsyncCallBase {
public:
  virtual void DoCall() = 0;
};

/*! Implementation of an async. call

    \tparam class that must support method 'void TT::Call()'
*/
template <class T> class AsyncCallImplT {
  AsyncCallImplT &operator=(const AsyncCallImplT &);
  AsyncCallImplT(const AsyncCallImplT &);

public:
  AsyncCallImplT(AsyncCallBase *pControl) : m_pControl(pControl) { Create(); };

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
      static_cast<T *>(this)->Call();
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
  AsyncCallBase *m_pControl;
};

//! Abstract output class
class LoggerBase {
public:
  virtual void LogLine(const char *szOut) = 0;
};

//! Async action
class AsyncCall_1 : public AsyncCallImplT<AsyncCall_1> {
public:
  AsyncCall_1(LoggerBase *pOutput, AsyncCallBase *pControl)
      : AsyncCallImplT<AsyncCall_1>(pControl) {
    m_pOutput = pOutput;
  };

  ~AsyncCall_1() { m_pOutput = nullptr; };

  virtual void Call() {
    for (size_t i = 0; i < 100u; ++i) {
      std::ostringstream os;
      os << "Test1 #" << (i + 1);
      m_pOutput->LogLine(os.str().c_str());
    };
  };

protected:
  LoggerBase *m_pOutput;
};

//! Async action
class AsyncCall_0 : public AsyncCallImplT<AsyncCall_0> {
public:
  AsyncCall_0(LoggerBase *pOutput, AsyncCallBase *pControl)
      : AsyncCallImplT<AsyncCall_0>(pControl) {
    m_pOutput = pOutput;
  };

  virtual ~AsyncCall_0() { m_pOutput = nullptr; };

  void Call() {
    for (size_t i = 0; i < 1000u; ++i) {
      std::ostringstream os;
      os << "Test2 #" << (i + 1);
      m_pOutput->LogLine(os.str().c_str());
    };
  };

protected:
  LoggerBase *m_pOutput;
};

class LoggerImpl : public LoggerBase {
public:
  LoggerImpl(CritSec &oRM) : m_pOutput(nullptr), m_oRM(oRM) {
    m_pOutput = fopen("result.log", "w");
    TASK6044_CHK_ERRNO(m_pOutput);
  };

  ~LoggerImpl() {
    if (m_pOutput) {
      fclose(m_pOutput);
      m_pOutput = nullptr;
    };
  };

  // LoggerBase
  virtual void LogLine(const char *szOut) {
    AutoCritSec arm(m_oRM);
    if (m_pOutput) {
      TASK6044_CHK_ERRNO(EOF != fputs(szOut, m_pOutput));
    };
  };

protected:
  CritSec &m_oRM; // protects class variables
  FILE *m_pOutput;
};

class TheFactoryImpl : public LoggerImpl, public AsyncCallBase {
public:
  TheFactoryImpl()
      : LoggerImpl(m_oRM), m_pAsyncCall1(nullptr), m_pAsyncCall2(nullptr) {
    Start();
  };

  ~TheFactoryImpl() { Destroy(); };

  void Start() {
    AutoCritSec arm(m_oRM);
    m_pAsyncCall1.reset(new AsyncCall_1(this, this));
    m_pAsyncCall2.reset(new AsyncCall_0(this, this));
  };

protected:
  void Destroy() {
    AutoCritSec arm(m_oRM);
    m_pAsyncCall1.reset();
    m_pAsyncCall2.reset();
  };

  // AsyncCallBase
  virtual void DoCall() { ; };

protected:
  CritSec m_oRM; // protects class variables
  std::unique_ptr<AsyncCall_1> m_pAsyncCall1;
  std::unique_ptr<AsyncCall_0> m_pAsyncCall2;
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