#define BUILDING_NODE_EXTENSION 
extern "C"{
	#include <yaz/zoom.h>
}
#include <node.h>
#include "connection.h"
#include "scanset.h"
#include "query.h"
#include "resultset.h"
#include "record.h"

using namespace v8;

/** Connection **/

Handle<Value> CreateConnection(const Arguments& args) {
	HandleScope scope;
	return scope.Close(ConnectionObject::NewInstance(args));
}

void initAll(Handle<Object> target){

	ConnectionObject::Init();
	ScanSetObject::Init();
	QueryObject::Init();
	ResultSetObject::Init();
	RecordObject::Init();

	/** 建立 ObjectTemplate **/
	Handle<ObjectTemplate> Connection = ObjectTemplate::New();

	/** Connection **/
	Connection->Set(String::New("create"), FunctionTemplate::New(CreateConnection)->GetFunction());

	/** 加入 Object **/
	target->Set(String::NewSymbol("connection"), Connection->NewInstance());
	target->Set(String::NewSymbol("query"), QueryObject::NewInstance());
}

NODE_MODULE(zoom, initAll);
