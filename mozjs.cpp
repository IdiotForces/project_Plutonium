
#include <jsapi.h>
#include <cstdio>
#include <string>
#include <iostream>

static JSClass global_class = {
	"global", JSCLASS_GLOBAL_FLAGS,
	JS_PropertyStub, JS_DeletePropertyStub,
	JS_PropertyStub, JS_StrictPropertyStub,
	JS_EnumerateStub, JS_ResolveStub, JS_ConvertStub,
	nullptr, nullptr, nullptr, nullptr,
	JS_GlobalObjectTraceHook
};

class vx_test {
	public:

		vx_test(int t) : test(t) {
			printf("VXX: vx_test constructing ... %d %lx\n", t, this); }

		~vx_test() { printf("VXX: vx_test descructing ...\n"); }

		vx_test(const vx_test& o) {
			this->test_readonly = o.test_readonly;
			this->objref = o.objref;
			this->test = o.test;
			printf("vx_test copying ...\n");
		}

		int test_readonly = 2;

		vx_test *objref;

		int test = 3;

		int test_func(int a) {
			// printf("VXX: vx_test::test_func: %d %d\n", this->test, a);
			return a; }

		static int test_static_func(int a) {
			printf("VXX: vx_test::test_static_func.\n");
			return a;
		}
};

static JSClass *vx_test_sclass;
static JSObject *vx_test_sobj = nullptr;

bool getter_test(JSContext *context, unsigned int argc, JS::Value *vp) {
	
	JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
	vx_test *raw = (vx_test *)JS_GetPrivate(JS_THIS_OBJECT(context, vp));
	args.rval().setNumber((uint32_t) raw->test);

	return true;
}

bool setter_test(JSContext *context, unsigned int argc, JS::Value *vp) {
	
	JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
	double arg = args[0].get().toNumber();
	vx_test *raw = (vx_test *)JS_GetPrivate(JS_THIS_OBJECT(context, vp));
	raw->test = arg;

	return true;
}

bool getter_objref(JSContext *context, unsigned int argc, JS::Value *vp) {
	
	JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
	vx_test *raw = (vx_test *)JS_GetPrivate(JS_THIS_OBJECT(context, vp));
	JS::RootedObject proto(context, vx_test_sobj);

	JSObject *jsobj = JS_NewObject(context, vx_test_sclass, proto, JS::NullPtr());
	JS_SetPrivate(jsobj, (void *) (raw->objref));
	args.rval().set(OBJECT_TO_JSVAL(jsobj));

	return true;
}

bool setter_objref(JSContext *context, unsigned int argc, JS::Value *vp) {
	
	JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
	vx_test *raw = (vx_test *)JS_GetPrivate(JS_THIS_OBJECT(context, vp));
	vx_test *val = (vx_test *)JS_GetPrivate(&args[0].get().toObject());
	raw->objref = val;

	return true;
}

bool method_test_func(JSContext *context, unsigned int argc, JS::Value *vp) {

	JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
	double arg = args[0].get().toNumber();
	vx_test *raw = (vx_test *)JS_GetPrivate(JS_THIS_OBJECT(context, vp));
	int ret = raw->test_func((int)arg);
	args.rval().setNumber((uint32_t)ret);

	return true;
}

static JSPropertySpec properties[] = {
	// JS_PSGS( "test", getter_test, setter_test, JSPROP_PERMANENT ),
	// JS_PSGS( "objref", getter_objref, setter_objref, JSPROP_PERMANENT ),
	JS_PS_END
};

