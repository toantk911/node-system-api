#include "module.h"
#include "idle.h"

using namespace v8;

NAN_METHOD(IdleTime::GetIdleTime) {
  Nan::HandleScope scope;

  uint32_t idle;
  idle = SystemIdleTime();
  info.GetReturnValue().Set(idle);
}

NAN_METHOD(IdleTime::GetActiveWindow) {
  Nan::HandleScope scope;

  char* title;
  title = SystemActiveWindow();
  info.GetReturnValue().Set(Nan::New<String>(title, strlen(title)).ToLocalChecked());
}

void IdleTime::Init(Handle<Object> exports) {
  Nan::SetMethod(exports, "getIdleTime", IdleTime::GetIdleTime);
  Nan::SetMethod(exports, "getActiveWindow", IdleTime::GetActiveWindow);
}

NODE_MODULE(system_api, IdleTime::Init)
