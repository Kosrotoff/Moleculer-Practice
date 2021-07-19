#include "FibonacciSequence.h"

#include <algorithm>


using std::string;
using std::to_string;
using std::reverse;

using v8::Local;
using v8::Value;
using v8::Number;
using v8::String;
using v8::Object;
using v8::Context;
using v8::Isolate;
using v8::Function;
using v8::Exception;
using v8::FunctionTemplate;



// ----- [CONSTRUCTORS] ------------------------------------------------------------------------------------------------

Nan::Persistent<Function> FibonacciSequence::constructor;

FibonacciSequence::FibonacciSequence(string firstNumber, string secondNumber) :
    firstNumber(firstNumber),
    secondNumber(secondNumber)
{ }



// ----- [PROTECTED METHODS] -------------------------------------------------------------------------------------------

string FibonacciSequence::sumBigIntegerNumber(const string& A, const string& B)
{
	string result;

	int a, ia = A.size() - 1;
	int b, ib = B.size() - 1;
	int sum, transfer = 0;

	while ((ia >= 0) || (ib >= 0)) {
		a = (ia >= 0 ? A[ia] - '0' : 0);
		b = (ib >= 0 ? B[ib] - '0' : 0);

		sum = a + b + transfer;
		result.push_back((sum % 10) + '0');
		transfer = sum / 10;

		--ia;
		--ib;
	}
	if (transfer != 0) {
		result.push_back(transfer + '0');
	}

	reverse(result.begin(), result.end());
	return result;
}



// ----- [PUBLIC METHODS] ----------------------------------------------------------------------------------------------

void FibonacciSequence::addonInitialization(Local<Object> exports)
{
    Local<Context> context = exports->CreationContext();
    Nan::HandleScope scope;

    Local<FunctionTemplate> tpl = Nan::New<FunctionTemplate>(New);
    tpl->SetClassName(Nan::New("FibonacciSequence").ToLocalChecked());
    tpl->InstanceTemplate()->SetInternalFieldCount(1);

    Nan::SetPrototypeMethod(tpl, "next", next);
    Nan::SetPrototypeMethod(tpl, "current", current);
    Nan::SetPrototypeMethod(tpl, "reset", reset);

    constructor.Reset(tpl->GetFunction(context).ToLocalChecked());
    exports->Set(context,
        Nan::New("FibonacciSequence").ToLocalChecked(),
        tpl->GetFunction(context).ToLocalChecked()
    );
}

void FibonacciSequence::New(const Nan::FunctionCallbackInfo<Value>& info)
{
    Local<Context> context = info.GetIsolate()->GetCurrentContext();
    Isolate* isolate = info.GetIsolate();

    if (info.IsConstructCall()) {
        string firstNumber = info[0]->IsUndefined() ? "0" : to_string(static_cast<int>(info[0]->NumberValue(context).FromJust()));
        string secondNumber = info[1]->IsUndefined() ? "1" : to_string(static_cast<int>(info[1]->NumberValue(context).FromJust()));

        FibonacciSequence* obj = new FibonacciSequence(firstNumber, secondNumber);
        obj->Wrap(info.This());
        info.GetReturnValue().Set(info.This());
    }
    else {
        const int argc = 2;
        Local<Value> argv[argc] = { info[0], info[1] };
        Local<Function> cons = Nan::New<Function>(constructor);
        info.GetReturnValue().Set(cons->NewInstance(context, argc, argv).ToLocalChecked());
    }
}

void FibonacciSequence::next(const Nan::FunctionCallbackInfo<Value>& info)
{
    FibonacciSequence* obj = ObjectWrap::Unwrap<FibonacciSequence>(info.Holder());

    Isolate* isolate = info.GetIsolate();
    info.GetReturnValue().Set(String::NewFromUtf8(isolate, obj->firstNumber.c_str()).ToLocalChecked());

    string next = obj->sumBigIntegerNumber(obj->firstNumber, obj->secondNumber);
    obj->firstNumber = obj->secondNumber;
    obj->secondNumber = next;
}

void FibonacciSequence::current(const Nan::FunctionCallbackInfo<Value>& info)
{
    FibonacciSequence* obj = ObjectWrap::Unwrap<FibonacciSequence>(info.Holder());

    Isolate* isolate = info.GetIsolate();
    info.GetReturnValue().Set(String::NewFromUtf8(isolate, obj->firstNumber.c_str()).ToLocalChecked());
}

void FibonacciSequence::reset(const Nan::FunctionCallbackInfo<Value>& info)
{
    FibonacciSequence* obj = ObjectWrap::Unwrap<FibonacciSequence>(info.Holder());

    Isolate* isolate = info.GetIsolate();
    if (info.Length() == 0) {
        obj->firstNumber = "0";
        obj->secondNumber = "1";
    }
    else  if (info.Length() == 2) {
        if ((info[0]->IsNumber() == false) || (info[1]->IsNumber() == false)) {
            isolate->ThrowException(
                Exception::TypeError(
                    String::NewFromUtf8(isolate,
                    "Invalid type of arguments.").ToLocalChecked()
                )
            );
            return;
        }

        obj->firstNumber = to_string(static_cast<int>(info[0].As<Number>()->Value()));
        obj->secondNumber = to_string(static_cast<int>(info[1].As<Number>()->Value()));
    }
    else {
        isolate->ThrowException(
            Exception::TypeError(
                String::NewFromUtf8(isolate,
                "Invalid number of arguments.").ToLocalChecked()
            )
        );
        return;
    }
}



// ----- [ADDON INITIALIZATION] ----------------------------------------------------------------------------------------

void addonInitialization(v8::Local<v8::Object> exports) {
    FibonacciSequence::addonInitialization(exports);
}

NODE_MODULE(FibonacciSequence, addonInitialization)