static JSFunctionSpec funcs[] = {
	// JS_FN("test_func", method_test_func, 1, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_FS_END };

static JSFunctionSpec static_funcs[] = {
	JS_FS_END };

bool vx_test_ctor(JSContext *context, unsigned int argc, JS::Value *vp) {

	JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
	if (args.length() == 1) {
		JS::RootedObject proto(context, vx_test_sobj);
		JSObject *jsobj = JS_NewObject(context, vx_test_sclass, proto, JS::NullPtr());
		vx_test *raw = new vx_test((int) args[0].get().toNumber());	
		JS_SetPrivate(jsobj, (void *) raw);
		args.rval().set(OBJECT_TO_JSVAL(jsobj));
	}

	return true;
}

void init_vx_test_class(JSContext *context, JS::HandleObject global) {

	vx_test_sclass = (JSClass *)malloc(sizeof(JSClass));
	vx_test_sclass->name = "vx_test";
	vx_test_sclass->addProperty = JS_PropertyStub;
    vx_test_sclass->delProperty = JS_DeletePropertyStub;
    vx_test_sclass->getProperty = JS_PropertyStub;
    vx_test_sclass->setProperty = JS_StrictPropertyStub;
    vx_test_sclass->enumerate = JS_EnumerateStub;
    vx_test_sclass->resolve = JS_ResolveStub;
    vx_test_sclass->convert = JS_ConvertStub;
    vx_test_sclass->flags = JSCLASS_HAS_PRIVATE;

    vx_test_sobj = JS_InitClass(context, global, JS::NullPtr(), vx_test_sclass,
    					vx_test_ctor, 0, properties, funcs, nullptr, static_funcs);

    JS::RootedObject proto(context, vx_test_sobj);
    JS_DefineProperty(context, proto, "test", JS::UndefinedHandleValue, JSPROP_ENUMERATE | JSPROP_PERMANENT | JSPROP_SHARED | JSPROP_NATIVE_ACCESSORS,
                         (JSPropertyOp) getter_test, (JSStrictPropertyOp) setter_test);
    JS_DefineProperty(context, proto, "objref", JS::UndefinedHandleValue, JSPROP_ENUMERATE | JSPROP_PERMANENT | JSPROP_SHARED | JSPROP_NATIVE_ACCESSORS,
                         (JSPropertyOp) getter_objref, (JSStrictPropertyOp) setter_objref);

    JS_DefineFunction(context, proto, "test_func", method_test_func, 1, JSPROP_PERMANENT | JSPROP_ENUMERATE | JSFUN_STUB_GSOPS);

}

bool print(JSContext *context, unsigned int argc, JS::Value *vp) {

	JS::CallArgs args = JS::CallArgsFromVp(argc, vp);

	for (size_t i = 0; i < args.length(); i++)
		printf("%s", JS_EncodeString(context, JS::ToString(context, args[i])));
	printf("\n");
	args.rval().setUndefined();

	return true;
}

static JSFunctionSpec js_global_funcs[] = {
	JS_FS("print", print, 0, 0),
};

void report_exception(JSContext *context, const char *msg, JSErrorReport *rep) {
	fprintf(stderr, "%s:%u:%s\n", rep->filename ? rep->filename : "[anonymous]", 
		(unsigned int) rep->lineno, msg); }

std::string readfile(const std::string& filename) {
	printf("::readfile - Reading %s ...\n", filename.c_str());
	FILE *fp = fopen(filename.c_str(), "r");

	fseek(fp, 0, SEEK_END);
	size_t fsize = ftell(fp);
	rewind(fp);

	char *ret = new char[fsize+1];
	fread(ret, fsize, 1, fp);
	ret[fsize] = '\0';
	std::string str_ret(ret);
	delete ret;
	fclose(fp);

	return str_ret;
}

int main(int argc, const char *argv[]) {

	if (!JS_Init()) return 1;

	JSRuntime *js_rt = JS_NewRuntime(32L * 1024L * 1024L,
			JSUseHelperThreads::JS_USE_HELPER_THREADS);
	if (!js_rt) return 1;

	JSContext *context = JS_NewContext(js_rt, 8192 * 2);
	if (!context) return 1;
	JS_SetErrorReporter(context, report_exception);

{
	JSAutoRequest at_req(context);

	JS::RootedObject global(context);
	global = JS_NewGlobalObject(context, &global_class, nullptr, JS::DontFireOnNewGlobalHook);
	if (!global) return 1;

	JSAutoCompartment at_comp(context, global);

	if (!JS_InitStandardClasses(context, global)) return 1;

	if (!JS_DefineFunctions(context, global, js_global_funcs))
		return 1;

	init_vx_test_class(context, global);

	std::string source = readfile(argv[1]);
	JS::RootedValue ret(context);
	bool ok = JS_EvaluateScript(context, global, source.c_str(), source.length(), argv[1], 0, &ret);
	if (!ok) return 1;
	printf("Return from %s: %s\n", argv[1], JS_EncodeString(context, JS::ToString(context, ret)));
}

	JS_DestroyContext(context);
	JS_DestroyRuntime(js_rt);
	JS_ShutDown();

	return 0;
}