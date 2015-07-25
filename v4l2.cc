#include <iostream>
#include <fstream>
using namespace std;

#include <node.h>
#include <node_object_wrap.h>
using namespace v8;

#include "webcam.h"

Webcam* webcam;

void start(const FunctionCallbackInfo<Value>& args) {
  Isolate* isolate = Isolate::GetCurrent();
  if (args.Length() < 3) {
    isolate->ThrowException(Exception::TypeError(
      String::NewFromUtf8(isolate, "start(string device, Uint32 width, Uint32 height)"))
    );
    return;
  }

  if (!args[0]->IsString() || !args[1]->IsUint32() || !args[2]->IsUint32()) {
    isolate->ThrowException(Exception::TypeError(
      String::NewFromUtf8(isolate, "start(string device, int width, int height)")));
    return;
  }

  String::Utf8Value devstr_arg(args[0]->ToString());
  string devstr = string(*devstr_arg);

  webcam = new Webcam(devstr, args[1]->Uint32Value(), args[2]->Uint32Value());
}

uint8_t* data;
unsigned long size;

void frame(const FunctionCallbackInfo<Value>& args) {
  Isolate* isolate = Isolate::GetCurrent();

  auto frame = webcam->frame();

  auto array_buffer = ArrayBuffer::New(isolate, frame.data, frame.size);
//  auto uint8_array = Uint8Array::New(array_buffer, 0, frame.size);

  args.GetReturnValue().Set(array_buffer);
}

void init(Handle<Object> exports) {
  NODE_SET_METHOD(exports, "start", start);
  NODE_SET_METHOD(exports, "frame", frame);
}

NODE_MODULE(v4l2, init)
