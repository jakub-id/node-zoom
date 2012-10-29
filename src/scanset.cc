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

Handle<Value> ScanSetObject::term(const Arguments& args){
	HandleScope scope;
	
	ScanSetObject * obj = node::ObjectWrap::Unwrap<ScanSetObject>(args.This());
	
	const char * value_term;
	size_t pos, *occ = 0, *len = 0;
	
	pos = args[0]->ToNumber()->Value();
	
	value_term = ZOOM_scanset_term(obj->scan, pos, occ, len);
	return scope.Close(String::New(value_term));
}

Handle<Value> ScanSetObject::displayTerm(const Arguments& args){
	HandleScope scope;
	
	ScanSetObject * obj = node::ObjectWrap::Unwrap<ScanSetObject>(args.This());
	
	const char * display_term;
	size_t pos, *occ = 0, *len = 0;
	
	pos = args[0]->ToNumber()->Value();
	
	display_term = ZOOM_scanset_display_term(obj->scan, pos, occ, len);
	
	return scope.Close(String::New(display_term));
}

Handle<Value> ScanSetObject::size(const Arguments& args){
	HandleScope scope;
	ScanSetObject * obj = node::ObjectWrap::Unwrap<ScanSetObject>(args.This());
	return scope.Close(Number::New(ZOOM_scanset_size(obj->scan)));
}

Handle<Value> ScanSetObject::distory(const Arguments& args){
	HandleScope scope;
	ScanSetObject * obj = node::ObjectWrap::Unwrap<ScanSetObject>(args.This());
	ZOOM_scanset_destroy(obj->scan);
	return args.This();
}

Handle<Value> ScanSetObject::option(const Arguments& args){
	HandleScope scope;
	
	ScanSetObject * obj = node::ObjectWrap::Unwrap<ScanSetObject>(args.This());
	
	String::Utf8Value key(args[0]);
	
	switch(args.Length()){
		case 1: 
			return scope.Close(String::New(ZOOM_scanset_option_get(obj->scan, *key)));
			break;
		case 2:
			String::Utf8Value val2(args[1]);
			ZOOM_scanset_option_set(obj->scan, *key, *val2);
			return scope.Close(Boolean::New(true));
			break;
	}
	
	return scope.Close(Boolean::New(false));
}
