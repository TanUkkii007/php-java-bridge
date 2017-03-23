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

int eval_php(char* code, int argc, const char** argv, zval *retval_ptr) {
    int result;
    PHP_EMBED_START_BLOCK(argc, const_cast<char**>(argv))
    result = zend_eval_string(code, retval_ptr, "php-java-bridge" TSRMLS_CC);
    PHP_EMBED_END_BLOCK()
    return result;
}

jboolean convertZValToJBool(zval* zv) {
    jboolean result;
    switch (Z_TYPE_P(zv)) {
        case IS_NULL:
            NULL;
            break;
        case IS_BOOL:
            if (Z_LVAL_P(zv)) {
                result = true;
            } else {
                result = false;
            }
            break;
        case IS_LONG:
            Z_LVAL_P(zv);
            break;
        case IS_DOUBLE:
            Z_DVAL_P(zv);
        case IS_STRING:
            Z_STRVAL_P(zv);
        case IS_RESOURCE:
            break;
        case IS_ARRAY:
            break;
        case IS_OBJECT:
            break;
        default:
            break;
    }
    return result;
}

jlong convertZValToJLong(zval* zv) {
    jlong result;
    switch (Z_TYPE_P(zv)) {
        case IS_NULL:
            NULL;
            break;
        case IS_BOOL:
            Z_LVAL_P(zv);
            break;
        case IS_LONG:
            result = Z_LVAL_P(zv);
            break;
        case IS_DOUBLE:
            Z_DVAL_P(zv);
        case IS_STRING:
            Z_STRVAL_P(zv);
        case IS_RESOURCE:
            break;
        case IS_ARRAY:
            break;
        case IS_OBJECT:
            break;
        default:
            break;
    }
    return result;
}

jdouble convertZValToJDouble(zval* zv) {
    jdouble result;
    switch (Z_TYPE_P(zv)) {
        case IS_NULL:
            NULL;
            break;
        case IS_BOOL:
            Z_LVAL_P(zv);
            break;
        case IS_LONG:
            Z_LVAL_P(zv);
            break;
        case IS_DOUBLE:
            result = Z_DVAL_P(zv);
        case IS_STRING:
            Z_STRVAL_P(zv);
        case IS_RESOURCE:
            break;
        case IS_ARRAY:
            break;
        case IS_OBJECT:
            break;
        default:
            break;
    }
    return result;
}

jstring convertZValToJString(zval* zv, JNIEnv *jenv) {
    jstring result;
    switch (Z_TYPE_P(zv)) {
        case IS_NULL:
            NULL;
            break;
        case IS_BOOL:
            Z_LVAL_P(zv);
            break;
        case IS_LONG:
            Z_LVAL_P(zv);
            break;
        case IS_DOUBLE:
            Z_DVAL_P(zv);
        case IS_STRING:
            result = jenv->NewStringUTF(Z_STRVAL_P(zv));
        case IS_RESOURCE:
            break;
        case IS_ARRAY:
            break;
        case IS_OBJECT:
            break;
        default:
            break;
    }
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
    auto result = eval_php(const_cast<char*>(code_pstr), argc, buff, NULL);
    for (int i = 0; i < argc; ++i) {
        auto arg_pstr = buff[i];
        auto jstr = jstrings[i];
        jenv->ReleaseStringUTFChars(jstr, arg_pstr);
    }
    jenv->ReleaseStringUTFChars(code, code_pstr);
    return result;
}

#define JNI_BUILD_ARGS(jenv, argv, argc, buff, jstrings)                                                       \
    auto argc = jenv->GetArrayLength(argv);                                    \
    const char* buff[argc];                                                    \
    jstring jstrings[argc];                                                    \
    for (int i = 0; i < argc; ++i) {                                           \
        auto obj = jenv->GetObjectArrayElement(argv, i);                       \
        auto jstr = static_cast<jstring >(obj);                                \
        const char* arg_pstr = jenv->GetStringUTFChars(jstr, 0);               \
        buff[i] = arg_pstr;                                                    \
        jstrings[i] = jstr;                                                    \
    }                                                                          \

#define JNI_RELEASE_ARGS(argc, buff, jstrings)                             \
    for (int i = 0; i < argc; ++i) {                                       \
        auto arg_pstr = buff[i];                                           \
        auto jstr = jstrings[i];                                           \
        jenv->ReleaseStringUTFChars(jstr, arg_pstr);                       \
    }                                                                      \


JNIEXPORT jboolean JNICALL Java_com_github_tanukkii007_php_core_PHP_evalInternalBoolean
        (JNIEnv *jenv, jclass clazz, jstring code, jobjectArray argv) {
    jboolean result = 0;
    const char *code_pstr = (const char *)jenv->GetStringUTFChars(code, 0);
    if (!code_pstr) return result;
    zval *retval_ptr = (zval*)malloc(sizeof(zval));
    JNI_BUILD_ARGS(jenv, argv, argc, buff, jstrings)
    auto status = eval_php(const_cast<char*>(code_pstr), argc, buff, retval_ptr);
    JNI_RELEASE_ARGS(argc, buff, jstrings)
    jenv->ReleaseStringUTFChars(code, code_pstr);
    if (status == SUCCESS) {
        result = convertZValToJBool(retval_ptr);
        // ToDo: throw exception when not a boolean
    }
    free(retval_ptr);
    return result;
}

#ifdef __cplusplus
}
#endif
#endif