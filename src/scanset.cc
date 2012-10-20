#define BUILDING_NODE_EXTENSION
extern "C"{
	#include <yaz/zoom.h>
}
#include <node.h>
#include "scanset.h"

using namespace v8;

ScanSetObject::ScanSetObject(){};
ScanSetObject::~ScanSetObject(){};

Persistent<Function> ScanSetObject::constructor;

void ScanSetObject::Init(){
	Local<FunctionTemplate> tpl = FunctionTemplate::New(New);
	tpl->SetClassName(String::NewSymbol("ScanSet"));
	tpl->InstanceTemplate()->SetInternalFieldCount(1);

	// Prototype
	tpl->PrototypeTemplate()->Set(String::NewSymbol("size"), 
			FunctionTemplate::New(size)->GetFunction());
	
	tpl->PrototypeTemplate()->Set(String::NewSymbol("term"), 
			FunctionTemplate::New(term)->GetFunction());
	
	constructor = Persistent<Function>::New(tpl->GetFunction());
}

Handle<Value> ScanSetObject::New(const Arguments& args){
	HandleScope scope;
	ScanSetObject * ss = new ScanSetObject();
	ss->Wrap(args.This());
	return args.This();
}

Handle<Value> ScanSetObject::NewInstance(ZOOM_connection conn, const char * startterm){
	HandleScope scope;
	
	Local<Object> instance = constructor->NewInstance();

	ScanSetObject * obj = node::ObjectWrap::Unwrap<ScanSetObject>(instance);
	
	obj->scan = ZOOM_connection_scan(conn, startterm);
	
	return scope.Close(instance);
}

Handle<Value> ScanSetObject::size(const Arguments& args){
	HandleScope scope;
	return args.This();
}

Handle<Value> ScanSetObject::term(const Arguments& args){
	HandleScope scope;
	
	ScanSetObject * obj = node::ObjectWrap::Unwrap<ScanSetObject>(args.This());
	const char * value_term;

	if(args.Length() != 3){
		return scope.Close(Boolean::New(false));
	}
// 這裡有問題
	if(args[0]->IsNumber() && args[1]->IsNumber() && args[2]->IsNumber()){
		value_term = ZOOM_scanset_term(obj->scan, int(args[0]->ToNumber()->Value()), int(args[1]->ToNumber()->Value()), int(args[2]->ToNumber()->Value()));
		return scope.Close(value_term);
	}else{
		return scope.Close(Boolean::New(false));
	}
	
	return args.This();
}
