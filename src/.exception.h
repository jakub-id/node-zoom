#define BUILDING_NODE_EXTENSION
#ifndef SCANSET_H
#define SCANSET_H
#include <iostream>
#include <errno.h>
#include <string.h>
#include <stdio.h>
extern "C"{
	#include <yaz/zoom.h>
}
#include <node.h>

class ExceptionObject : public node::ObjectWrap {
	public:
		static void Init();
		static v8::Handle<v8::Value> NewInstance();
		static v8::Handle<v8::Value> errcode(const v8::Arguments& args);
		static v8::Handle<v8::Value> errmsg(const v8::Arguments& args);
		ExceptionObject(int errcode);
		~ExceptionObject();
		int code;
	
	private:
		static v8::Handle<v8::Value> New(const v8::Arguments& args);
		static v8::Persistent<v8::Function> constructor;

};

class SystemExceptionObject : public ExceptionObject {
	public:
		static void Init();
		static v8::Handle<v8::Value> NewInstance();
		static v8::Handle<v8::Value> errcode(const v8::Arguments& args);
		static v8::Handle<v8::Value> errmsg(const v8::Arguments& args);
		SystemExceptionObject();
		SystemExceptionObject(int code);
		~SystemExceptionObject();

	private:
		static v8::Handle<v8::Value> New(const v8::Arguments& args);
		static v8::Persistent<v8::Function> constructor;

};

class Bib1ExceptionObject : public ExceptionObject {
	public:
		static void Init();
		static v8::Handle<v8::Value> NewInstance();
		static v8::Handle<v8::Value> errcode(const v8::Arguments& args);
		static v8::Handle<v8::Value> errmsg(const v8::Arguments& args);
		Bib1ExceptionObject(int code, const char * addinfo);
		~Bib1ExceptionObject();

	private:
		static v8::Handle<v8::Value> New(const v8::Arguments& args);
		static v8::Persistent<v8::Function> constructor;

};

#endif
