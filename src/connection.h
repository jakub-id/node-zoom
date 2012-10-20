#define BUILDING_NODE_EXTENSION
#ifndef CONNECTION_H
#define CONNECTION_H

extern "C"{
	#include <yaz/zoom.h>
}
#include <node.h>

class ConnectionObject : public node::ObjectWrap {
	public:
		static void Init();
		static v8::Handle<v8::Value> NewInstance(const v8::Arguments& args);
		static v8::Handle<v8::Value> create(const v8::Arguments& args);
		static v8::Handle<v8::Value> option(const v8::Arguments& args);
		static v8::Handle<v8::Value> search(const v8::Arguments& args);
		static v8::Handle<v8::Value> connect(const v8::Arguments& args);
		static v8::Handle<v8::Value> scan(const v8::Arguments& args);
		static v8::Handle<v8::Value> close(const v8::Arguments& args);
		ZOOM_connection conn;

	private:
		static v8::Handle<v8::Value> New(const v8::Arguments& args);
		static v8::Persistent<v8::Function> constructor;
		ConnectionObject();
		~ConnectionObject();

};

#endif
