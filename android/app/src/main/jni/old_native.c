#include <string.h>
#include <jni.h>

jstring
Java_com_xband_MainActivity_stringFromOldJNI( JNIEnv* env,
                                                  jobject thiz )
{

    return (*env)->NewStringUTF(env, "Do you even lift!  Compiled with ABI ");
}