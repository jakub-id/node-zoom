#define BUILDING_NODE_EXTENSION
extern "C"{
	#include <yaz/zoom.h>
}
#include <node.h>
#include "connection.h"
#include "query.h"

using namespace v8;

bool query_failed(int g, char * query, Handle<Value> exception){
	
	char str[80];
	
	if (g == -1){
		sprintf(str, "Wrong query: %s", query);
		exception = ThrowException(String::New(str));
		return true;
	}
	
	return false;
}

QueryObject::QueryObject(){};
QueryObject::~QueryObject(){};

Persistent<Function> QueryObject::constructor;

void QueryObject::Init(){
	Local<FunctionTemplate> tpl = FunctionTemplate::New(New);
	tpl->SetClassName(String::NewSymbol("Query"));
	tpl->InstanceTemplate()->SetInternalFieldCount(1);

	// Prototype
	tpl->PrototypeTemplate()->Set(String::NewSymbol("destroy"), 
			FunctionTemplate::New(destroy)->GetFunction());
	
	tpl->PrototypeTemplate()->Set(String::NewSymbol("cql"), 
			FunctionTemplate::New(cql)->GetFunction());
	
	tpl->PrototypeTemplate()->Set(String::NewSymbol("cql2rpn"), 
			FunctionTemplate::New(cql2rpn)->GetFunction());
	
	tpl->PrototypeTemplate()->Set(String::NewSymbol("prefix"), 
			FunctionTemplate::New(prefix)->GetFunction());
	
	tpl->PrototypeTemplate()->Set(String::NewSymbol("sortby"), 
			FunctionTemplate::New(sortby)->GetFunction());
	
	constructor = Persistent<Function>::New(tpl->GetFunction());
}

Handle<Value> QueryObject::New(const Arguments& args){
	HandleScope scope;
	QueryObject * query = new QueryObject();
	query->Wrap(args.This());
	return args.This();
}

Handle<Value> QueryObject::NewInstance(const Arguments& args){
	HandleScope scope;
	
	Local<Object> instance = constructor->NewInstance();

	QueryObject * obj = node::ObjectWrap::Unwrap<QueryObject>(instance);
	
	obj->q = ZOOM_query_create();

	return scope.Close(instance);
}

Handle<Value> QueryObject::NewInstance(){
	HandleScope scope;
	
	Local<Object> instance = constructor->NewInstance();

	return scope.Close(instance);
}

Handle<Value> QueryObject::destroy(const Arguments& args){
	HandleScope scope;
	QueryObject * obj = node::ObjectWrap::Unwrap<QueryObject>(args.This());
	ZOOM_query_destroy(obj->q);
	return args.This();
}

Handle<Value> QueryObject::cql(const Arguments& args){
	HandleScope scope;
	ZOOM_query query = ZOOM_query_create();
	Handle<Value> exception;
	String::Utf8Value str(args[0]);
	Local<Object> instance = constructor->NewInstance();
	QueryObject * obj = node::ObjectWrap::Unwrap<QueryObject>(instance);
	
	if(query_failed(ZOOM_query_cql(query, *str), *str, exception)){
		return exception;
	}
	
	obj->q = query;
	return scope.Close(instance);
}

Handle<Value> QueryObject::cql2rpn(const Arguments& args){
	HandleScope scope;
	ZOOM_query query = ZOOM_query_create();
	ConnectionObject * cobj = node::ObjectWrap::Unwrap<ConnectionObject>(args[1]->ToObject());
	Handle<Value> exception;
	String::Utf8Value str(args[0]);
	Local<Object> instance = constructor->NewInstance();
	QueryObject * obj = node::ObjectWrap::Unwrap<QueryObject>(instance);
	
	if(query_failed(ZOOM_query_cql2rpn(query, *str, cobj->conn), *str, exception)){
		return exception;
	}
	
	obj->q = query;
	return scope.Close(instance);
}

Handle<Value> QueryObject::prefix(const Arguments& args){
	HandleScope scope;
	ZOOM_query query = ZOOM_query_create();
	Handle<Value> exception;
	String::Utf8Value str(args[0]);
	Local<Object> instance = constructor->NewInstance();
	QueryObject * obj = node::ObjectWrap::Unwrap<QueryObject>(instance);
	
	if(query_failed(ZOOM_query_prefix(query, *str), *str, exception)){
		return exception;
	}
	
	obj->q = query;
	return scope.Close(instance);
}

Handle<Value> QueryObject::sortby(const Arguments& args){
	HandleScope scope;
	ZOOM_query query = ZOOM_query_create();
	QueryObject * obj = new QueryObject();
	String::Utf8Value str(args[0]);
	
	ZOOM_query_sortby(query, *str); 
	
	obj->q = query;
	obj->Wrap(args.This());
	return args.This();
}
