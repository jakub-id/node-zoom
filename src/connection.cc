#define BUILDING_NODE_EXTENSION
extern "C"{
	#include <yaz/zoom.h>
}
#include <node.h>
#include "scanset.h"
#include "query.h"
#include "resultset.h"
#include "connection.h"

using namespace v8;

bool connection_failed(ZOOM_connection connection, Handle<Value> exception){
	int error;
	const char *errmsg, *addinfo;
	char str[80];

	error = ZOOM_connection_error(connection, &errmsg, &addinfo);

	if (error != 0){
		sprintf(str, "%s (%d) %s", errmsg, error, addinfo);
		exception = ThrowException(String::New(str));
		return true;
	}
	
	return false;
}

ConnectionObject::ConnectionObject(){};
ConnectionObject::~ConnectionObject(){};

Persistent<Function> ConnectionObject::constructor;

void ConnectionObject::Init(){
	Local<FunctionTemplate> tpl = FunctionTemplate::New(New);
	tpl->SetClassName(String::NewSymbol("Connection"));
	tpl->InstanceTemplate()->SetInternalFieldCount(1);

	// Prototype
	tpl->PrototypeTemplate()->Set(String::NewSymbol("option"), 
			FunctionTemplate::New(option)->GetFunction());
	
	tpl->PrototypeTemplate()->Set(String::NewSymbol("search"), 
			FunctionTemplate::New(search)->GetFunction());
	
	tpl->PrototypeTemplate()->Set(String::NewSymbol("connect"), 
			FunctionTemplate::New(connect)->GetFunction());
	
	tpl->PrototypeTemplate()->Set(String::NewSymbol("scan"), 
			FunctionTemplate::New(scan)->GetFunction());
	
	tpl->PrototypeTemplate()->Set(String::NewSymbol("close"), 
			FunctionTemplate::New(close)->GetFunction());
	
	constructor = Persistent<Function>::New(tpl->GetFunction());
}

Handle<Value> ConnectionObject::New(const Arguments& args){
	HandleScope scope;
	ConnectionObject* co = new ConnectionObject();
	co->Wrap(args.This());
	return args.This();
}

Handle<Value> ConnectionObject::NewInstance(const Arguments& args){
	HandleScope scope;

	const unsigned argc = 1;
	Handle<Value> argv[argc] = { args[0] };
	Local<Object> instance = constructor->NewInstance(argc, argv);
	
	ConnectionObject * obj = node::ObjectWrap::Unwrap<ConnectionObject>(instance);

	ZOOM_options o = ZOOM_options_create();
	obj->conn = ZOOM_connection_create(o);

	if(args[0]->IsString() && args[1]->IsNumber()){
		obj->connect(args);
	}

	return scope.Close(instance);
}

Handle<Value> ConnectionObject::search(const Arguments& args){
	HandleScope scope;
	
	ConnectionObject * obj = node::ObjectWrap::Unwrap<ConnectionObject>(args.This());
	QueryObject * query = node::ObjectWrap::Unwrap<QueryObject>(args[0]->ToObject());
	ZOOM_resultset rs = ZOOM_connection_search(obj->conn, query->q);
	Handle<Value> exception;

	if(connection_failed(obj->conn, exception)){
		return exception;
	}

	return scope.Close(ResultSetObject::NewInstance(rs));
}

Handle<Value> ConnectionObject::connect(const Arguments& args){
	HandleScope scope;
	
	String::Utf8Value str(args[0]);
	const char * hostname = *str;
	int port = args[1]->ToNumber()->Value();
	Handle<Value> exception;
	
	ConnectionObject * obj = node::ObjectWrap::Unwrap<ConnectionObject>(args.This());

	ZOOM_connection_connect(obj->conn, hostname, port);

	if(connection_failed(obj->conn, exception)){
		return exception;
	}

	return args.This();
}

Handle<Value> ConnectionObject::option(const Arguments& args){
	HandleScope scope;
	
	ConnectionObject * obj = node::ObjectWrap::Unwrap<ConnectionObject>(args.This());
	
	String::Utf8Value key(args[0]);
	
	switch(args.Length()){
		case 1: 
			return scope.Close(String::New(ZOOM_connection_option_get(obj->conn, *key)));
			break;
		case 2:
			String::Utf8Value val2(args[1]);
			ZOOM_connection_option_set(obj->conn, *key, *val2);
			return scope.Close(Boolean::New(true));
			break;
	}
	
	return scope.Close(Boolean::New(false));
}

Handle<Value> ConnectionObject::scan(const Arguments& args){
	HandleScope scope;
	
	String::Utf8Value startterm(args[0]);
	const ConnectionObject * obj = node::ObjectWrap::Unwrap<ConnectionObject>(args.This());
	
	return scope.Close(ScanSetObject::NewInstance(obj->conn, *startterm));
}

Handle<Value> ConnectionObject::close(const Arguments& args){
	const ConnectionObject * obj = node::ObjectWrap::Unwrap<ConnectionObject>(args.This());
	ZOOM_connection_destroy(obj->conn);
	return args.This();
}
