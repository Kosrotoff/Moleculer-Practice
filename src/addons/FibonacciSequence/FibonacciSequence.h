#include <nan.h>
#include <string>


class FibonacciSequence : public Nan::ObjectWrap {
private:
    std::string firstNumber;
    std::string secondNumber;

protected:
    std::string sumBigIntegerNumber(const std::string& A, const std::string& B);

public:
    static Nan::Persistent<v8::Function> constructor;


    explicit FibonacciSequence(std::string firstNumber = "0", std::string secondNumber = "1");

    static void New(const Nan::FunctionCallbackInfo<v8::Value>& info);
    static void addonInitialization(v8::Local<v8::Object> exports);

    static void next(const Nan::FunctionCallbackInfo<v8::Value>& info);
    static void current(const Nan::FunctionCallbackInfo<v8::Value>& info);
    static void reset(const Nan::FunctionCallbackInfo<v8::Value>& info);
};


void addonInitialization(v8::Local<v8::Object> exports);