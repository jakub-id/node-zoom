#define BUILDING_NODE_EXTENSION
#ifndef QUERY_H
#define QUERY_H

extern "C"{
	#include <yaz/zoom.h>
}
#include <node.h>

class QueryObject : public node::ObjectWrap {
	public:
		static void Init();
		static v8::Handle<v8::Value> NewInstance(const v8::Arguments& args);
		static v8::Handle<v8::Value> NewInstance();
		static v8::Handle<v8::Value> destroy(const v8::Arguments& args);
		static v8::Handle<v8::Value> cql(const v8::Arguments& args);
		static v8::Handle<v8::Value> cql2rpn(const v8::Arguments& args);
		static v8::Handle<v8::Value> prefix(const v8::Arguments& args);
		static v8::Handle<v8::Value> sortby(const v8::Arguments& args);
		ZOOM_query q;

	private:
		static v8::Handle<v8::Value> New(const v8::Arguments& args);
		static v8::Persistent<v8::Function> constructor;
		QueryObject();
		~QueryObject();

};

#endif
