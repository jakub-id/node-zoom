#define BUILDING_NODE_EXTENSION
#ifndef SCANSET_H
#define SCANSET_H

extern "C"{
	#include <yaz/zoom.h>
}
#include <node.h>

class ScanSetObject : public node::ObjectWrap {
	public:
		static void Init();
		static v8::Handle<v8::Value> NewInstance(ZOOM_connection conn, const char * startterm);
		static v8::Handle<v8::Value> size(const v8::Arguments& args);
		static v8::Handle<v8::Value> term(const v8::Arguments& args);

	private:
		static v8::Handle<v8::Value> New(const v8::Arguments& args);
		static v8::Persistent<v8::Function> constructor;
		ZOOM_scanset scan;
		ScanSetObject();
		~ScanSetObject();

};

#endif
