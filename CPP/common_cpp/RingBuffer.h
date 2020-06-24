#ifndef STARSTAFF_RINGBUFFER_H
#define STARSTAFF_RINGBUFFER_H

#include <QtCore>
#include <memory>
class RingBuffer : public QObject {
  Q_OBJECT

public:
  explicit RingBuffer(quint32 a_size, QObject *parent = nullptr);
  ~RingBuffer() override;
  void clean();
  bool read(float *data, quint32 a_len);
  bool write(float *data, quint32 a_len);
  float getSample();
  void addSample(float sample);
  quint32 availableRead();
  quint32 availableWrite();

private:
  quint32 size_;
  quint32 mask_;
  quint32 idxRead_{};
  quint32 idxWrite_{};
  quint32 dataCount_{};
  float *ptr_buf_ = NULL;
};

#endif
