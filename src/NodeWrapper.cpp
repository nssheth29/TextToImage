//
//  NodeWrapper.cpp
//  TextToImage
//
//  Created by Nirav Sheth on 1/13/16.
//
//

#include <stdio.h>
#include "DrawText.cpp"
#include <node.h>
#include <v8.h>
#include "nan.h"
#include <node_buffer.h>


using v8::FunctionTemplate;
using v8::Handle;
using v8::Object;
using v8::String;
using v8::Value;
using v8::Local;
using v8::Persistent;
using v8::Function;

using Nan::AsyncQueueWorker;
using Nan::AsyncWorker;
using Nan::Callback;
using Nan::HandleScope;
using Nan::Utf8String;
using Nan::Has;
using Nan::Get;
using Nan::Set;
using Nan::To;
using Nan::New;
using Nan::NewBuffer;
using Nan::Null;
using Nan::Equals;
using Nan::ThrowError;

struct Options {
  std::string text;
  
  std::string font;
  int fontsize;
  bool italic;
  bool bold;
  
  int r;
  int g;
  int b;
  double a;
  
  bool shadow;
  int shadowOffsetX;
  int shadowOffsetY;
  int shadowR;
  int shadowG;
  int shadowB;
  double shadowA;
  
};

void FreeCallback(char* data, void* hint) {
  if (data != nullptr) {
    free(data);
  }
}

class Worker : public AsyncWorker {
public:
  Worker(Callback *callback, Options *options) : AsyncWorker(callback), options(options), buffer(nullptr) {}
  ~Worker() {}
  
  void Execute() {
    FontOptions *fo = new FontOptions;
    fo->fontFamily = options->font.c_str();
    fo->bold = options->bold;
    fo->italic = options->italic;
    fo->size = options->fontsize;
    
    ColorOptions *co = new ColorOptions;
    co->r = options->r;
    co->g = options->g;
    co->b = options->b;
    co->a = options->a;
    
    ShadowOptions *so = nullptr;
    if (options->shadow) {
      so = new ShadowOptions;
      so->offsetX = options->shadowOffsetX;
      so->offsetY = options->shadowOffsetY;
      ColorOptions *sco = new ColorOptions;
      sco->r = options->shadowR;
      sco->g = options->shadowG;
      sco->b = options->shadowB;
      sco->a = options->shadowA;
      so->colorOptions = sco;
    }
    buffer = new OutputBuffer;
    ConvertTextToImage(buffer, options->text.c_str(), fo, co, so);
  }
  
  void HandleOKCallback() {
    HandleScope();
    Local<Value> args[] = {
      Null(),
      getBuffer(buffer)
    };
    callback->Call(2, args);
  }
  
private:
  Options *options;
  OutputBuffer *buffer;
  
  Local<Object> getBuffer(OutputBuffer *b) {
    return  NewBuffer(static_cast<char*>(buffer->data), buffer->length, FreeCallback, nullptr).ToLocalChecked();
  }
};


NAN_METHOD(Apply) {
  HandleScope();
  if (!info[0]->IsObject()) {
    return ThrowError("Invalid Argument, must be an object");
  }
  Local<Object> options = info[0]->ToObject();
  Options *o = new Options;
  o->text = *Utf8String(Get(options, New("text").ToLocalChecked()).ToLocalChecked());
  o->font = *Utf8String(Get(options, New("font").ToLocalChecked()).ToLocalChecked());
  o->fontsize = To<int>(Get(options, New("fontsize").ToLocalChecked()).ToLocalChecked()).FromJust();
  o->bold = To<bool>(Get(options, New("bold").ToLocalChecked()).ToLocalChecked()).FromJust();
  o->italic = To<bool>(Get(options, New("italic").ToLocalChecked()).ToLocalChecked()).FromJust();
  
  o->r = To<int>(Get(options, New("r").ToLocalChecked()).ToLocalChecked()).FromJust();
  o->g = To<int>(Get(options, New("g").ToLocalChecked()).ToLocalChecked()).FromJust();
  o->b = To<int>(Get(options, New("b").ToLocalChecked()).ToLocalChecked()).FromJust();
  o->a = To<double>(Get(options, New("a").ToLocalChecked()).ToLocalChecked()).FromJust();
  
  o->shadow = To<bool>(Get(options, New("shadow").ToLocalChecked()).ToLocalChecked()).FromJust();
  o->shadowOffsetX = To<int>(Get(options, New("shadowOffsetX").ToLocalChecked()).ToLocalChecked()).FromJust();
  o->shadowOffsetY = To<int>(Get(options, New("shadowOffsetY").ToLocalChecked()).ToLocalChecked()).FromJust();
  o->shadowR = To<int>(Get(options, New("shadowR").ToLocalChecked()).ToLocalChecked()).FromJust();
  o->shadowG = To<int>(Get(options, New("shadowG").ToLocalChecked()).ToLocalChecked()).FromJust();
  o->shadowB = To<int>(Get(options, New("shadowB").ToLocalChecked()).ToLocalChecked()).FromJust();
  o->shadowA = To<double>(Get(options, New("shadowA").ToLocalChecked()).ToLocalChecked()).FromJust();

  if (!info[1]->IsFunction()) {
    return ThrowError("2nd argument must be a callback function");
  }
  Callback *callback = new Callback(info[1].As<Function>());
  AsyncQueueWorker(new Worker(callback, o));
}

NAN_MODULE_INIT(init) {
  Nan::Set(target, Nan::New("apply").ToLocalChecked(),
           Nan::GetFunction(Nan::New<v8::FunctionTemplate>(Apply)).ToLocalChecked());

}

NODE_MODULE(addon, init)