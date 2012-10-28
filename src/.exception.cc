#define BUILDING_NODE_EXTENSION
extern "C"{
	#include <yaz/zoom.h>
}
#include <node.h>
#include "exception.h"

using namespace v8;

// Exception

ExceptionObject::ExceptionObject(int errcode){
	code = errcode;
}

ExceptionObject::~ExceptionObject(){};

Handle<Value> ExceptionObject::errcode(const v8::Arguments& args){
	HandleScope scope;
	
	ExceptionObject * obj = node::ObjectWrap::Unwrap<ExceptionObject>(args.This());

	return scope.Close(Number::New(obj->code));
}

Handle<Value> ExceptionObject::errmsg(const v8::Arguments& args){
	HandleScope scope;
	
	ExceptionObject * obj = node::ObjectWrap::Unwrap<ExceptionObject>(args.This());

	char buf[40];
	sprintf(buf, "error #%d", obj->code);

	return scope.Close(String::New(buf));
}

// SystemException

SystemExceptionObject::SystemExceptionObject() : ExceptionObject(errno){
	code = errno;
}

Handle<Value> SystemExceptionObject::errmsg(const v8::Arguments& args){
	HandleScope scope;
	#ifdef __GLIBC__
		ExceptionObject * obj = node::ObjectWrap::Unwrap<ExceptionObject>(args.This());
		char buf[1024];
		const char* result = strerror_r(obj->code, buf, sizeof(buf));
		if (result != 0)
			return scope.Close(String::New(result));
		return ExceptionObject::errmsg(args);
	#else
		return scope.Close(String::New(strerror(code)));
	#endif
}

// Bib1Exception

