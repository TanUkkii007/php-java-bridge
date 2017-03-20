#include <php_embed.h>
#include <jni.h>
#include <zend_API.h>
#include <vector>
#include <string>

#ifndef _Included_com_github_tanukkii007_php_core_PHP
#define _Included_com_github_tanukkii007_php_core_PHP
#ifdef __cplusplus
extern "C" {
#endif

int eval_php(char* code, int argc, const char** argv) {
    zval *retval_ptr;
    int result;
    PHP_EMBED_START_BLOCK(argc, const_cast<char**>(argv))
    result = zend_eval_string(code, retval_ptr, "php-java-bridge");
    PHP_EMBED_END_BLOCK()
    return result;
}

JNIEXPORT jint JNICALL Java_com_github_tanukkii007_php_core_PHP_evalInternal(JNIEnv *jenv, jclass clazz, jstring code, jobjectArray argv) {
    const char *code_pstr = (const char *)jenv->GetStringUTFChars(code, 0);
    if (!code_pstr) return FAILURE;
    auto argc = jenv->GetArrayLength(argv);
    const char* buff[argc];
    jstring jstrings[argc];

    for (int i = 0; i < argc; ++i) {
        auto obj = jenv->GetObjectArrayElement(argv, i);
        auto jstr = static_cast<jstring >(obj);
        const char* arg_pstr = jenv->GetStringUTFChars(jstr, 0);
        buff[i] = arg_pstr;
        jstrings[i] = jstr;
    }
    auto result = eval_php(const_cast<char*>(code_pstr), argc, buff);
    for (int i = 0; i < argc; ++i) {
        auto arg_pstr = buff[i];
        auto jstr = jstrings[i];
        jenv->ReleaseStringUTFChars(jstr, arg_pstr);
    }
    jenv->ReleaseStringUTFChars(code, code_pstr);
    return result;
}

#ifdef __cplusplus
}
#endif
#endif