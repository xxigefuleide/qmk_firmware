// Copyright (c) 2017 Filipe Calasans
// SPDX-License-Identifier: GPL-3.0-or-later
#include "ringbuffer.h"
#include <string.h>

int ringBufferInit(RingBuffer *buffer, uint8_t *data, size_t len) {
   if(!isMultipleTwo(len)) {
      return 0;
   }

   buffer->tail = 0;
   buffer->head = 0;
   buffer->sizeMask = len-1;
   buffer->data = data;
   return 1;
}

size_t ringBufferLen(const RingBuffer *buffer) {
   if(buffer->tail >= buffer->head) {
      return buffer->tail-buffer->head;
   }

   return buffer->sizeMask-(buffer->head-buffer->tail)+1;
}

uint8_t ringBufferEmpty(const RingBuffer *buffer) {
   return (buffer->tail == buffer->head);
}

size_t ringBufferFreeSpace(const RingBuffer *buffer){
   return buffer->sizeMask - ringBufferLen(buffer);
}

size_t ringBufferMaxSize(const RingBuffer *buffer) {
   return buffer->sizeMask;
}

void ringBufferAppendOne(RingBuffer *buffer, uint8_t data){
   buffer->data[buffer->tail] = data;
   buffer->tail = (buffer->tail + 1) & buffer->sizeMask;
}

void ringBufferAppendMultiple(RingBuffer *buffer, const uint8_t *data, size_t len){
   if(buffer->tail + len > buffer->sizeMask) {
      uint32_t lenToTheEnd = buffer->sizeMask - buffer->tail + 1;
      uint32_t lenFromBegin = len - lenToTheEnd;
      memcpy(buffer->data + buffer->tail, data, lenToTheEnd);
      memcpy(buffer->data, data + lenToTheEnd, lenFromBegin);
   }
   else {
      memcpy(buffer->data + buffer->tail, data, len);
   }
   buffer->tail = (buffer->tail + len) & buffer->sizeMask;
}

uint8_t ringBufferPeekOne(const RingBuffer *buffer){
   return buffer->data[buffer->head];
}

uint8_t ringBufferGetOne(RingBuffer *buffer){
   uint8_t data =  buffer->data[buffer->head];
   buffer->head = (buffer->head + 1) & buffer->sizeMask;
   return data;
}

void ringBufferGetMultiple(RingBuffer *buffer, uint8_t *dst, size_t len) {
   ringBufferPeekMultiple(buffer, dst, len);
   buffer->head = (buffer->head + len) & buffer->sizeMask;
}

void ringBufferPeekMultiple(const RingBuffer *buffer, uint8_t *dst, size_t len){
   if(buffer->head + len > buffer->sizeMask) {
      uint32_t lenToTheEnd = buffer->sizeMask - buffer->head + 1;
      uint32_t lenFromBegin = len - lenToTheEnd;
      memcpy(dst, buffer->data + buffer->head, lenToTheEnd);
      memcpy(dst + lenToTheEnd, buffer->data, lenFromBegin);
   }
   else {
      memcpy(dst, buffer->data + buffer->head, len);
   }
}

void ringBufferDiscardMultiple(RingBuffer *buffer, size_t len){
   buffer->head = (buffer->head + len) & buffer->sizeMask;
}

void ringBufferClear(RingBuffer *buffer){
   buffer->head = buffer->tail = 0;
}
