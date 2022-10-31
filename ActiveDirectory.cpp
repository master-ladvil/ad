
#include <iostream>
#include <jni.h>
#include "ActiveDirectory.h"
#include "atlbase.h"
#include "activeds.h"
#include <string>
#include <comdef.h>  
#include <assert.h>

int login(LPCWSTR uname, LPCWSTR pass)
{

    HRESULT hr;
    IADsContainer* pCont;
    IDispatch* pDisp = NULL;
    IADs* pUser;

    // Initialize COM before calling any ADSI functions or interfaces.
    CoInitialize(NULL);

    hr = ADsOpenObject(L"LDAP://WIN-I5NTT42PG5D.test.local/CN=Users,DC=test,DC=local", uname, pass,
        ADS_SECURE_AUTHENTICATION, // For secure authentication
        IID_IADsContainer,
        (void**)&pCont);
    std::cout << hr << std::endl;
    std::string message = std::system_category().message(hr);
    std::cout << message << std::endl;
    if (!SUCCEEDED(hr)) {
        return 0;
    }
    else {
        return 1;
    }
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




JNIEXPORT jint JNICALL Java_ActiveDirectory_getP
(JNIEnv* env, jobject obj, jstring user) {
    std::cout << "inside testp" << std::endl;
    HRESULT hr;
    const char* result;
    CoInitialize(NULL);
    IADs* pUsr = NULL;
    LPCWSTR userpath = jstring2string(env, user);
    //results are stored in column
    ADS_SEARCH_COLUMN col;
    //creating path
    CComBSTR path = L"LDAP://WIN-I5NTT42PG5D.test.local/CN=";
    path += userpath;
    path += L",CN=Users,DC=test,DC=local";
    //connecting
    hr = ADsOpenObject(path, L"Administrator", L"admin@Admin",
        ADS_SECURE_AUTHENTICATION,
        IID_IADs,
        (void**)&pUsr);
    std::string message = std::system_category().message(hr);
    std::cout << message << std::endl;
    if (!SUCCEEDED(hr)) { std::cout << "failed connect" << std::endl; }
    VARIANT var;
    VariantInit(&var);
    hr = pUsr->Get(CComBSTR("userAccountControl"), &var);
    //std::string messageeee = std::system_category().message(hr);
    //std::cout << messageeee << std::endl;
    if (SUCCEEDED(hr)) {
        std::cout << V_BSTR(&var) << std::endl;
        std::cout << V_I4(&var) << std::endl;
        int value;
        value = V_I4(&var);
        VariantClear(&var);
        return value;
    }
    else {
        std::cout << "sorry" << std::endl;
    }
    if (pUsr) {
        pUsr->Release();
    }
}
JNIEXPORT jstring JNICALL Java_ActiveDirectory_getname
(JNIEnv* env, jobject obj, jstring user) {

    HRESULT hr;
    int i = 0, arrSize = 0;
    wchar_t* buffer;
    const char* result;
    jstring* str;
    CoInitialize(NULL);
    IDirectorySearch* pdSearch = NULL;
    LPCWSTR userpath = jstring2string(env, user);
    //results are stored in column
    ADS_SEARCH_COLUMN col;
    //creating path
    CComBSTR path = L"LDAP://WIN-I5NTT42PG5D.test.local/CN=";
    path += userpath;
    path += L",CN=Users,DC=test,DC=local";
    //connecting
    hr = ADsOpenObject(path, L"Administrator", L"admin@Admin",
        ADS_SECURE_AUTHENTICATION,
        IID_IDirectorySearch,
        (void**)&pdSearch);
    if (SUCCEEDED(hr)) {
        std::cout << "initiating search"<<std::endl;
        LPWSTR userAccountControl = convert(L"givenName");
        LPWSTR pszAttr[] = { userAccountControl };
        ADS_SEARCH_HANDLE hSearch;
        DWORD dwCount = 0;
        DWORD nameSize = sizeof(pszAttr) / sizeof(LPWSTR);
        LPWSTR path = convert(L"(&(objectClass=user))");
        hr = pdSearch->ExecuteSearch(path,pszAttr,nameSize, &hSearch);
        std::string messagee = std::system_category().message(hr);
        std::cout << messagee << std::endl;
        str = new jstring[9];
        while (pdSearch->GetNextRow(hSearch) != S_ADS_NOMORE_ROWS) {
            std::cout << "inside while" << std::endl;
            hr = pdSearch->GetColumn(hSearch, userAccountControl, &col);
            std::string messagee = std::system_category().message(hr);
            std::cout << messagee << std::endl;
            
            if (SUCCEEDED(hr)) {
                if (col.dwADsType == ADSTYPE_CASE_IGNORE_STRING)
                {
                    std::cout << "inside caseignore" << std::endl;
                    buffer = (wchar_t*)col.pADsValues->CaseIgnoreString;
                    _bstr_t b(buffer);
                    result = b;
                    return env->NewStringUTF(result);
                }
                else {
                    return env->NewStringUTF("sorry");
                }
            }
            if (!SUCCEEDED(hr)) {
                const char* message = "Not Entered";
                str[0] = env->NewStringUTF(message);
                std::string messagee = std::system_category().message(hr);
                std::cout << messagee << std::endl;
                return env->NewStringUTF(message);
            }
        }
        pdSearch->CloseSearchHandle(hSearch);
        pdSearch->Release();

    }
}


    
JNIEXPORT jint JNICALL Java_ActiveDirectory_testt
(JNIEnv* env, jobject obj, jstring uname, jstring pass)
 {
    std::cout << "inside" << std::endl;
    return login(convert(jstring2string(env, uname)), jstring2string(env, pass));
}





JNIEXPORT jlong JNICALL Java_ActiveDirectory_passWordLastSet
(JNIEnv* env, jobject obj, jstring user) {
    HRESULT hr;
    jlong result;
    CComPtr<IADs> spads;
    CoInitialize(NULL);
    LPCWSTR userPath = jstring2string(env, user);
    CComBSTR  path = L"LDAP://WIN-I5NTT42PG5D.test.local/CN=";
    path += userPath;
    path += L",CN=Users,DC=Test,DC=local";
    hr = ADsOpenObject(path, L"Administrator", L"admin@Admin",
        ADS_SECURE_AUTHENTICATION, // For secure authentication
        IID_IADs,
        (void**)&spads);
    std::string message = std::system_category().message(hr);
    std::cout << message << std::endl;
    if (FAILED(hr))
    {
        std::cout << "failed to connect" << std::endl;
        return hr;
    }
    CComVariant svar;
    hr = spads->Get(CComBSTR("pwdLastSet"), &svar);
    std::string messageeee = std::system_category().message(hr);
    std::cout << messageeee << std::endl;
    if (FAILED(hr))
    {
        std::cout << "failed to get attribute" << std::endl;
        return hr;
    }

    // Get the IADsLargeInteger interface.
    CComPtr<IADsLargeInteger> spli;
    hr = svar.pdispVal->QueryInterface(IID_IADsLargeInteger,
        (LPVOID*)&spli);
    if (FAILED(hr))
    {
        return hr;
    }

    long lHigh;
    long lLow;
    hr = spli->get_HighPart(&lHigh);
    hr = spli->get_LowPart(&lLow);
    __int64 i64;
    i64 = (ULONG)lHigh;
    i64 = (i64 << 32);
    i64 = i64 + (ULONG)lLow;
    // Print all of the values.
    printf("Combined = %I64d\n", i64);
    return i64;
}

int ResetPassWord(LPCWSTR pwszAccountName, LPCWSTR pwszNewPassword) {
    HRESULT hr;
    CoInitialize(NULL);
    IADsContainer* pUsers = NULL;
    CComBSTR  path = L"LDAP://WIN-I5NTT42PG5D.test.local/CN=";
    path += pwszAccountName;
    path += L",CN=Users,DC=Test,DC=local";
    hr = ADsOpenObject(path, L"Administrator", L"admin@Admin",
        ADS_SECURE_AUTHENTICATION, // For secure authentication
        IID_IADsContainer,
        (void**)&pUsers);
    std::string messageee = std::system_category().message(hr);
    std::cout << messageee << std::endl;
    if (SUCCEEDED(hr))
    {
        IADsUser* pDisp = NULL;
        hr = pUsers->QueryInterface(IID_IADsUser, (void**)&pDisp);
        if (SUCCEEDED(hr)) {
            hr = pDisp->SetPassword((BSTR)pwszNewPassword);
            CoUninitialize();
            std::string message = std::system_category().message(hr);
            std::cout << message << std::endl;
            return hr;
        }
        std::string message = std::system_category().message(hr);
        std::cout << message << std::endl;
        return hr;
    }
    std::string message = std::system_category().message(hr);
    std::cout << message << std::endl;
    return hr;
}

JNIEXPORT jint JNICALL Java_ActiveDirectory_resetPassword
(JNIEnv* env, jobject object, jstring user, jstring pass){

    LPCWSTR username = jstring2string(env, user);
    LPCWSTR newpass = jstring2string(env, pass);
    int result = ResetPassWord(username, newpass);
    std::cout << "pwd changed" << std::endl;
    return result;

}