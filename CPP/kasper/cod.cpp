#include "cod.h"

#include <memory>

namespace TASK6044 {

AsyncCall_1::AsyncCall_1(ILoggerBase *pOutput_1, IAsyncCallBase *pControl_1)
    : AsyncCallImplT<AsyncCall_1>(pControl_1){
                                    m_pOutput_1 = pOutput_1;
                              };

AsyncCall_1::~AsyncCall_1() {  };

void AsyncCall_1::Call() {
  for (size_t i = 0; i < 100u; ++i) {
    std::ostringstream os;
    os << "Test1 #" << (i + 1);
    m_pOutput_1->LogLine(os.str().c_str());
  }
}
//==============================================================================
//==============================================================================
//==============================================================================
//! Async action
AsyncCall_0::AsyncCall_0(ILoggerBase *pOutput_0, IAsyncCallBase *pControl_0)
    :m_pOutput_0 (pOutput_0), AsyncCallImplT<AsyncCall_0>(pControl_0) {

};

AsyncCall_0::~AsyncCall_0() = default;

void AsyncCall_0::Call() {
  for (size_t i = 0; i < 1000u; ++i) {
    std::ostringstream os;
    os << "Test2 #" << (i + 1);
    m_pOutput_0->LogLine(os.str().c_str());
  };
};
//==============================================================================
//==============================================================================
//==============================================================================
LoggerImpl::LoggerImpl() : m_pOutput(nullptr) {
  m_pOutput = fopen("/home/timofey/result.log", "w");
  TASK6044_CHK_ERRNO(m_pOutput);
};

LoggerImpl::~LoggerImpl() {
  if (m_pOutput) {
    fclose(m_pOutput);
    m_pOutput = nullptr;
  };
};

// ILoggerBase
void LoggerImpl::LogLine(const char *szOut) {
  //  AutoCritSec arm(m_oRM);
  m_oRM.lock();
  if (m_pOutput) {
    TASK6044_CHK_ERRNO(EOF != fputs(szOut, m_pOutput));
  };
  m_oRM.unlock();
};
//==============================================================================
//==============================================================================
//==============================================================================
TheFactoryImpl::TheFactoryImpl()
    : m_pAsyncCall1(nullptr), m_pAsyncCall0(nullptr) {
  Start();
};

TheFactoryImpl::~TheFactoryImpl() { Destroy(); };

void TheFactoryImpl::Start() {
  //  AutoCritSec arm(m_oRM);
  m_oRM.lock();
  m_pAsyncCall1.reset(new AsyncCall_1(this, this));
  m_pAsyncCall0.reset(new AsyncCall_0(this, this));
  m_oRM.unlock();
};

void TheFactoryImpl::Destroy() {
  //  AutoCritSec arm(m_oRM);
  m_oRM.lock();
  m_pAsyncCall1.reset();
  m_pAsyncCall0.reset();

  m_oRM.unlock();
};

void TheFactoryImpl::DoCall() { ; };
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