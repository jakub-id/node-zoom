#define BUILDING_NODE_EXTENSION 
extern "C"{
	#include <yaz/zoom.h>
}
#include <node.h>
#include "connection.h"
#include "scanset.h"

using namespace v8;

/** Connection **/

Handle<Value> CreateConnection(const Arguments& args) {
	HandleScope scope;
	return scope.Close(ConnectionObject::NewInstance(args));
}

void initAll(Handle<Object> target){

	ConnectionObject::Init();
	ScanSetObject::Init();

	/** 建立 ObjectTemplate **/
	Handle<ObjectTemplate> Connection = ObjectTemplate::New();

	/** Connection **/
	Connection->Set(String::New("create"), FunctionTemplate::New(CreateConnection)->GetFunction());

	/** 加入 Object **/
	target->Set(String::NewSymbol("Connection"), Connection->NewInstance());
}

NODE_MODULE(zoom, initAll);
