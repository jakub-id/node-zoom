#define BUILDING_NODE_EXTENSION
extern "C"{
	#include <yaz/zoom.h>
}
#include <node.h>
#include "connection.h"
#include "record.h"
#include "resultset.h"

using namespace v8;

ResultSetObject::ResultSetObject(){};
ResultSetObject::~ResultSetObject(){};

Persistent<Function> ResultSetObject::constructor;

void ResultSetObject::Init(){
	Local<FunctionTemplate> tpl = FunctionTemplate::New(New);
	tpl->SetClassName(String::NewSymbol("ResultSet"));
	tpl->InstanceTemplate()->SetInternalFieldCount(1);

	// Prototype
	tpl->PrototypeTemplate()->Set(String::NewSymbol("destroy"), 
			FunctionTemplate::New(destroy)->GetFunction());
	
	tpl->PrototypeTemplate()->Set(String::NewSymbol("release"), 
			FunctionTemplate::New(destroy)->GetFunction());
	
	tpl->PrototypeTemplate()->Set(String::NewSymbol("option"), 
			FunctionTemplate::New(option)->GetFunction());
	
	tpl->PrototypeTemplate()->Set(String::NewSymbol("size"), 
			FunctionTemplate::New(size)->GetFunction());
	
	tpl->PrototypeTemplate()->Set(String::NewSymbol("records"), 
			FunctionTemplate::New(records)->GetFunction());
	
	tpl->PrototypeTemplate()->Set(String::NewSymbol("record"), 
			FunctionTemplate::New(record)->GetFunction());
	
	constructor = Persistent<Function>::New(tpl->GetFunction());
}

Handle<Value> ResultSetObject::New(const Arguments& args){
	HandleScope scope;
	ResultSetObject * rs = new ResultSetObject();
	rs->Wrap(args.This());
	return args.This();
}

Handle<Value> ResultSetObject::NewInstance(ZOOM_resultset result){
	HandleScope scope;
	
	Local<Object> instance = constructor->NewInstance();

	ResultSetObject * obj = node::ObjectWrap::Unwrap<ResultSetObject>(instance);
	
	obj->rs = result;
	
	return scope.Close(instance);
}

Handle<Value> ResultSetObject::destroy(const Arguments& args){
	HandleScope scope;
	ResultSetObject * rs = new ResultSetObject();
	ZOOM_resultset_destroy(rs->rs);
	return args.This();
}

Handle<Value> ResultSetObject::option(const Arguments& args){
	HandleScope scope;
	ResultSetObject * obj = new ResultSetObject();
	
	String::Utf8Value key(args[0]);
	
	switch(args.Length()){
		case 1:
			return scope.Close(String::New(ZOOM_resultset_option_get(obj->rs, *key)));
			break;
		case 2:
			String::Utf8Value val2(args[1]);
			ZOOM_resultset_option_set(obj->rs, *key, *val2);
			return scope.Close(Boolean::New(true));
			break;
	}
	
	return scope.Close(Boolean::New(false));
}

Handle<Value> ResultSetObject::record(const Arguments& args){
	HandleScope scope;
	ResultSetObject * obj = node::ObjectWrap::Unwrap<ResultSetObject>(args.This());
	int index = args[0]->ToNumber()->Value();
	return RecordObject::NewInstance(obj, index);
}

Handle<Value> ResultSetObject::records(const Arguments& args){
	HandleScope scope;
	ResultSetObject * query = new ResultSetObject();
	query->Wrap(args.This());
	return args.This();
}

Handle<Value> ResultSetObject::size(const Arguments& args){
	HandleScope scope;
	ResultSetObject * obj = node::ObjectWrap::Unwrap<ResultSetObject>(args.This());
	return scope.Close(Number::New(ZOOM_resultset_size(obj->rs)));
}
