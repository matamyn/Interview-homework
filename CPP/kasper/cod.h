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

//! Abstract output class
class LoggerBase {
public:
  virtual void LogLine(const char *szOut) = 0;
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

//! Async action
class AsyncCall_1 : public AsyncCallImplT<AsyncCall_1> {
public:
  AsyncCall_1(LoggerBase *pOutput, AsyncCallBase *pControl);

  ~AsyncCall_1();
  virtual void Call();

protected:
  LoggerBase *m_pOutput_log_base;
};

//! Async action
class AsyncCall_0 : public AsyncCallImplT<AsyncCall_0> {
public:
  AsyncCall_0(LoggerBase *pOutput, AsyncCallBase *pControl);

  virtual ~AsyncCall_0();

  void Call();

protected:
  LoggerBase *m_pOutput_log_base;
};

class LoggerImpl : public LoggerBase {
public:
  LoggerImpl(CritSec &oRM);

  ~LoggerImpl();

  // LoggerBase
  virtual void LogLine(const char *szOut);

protected:
  CritSec &m_oRM; // protects class variables
  FILE *m_pOutput;
};

class TheFactoryImpl : public LoggerImpl, public AsyncCallBase {
public:
  TheFactoryImpl();

  ~TheFactoryImpl();

  void Start();

protected:
  void Destroy();

  // AsyncCallBase
  virtual void DoCall();

protected:
  CritSec m_oRM; // protects class variables
  std::unique_ptr<AsyncCall_1> m_pAsyncCall1;
  std::unique_ptr<AsyncCall_0> m_pAsyncCall2;
};
}; // namespace TASK6044