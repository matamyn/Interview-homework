//#include "RingBuffer.h"
//
//RingBuffer::RingBuffer(quint32 a_size, QObject *parent)
//    : QObject(parent), size_(a_size) {
//  mask_ = a_size - 1;
//  ptr_buf_ = (float*)malloc(sizeof(float) * size_);
//  clean();
//}
//
//RingBuffer::~RingBuffer() { free(ptr_buf_); }
//
//void RingBuffer::clean() {
//  idxRead_ = 0;
//  idxWrite_ = 0;
//  dataCount_ = 0;
//}
//
//bool RingBuffer::write(float *data, quint32 len) {
//  if (availableWrite() < len)
//    return false;
//
//  dataCount += len;
//
//  for (int i = 0; i < len; i++) {
//    fbuf[idxWrite++] = data[i];
//    idxWrite &= mask;
//  }
//
//  return true;
//}
//
//bool RingBuffer::read(float *data, quint32 len) {
//  if (availableRead() < len)
//    return false;
//
//  dataCount -= len;
//
//  for (int i = 0; i < len; i++) {
//    data[i] = fbuf[idxRead++];
//    idxRead &= mask;
//  }
//
//  return true;
//}
//
//float RingBuffer::getSample() {
//  dataCount--;
//  float sample = fbuf[idxRead++];
//  idxRead &= mask;
//  return sample;
//}
//
//void RingBuffer::addSample(float sample) {
//  dataCount++;
//  fbuf[idxWrite++] = sample;
//  idxWrite &= mask;
//}
//
//quint32 RingBuffer::availableRead() { return dataCount; }
//
//quint32 RingBuffer::availableWrite() { return size - dataCount; }