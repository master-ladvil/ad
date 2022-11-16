
#include <iostream>
#include <jni.h>
#include "Test.h"
#include "activeds.h"
#include <string>

void login(LPCWSTR uname,LPCWSTR pass)
{
std::cout << uname <<std::endl;
std::cout << pass << std::endl;
    HRESULT hr;
    IADsContainer* pCont;
    IDispatch* pDisp = NULL;
    IADs* pUser;
    // Initialize COM before calling any ADSI functions or interfaces.
    CoInitialize(NULL);

    hr = ADsOpenObject(L"LDAP://WIN-E018QOP75MH.test.local/CN=Users,DC=test,DC=local", uname, pass,
        ADS_SECURE_AUTHENTICATION, // For secure authentication
        IID_IADsContainer,
        (void**)&pCont);
    std::cout << hr << std::endl;
    std::string message = std::system_category().message(hr);
    std::cout << message << std::endl; 
}
LPWSTR ConvertToLPWSTR(const std::string& s)
{
    LPWSTR ws = new wchar_t[s.size() + 1]; // +1 for zero at the end
    copy(s.begin(), s.end(), ws);
    ws[s.size()] = 0; // zero at the end
    return ws;
}
LPCWSTR jstring2string(JNIEnv* env, jstring jStr) {
    const jclass stringClass = env->GetObjectClass(jStr);
    const jmethodID getBytes = env->GetMethodID(stringClass, "getBytes", "(Ljava/lang/String;)[B");
    const jbyteArray stringJbytes = (jbyteArray)env->CallObjectMethod(jStr, getBytes, env->NewStringUTF("UTF-8"));

    size_t length = (size_t)env->GetArrayLength(stringJbytes);
    jbyte* pBytes = env->GetByteArrayElements(stringJbytes, NULL);

    std::string ret = std::string((char*)pBytes, length);
    env->ReleaseByteArrayElements(stringJbytes, pBytes, JNI_ABORT);

    env->DeleteLocalRef(stringJbytes);
    env->DeleteLocalRef(stringClass);
    LPCWSTR converted = ConvertToLPWSTR(ret);
    return converted;
    //return ConvertToLPWSTR(ret);
}
LPWSTR convert(LPCWSTR s) {
    wchar_t* att2 = (wchar_t*)s;
    LPWSTR att3 = att2;
    return att3;
}

JNIEXPORT void JNICALL Java_Test_testt
(JNIEnv* env, jobject obj, jstring uname, jstring pass) {
    std::cout << "inside" << std::endl;
    login(convert(jstring2string(env, uname)), jstring2string(env, pass));
}
