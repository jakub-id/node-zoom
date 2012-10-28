#define BUILDING_NODE_EXTENSION
#ifndef RESULTSET_H
#define RESULTSET_H

extern "C"{
	#include <yaz/zoom.h>
}
#include <node.h>

class ResultSetObject : public node::ObjectWrap {
	public:
		static void Init();
		static v8::Handle<v8::Value> NewInstance(ZOOM_resultset result);
		static v8::Handle<v8::Value> destroy(const v8::Arguments& args);
		static v8::Handle<v8::Value> option(const v8::Arguments& args);
		static v8::Handle<v8::Value> record(const v8::Arguments& args);
		static v8::Handle<v8::Value> records(const v8::Arguments& args);
		static v8::Handle<v8::Value> size(const v8::Arguments& args);
		ZOOM_resultset rs;

	private:
		static v8::Handle<v8::Value> New(const v8::Arguments& args);
		static v8::Persistent<v8::Function> constructor;
		ResultSetObject();
		~ResultSetObject();

};

#endif

