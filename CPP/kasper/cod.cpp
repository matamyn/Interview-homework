#include "cod.h"

namespace TASK6044 {

AsyncCall_1::AsyncCall_1(LoggerBase *pOutput, AsyncCallBase *pControl)
    : AsyncCallImplT<AsyncCall_1>(pControl) {
  m_pOutput_log_base = pOutput;
};

AsyncCall_1::~AsyncCall_1() { m_pOutput_log_base = nullptr; };

void AsyncCall_1::Call() {
  for (size_t i = 0; i < 100u; ++i) {
    std::ostringstream os;
    os << "Test1 #" << (i + 1);
    m_pOutput_log_base->LogLine(os.str().c_str());
  }
}
//==============================================================================
//==============================================================================
//==============================================================================
//! Async action
AsyncCall_0::AsyncCall_0(LoggerBase *pOutput, AsyncCallBase *pControl)
    : AsyncCallImplT<AsyncCall_0>(pControl) {
  m_pOutput_log_base = pOutput;
};

AsyncCall_0::~AsyncCall_0() { m_pOutput_log_base = nullptr; };

void AsyncCall_0::Call() {
  for (size_t i = 0; i < 1000u; ++i) {
    std::ostringstream os;
    os << "Test2 #" << (i + 1);
    m_pOutput_log_base->LogLine(os.str().c_str());
  };
};
//==============================================================================
//==============================================================================
//==============================================================================
LoggerImpl::LoggerImpl(CritSec &oRM) : m_pOutput(nullptr), m_oRM(oRM) {
  m_pOutput = fopen("result.log", "w");
  TASK6044_CHK_ERRNO(m_pOutput);
};

LoggerImpl::~LoggerImpl() {
  if (m_pOutput) {
    fclose(m_pOutput);
    m_pOutput = nullptr;
  };
};

// LoggerBase
void LoggerImpl::LogLine(const char *szOut) {
  AutoCritSec arm(m_oRM);
  if (m_pOutput) {
    TASK6044_CHK_ERRNO(EOF != fputs(szOut, m_pOutput));
  };
};
//==============================================================================
//==============================================================================
//==============================================================================
TheFactoryImpl::TheFactoryImpl()
    : LoggerImpl(m_oRM), m_pAsyncCall1(nullptr), m_pAsyncCall2(nullptr) {
  Start();
};

TheFactoryImpl::~TheFactoryImpl() { Destroy(); };

void TheFactoryImpl::Start() {
  AutoCritSec arm(m_oRM);
  m_pAsyncCall1.reset(new AsyncCall_1(this, this));
  m_pAsyncCall2.reset(new AsyncCall_0(this, this));
};

void TheFactoryImpl::Destroy() {
  AutoCritSec arm(m_oRM);
  m_pAsyncCall1.reset();
  m_pAsyncCall2.reset();
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