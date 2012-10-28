#define BUILDING_NODE_EXTENSION
extern "C"{
	#include <yaz/zoom.h>
}
#include <node.h>
#include <string>
#include "connection.h"
#include "record.h"
#include "resultset.h"

using namespace v8;

RecordObject::RecordObject(){};
RecordObject::~RecordObject(){};

Persistent<Function> RecordObject::constructor;

void RecordObject::Init(){
	Local<FunctionTemplate> tpl = FunctionTemplate::New(New);
	tpl->SetClassName(String::NewSymbol("ResultSet"));
	tpl->InstanceTemplate()->SetInternalFieldCount(1);

	// Prototype
	tpl->PrototypeTemplate()->Set(String::NewSymbol("render"), 
			FunctionTemplate::New(render)->GetFunction());
	
	tpl->PrototypeTemplate()->Set(String::NewSymbol("raw"), 
			FunctionTemplate::New(raw)->GetFunction());
	
	tpl->PrototypeTemplate()->Set(String::NewSymbol("get"), 
			FunctionTemplate::New(get)->GetFunction());
	
	constructor = Persistent<Function>::New(tpl->GetFunction());
}

Handle<Value> RecordObject::New(const Arguments& args){
	HandleScope scope;
	RecordObject * rs = new RecordObject();
	rs->Wrap(args.This());
	return args.This();
}

Handle<Value> RecordObject::NewInstance(ResultSetObject * res, int index){
	HandleScope scope;
	
	Local<Object> instance = constructor->NewInstance();

	RecordObject * obj = node::ObjectWrap::Unwrap<RecordObject>(instance);
	
	obj->r = ZOOM_resultset_record(res->rs, index);
	
	return scope.Close(instance);
}

Handle<Value> RecordObject::render(const Arguments& args){
	HandleScope scope;
	int len;
	RecordObject * obj = node::ObjectWrap::Unwrap<RecordObject>(args.This());
	const char* data = ZOOM_record_get(obj->r, "render", &len);
	return scope.Close(String::New(std::string(data, len).c_str()));
}

Handle<Value> RecordObject::raw(const Arguments& args){
	HandleScope scope;
	RecordObject * rs = new RecordObject();
	rs->Wrap(args.This());
	return args.This();
}

Handle<Value> RecordObject::get(const Arguments& args){
	HandleScope scope;
	RecordObject * rs = new RecordObject();
	rs->Wrap(args.This());
	return args.This();
}

