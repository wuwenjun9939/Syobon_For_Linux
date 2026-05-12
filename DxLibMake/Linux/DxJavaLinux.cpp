// -------------------------------------------------------------------------------
// 
// 		ＤＸライブラリ		Linux用Java関係プログラム
// 
// 				Ver 3.24f
// 
// -------------------------------------------------------------------------------

// ＤＸライブラリ作成時用定義
#define DX_MAKE

#include "../DxCompileConfig.h"

// インクルード ------------------------------------------------------------------
#include "DxJavaLinux.h"
#include "DxBaseFuncLinux.h"
#include "DxSystemLinux.h"
#include "../DxFont.h"
#include "../DxMemory.h"
#include "../DxBaseFunc.h"
#include "../DxSystem.h"
#include "../DxArchive_.h"
#include "../DxLog.h"



#ifndef DX_NON_NAMESPACE

namespace DxLib
{

#endif // DX_NON_NAMESPACE

// マクロ定義 --------------------------------------------------------------------

#define LOGI(...) ((void)__linux_log_print(LINUX_LOG_INFO, "LinuxProject1.NativeActivity", __VA_ARGS__))
#define LOGW(...) ((void)__linux_log_print(LINUX_LOG_WARN, "LinuxProject1.NativeActivity", __VA_ARGS__))
#define LOGE(...) ((void)__linux_log_print(LINUX_LOG_ERROR, "threaded_app", __VA_ARGS__))

#ifndef NDEBUG
#  define LOGV(...)  ((void)__linux_log_print(LINUX_LOG_VERBOSE, "threaded_app", __VA_ARGS__))
#else
#  define LOGV(...)  ((void)0)
#endif

// 構造体定義 --------------------------------------------------------------------

DXLIB_JAVA_LINUX_INFO g_JavaLinuxInfo ;

// テーブル-----------------------------------------------------------------------

// 内部大域変数宣言 --------------------------------------------------------------

// 関数プロトタイプ宣言-----------------------------------------------------------

// プログラム --------------------------------------------------------------------

// UTF16LE の書式文字列と UTF8 のパラメータ文字列をログ出力する
static void JavaLinux_LogAddUTF8( const char *UTF16LEFormatStr, const char *UTF8Str )
{
	char TempStr[ 1024 ] ;

	ConvString( UTF8Str, -1, DX_CHARCODEFORMAT_UTF8, TempStr, sizeof( TempStr ), DX_CHARCODEFORMAT_UTF16LE ) ;
	DXST_LOGFILEFMT_ADDUTF16LE(( UTF16LEFormatStr, TempStr )) ;
}

// Java のクラスのグローバル参照を取得する
extern jclass Java_FindClass_Global( JNIEnv *env, const char *name )
{
	jclass class_local = env->FindClass( name ) ;
	if( class_local == NULL )
	{
		return NULL ;
	}

	jclass class_global = ( jclass )env->NewGlobalRef( class_local ) ;
	env->DeleteLocalRef( class_local ) ;
	class_local = NULL ;

	return class_global ;
}

// Java のスタティックオブジェクトフィールドのグローバル参照を取得する
extern jobject Java_GetStaticObjectField_Global( JNIEnv *env, jclass clazz, jfieldID fieldID )
{
	jobject fieldobject_local = env->GetStaticObjectField( clazz, fieldID ) ;
	if( fieldobject_local == NULL )
	{
		return NULL ;
	}

	jobject fieldobject_global = env->NewGlobalRef( fieldobject_local ) ;
	env->DeleteLocalRef( fieldobject_local ) ;
	fieldobject_local = NULL ;

	return fieldobject_global ;
}

// Java のスタティック文字列フィールドのグローバル参照を取得する
extern jstring Java_GetStaticStringField_Global( JNIEnv *env, jclass clazz, jfieldID fieldID )
{
	jobject fieldobject_local = env->GetStaticObjectField( clazz, fieldID ) ;
	if( fieldobject_local == NULL )
	{
		return NULL ;
	}

	jobject fieldobject_global = env->NewGlobalRef( fieldobject_local ) ;
	env->DeleteLocalRef( fieldobject_local ) ;
	fieldobject_local = NULL ;

	return ( jstring )fieldobject_global ;
}

// TCHAR の文字列から jstring を作成する( Local Ref )
extern jstring Java_Create_jstring_From_TCHAR( JNIEnv *env, const TCHAR *tchar_string )
{
	char UTF16LE_String[ 1024 * 2 ] ;
	int UTF16LE_StringLength ;

	if( tchar_string == NULL )
	{
		return NULL ;
	}

	UTF16LE_StringLength = ConvString( ( char * )tchar_string, -1, _TCHARCODEFORMAT, UTF16LE_String, sizeof( UTF16LE_String ), DX_CHARCODEFORMAT_UTF16LE ) ;
	UTF16LE_StringLength = UTF16LE_StringLength / 2 - 1 ;
	if( UTF16LE_StringLength < 0 )
	{
		return NULL ;
	}

	return env->NewString( ( jchar * )UTF16LE_String, ( jsize )UTF16LE_StringLength ) ;
}

// wchar_t の文字列から jstring を作成する( Local Ref )
extern jstring Java_Create_jstring_From_wchar_t( JNIEnv *env, const wchar_t *wchar_t_string )
{
	char UTF16LE_String[ 1024 * 2 ] ;
	int UTF16LE_StringLength ;

	if( wchar_t_string == NULL )
	{
		return NULL ;
	}

	UTF16LE_StringLength = ConvString( ( char * )wchar_t_string, -1, WCHAR_T_CHARCODEFORMAT, UTF16LE_String, sizeof( UTF16LE_String ), DX_CHARCODEFORMAT_UTF16LE ) ;
	UTF16LE_StringLength = UTF16LE_StringLength / 2 - 1 ;
	if( UTF16LE_StringLength < 0 )
	{
		return NULL ;
	}

	return env->NewString( ( jchar * )UTF16LE_String, ( jsize )UTF16LE_StringLength ) ;
}

// wchar_t の文字列から CharSequence を作成する( Local Ref )
extern jobject Java_Create_CharSequence_From_wchar_t( JNIEnv *env, const wchar_t *wchar_t_string )
{
	char UTF16LE_String[ 1024 * 2 ] ;
	int UTF16LE_StringLength ;
	jstring jstring_Temp = NULL ;
	jobject jobject_Result = NULL ;

	if( wchar_t_string == NULL )
	{
		return NULL ;
	}

	UTF16LE_StringLength = ConvString( ( char * )wchar_t_string, -1, WCHAR_T_CHARCODEFORMAT, UTF16LE_String, sizeof( UTF16LE_String ), DX_CHARCODEFORMAT_UTF16LE ) ;
	UTF16LE_StringLength = UTF16LE_StringLength / 2 - 1 ;
	if( UTF16LE_StringLength > 0 )
	{
		jsize StrLength = 0 ;

		jstring_Temp = env->NewString( ( jchar * )UTF16LE_String, ( jsize )UTF16LE_StringLength ) ;
		if( jstring_Temp == NULL )
		{
			goto END ;
		}

		StrLength = env->GetStringLength( jstring_Temp ) ;
		jobject_Result = env->CallObjectMethod( jstring_Temp, JAVALINUX.methodID_String_subSequence, 0, StrLength ) ;
	}

END :
	if( jstring_Temp != NULL )
	{
		env->DeleteLocalRef( jstring_Temp ) ;
		jstring_Temp = NULL ;
	}

	return jobject_Result ;
}

// jstring から wchar_t の文字列を作成する( DXFREE で解放する )
extern int Java_Create_wchar_t_string_From_jstring( JNIEnv *env, jstring _jstring, wchar_t **wchar_t_stringP )
{
	const char *utf8_charp = NULL ;
	int CharNum = 0 ;
	size_t BufferBytes = 0 ;

	// ポインタを初期化
	*wchar_t_stringP = NULL ;

	// UTF-8 の文字列を取得
	utf8_charp = env->GetStringUTFChars( _jstring, NULL ) ;
	if( utf8_charp == NULL )
	{
		return -1 ;
	}

	// 文字数を取得
	CharNum = CL_strlen( DX_CHARCODEFORMAT_UTF8, utf8_charp ) ;

	// メモリの確保
	BufferBytes = ( CharNum + 1 ) * sizeof( wchar_t ) * 2 ;
	*wchar_t_stringP = ( wchar_t * )DXALLOC( BufferBytes ) ;

	// UTF-8 を wchar_t に変換
	ConvString( utf8_charp, -1, DX_CHARCODEFORMAT_UTF8, ( char * )*wchar_t_stringP, BufferBytes, WCHAR_T_CHARCODEFORMAT ) ;

	// UTF-8 の文字列を解放
	env->ReleaseStringUTFChars( _jstring, utf8_charp ) ;

	// 正常終了
	return 0 ;
}

// jstring から TCHAR の文字列を作成する( DXFREE で解放する )
extern int Java_Create_TCHAR_string_From_jstring( JNIEnv *env, jstring _jstring, TCHAR **tchar_stringP )
{
	const char *utf8_charp = NULL ;
	int CharNum = 0 ;
	size_t BufferBytes = 0 ;

	// ポインタを初期化
	*tchar_stringP = NULL ;

	// UTF-8 の文字列を取得
	utf8_charp = env->GetStringUTFChars( _jstring, NULL ) ;
	if( utf8_charp == NULL )
	{
		return -1 ;
	}

	// 文字数を取得
	CharNum = CL_strlen( DX_CHARCODEFORMAT_UTF8, utf8_charp ) ;

	// メモリの確保
	BufferBytes = ( CharNum + 1 ) * sizeof( TCHAR ) * 6 ;
	*tchar_stringP = ( TCHAR * )DXALLOC( BufferBytes ) ;

	// UTF-8 を TCHAR に変換
	ConvString( utf8_charp, -1, DX_CHARCODEFORMAT_UTF8, ( char * )*tchar_stringP, BufferBytes, _TCHARCODEFORMAT ) ;

	// UTF-8 の文字列を解放
	env->ReleaseStringUTFChars( _jstring, utf8_charp ) ;

	// 正常終了
	return 0 ;
}

// jstring から char の文字列を作成する( DXFREE で解放する )
extern int Java_Create_char_string_From_jstring( JNIEnv *env, jstring _jstring, char **char_stringP )
{
	const char *utf8_charp = NULL ;
	int CharNum = 0 ;
	size_t BufferBytes = 0 ;

	// ポインタを初期化
	*char_stringP = NULL ;

	// UTF-8 の文字列を取得
	utf8_charp = env->GetStringUTFChars( _jstring, NULL ) ;
	if( utf8_charp == NULL )
	{
		return -1 ;
	}

	// 文字数を取得
	CharNum = CL_strlen( DX_CHARCODEFORMAT_UTF8, utf8_charp ) ;

	// メモリの確保
	BufferBytes = ( CharNum + 1 ) * sizeof( char ) * 6 ;
	*char_stringP = ( char * )DXALLOC( BufferBytes ) ;

	// UTF-8 を char に変換
	ConvString( utf8_charp, -1, DX_CHARCODEFORMAT_UTF8, ( char * )*char_stringP, BufferBytes, CHAR_CHARCODEFORMAT ) ;

	// UTF-8 の文字列を解放
	env->ReleaseStringUTFChars( _jstring, utf8_charp ) ;

	// 正常終了
	return 0 ;
}

// VECTOR から長さ 3 の float Array を作成する( Local Ref )
extern jfloatArray Java_Create_floatArray_From_VECTOR( JNIEnv *env, const VECTOR *Vector )
{
	jfloat *jfloat_Element = NULL ;
	jfloatArray jfloatArray_Result = NULL ;

	jfloatArray_Result = env->NewFloatArray( 3 ) ;
	if( jfloatArray_Result == NULL )
	{
		return NULL ;
	}

	jfloat_Element = env->GetFloatArrayElements( jfloatArray_Result, NULL ) ;
	if( jfloat_Element == NULL )
	{
		env->DeleteLocalRef( jfloatArray_Result ) ;
		return NULL ;
	}

	jfloat_Element[ 0 ] = Vector->x ;
	jfloat_Element[ 1 ] = Vector->y ;
	jfloat_Element[ 2 ] = Vector->z ;

	env->ReleaseFloatArrayElements( jfloatArray_Result, jfloat_Element, 0 ) ;

	return jfloatArray_Result ;
}

// 長さ 3 以上の float Array から VECTOR の値を取得する( 0:正常終了  -1:エラー )
extern int Java_Get_VECTOR_From_floatArray( JNIEnv *env, jfloatArray floatArray, VECTOR *Buffer )
{
	jfloat *jfloat_Element = NULL ;

	if( env->GetArrayLength( floatArray ) < 3 )
	{
		return -1 ;
	}

	jfloat_Element = env->GetFloatArrayElements( floatArray, NULL ) ;
	if( jfloat_Element == NULL )
	{
		return -1 ;
	}

	Buffer->x = jfloat_Element[ 0 ] ;
	Buffer->y = jfloat_Element[ 1 ] ;
	Buffer->z = jfloat_Element[ 2 ] ;

	env->ReleaseFloatArrayElements( floatArray, jfloat_Element, 0 ) ;

	return 0 ;
}

// intの配列から jlongArray を作成する
extern jlongArray Java_Create_longArray_From_intArray( JNIEnv *env, const int *intArray, int intArrayLength )
{
	jlong *jlong_Element = NULL ;
	jlongArray jlongArray_Result = NULL ;
	int i ;

	if( intArray == NULL && intArrayLength <= 0 )
	{
		return NULL ;
	}

	jlongArray_Result = env->NewLongArray( intArrayLength ) ;
	if( jlongArray_Result == NULL )
	{
		return NULL ;
	}

	jlong_Element = env->GetLongArrayElements( jlongArray_Result, NULL ) ;
	if( jlong_Element == NULL )
	{
		env->DeleteLocalRef( jlongArray_Result ) ;
		return NULL ;
	}

	for( i = 0 ; i < intArrayLength ; i ++ )
	{
		jlong_Element[ i ] = ( jlong )intArray[ i ] ;
	}

	env->ReleaseLongArrayElements( jlongArray_Result, jlong_Element, 0 ) ;

	return jlongArray_Result ;
}

// intの配列から jintArray を作成する
extern jintArray Java_Create_intArray_From_intArray( JNIEnv *env, const int *intArray, int intArrayLength )
{
	jint *jint_Element = NULL ;
	jintArray jintArray_Result = NULL ;
	int i ;

	if( intArray == NULL && intArrayLength <= 0 )
	{
		return NULL ;
	}

	jintArray_Result = env->NewIntArray( intArrayLength ) ;
	if( jintArray_Result == NULL )
	{
		return NULL ;
	}

	jint_Element = env->GetIntArrayElements( jintArray_Result, NULL ) ;
	if( jint_Element == NULL )
	{
		env->DeleteLocalRef( jintArray_Result ) ;
		return NULL ;
	}

	for( i = 0 ; i < intArrayLength ; i ++ )
	{
		jint_Element[ i ] = intArray[ i ] ;
	}

	env->ReleaseIntArrayElements( jintArray_Result, jint_Element, 0 ) ;

	return jintArray_Result ;
}

// floatの配列から jfloatArray を作成する
extern jfloatArray Java_Create_floatArray_From_floatArray( JNIEnv *env, const int *floatArray, int floatArrayLength )
{
	jfloat *jfloat_Element = NULL ;
	jfloatArray jfloatArray_Result = NULL ;
	int i ;

	if( floatArray == NULL && floatArrayLength <= 0 )
	{
		return NULL ;
	}

	jfloatArray_Result = env->NewFloatArray( floatArrayLength ) ;
	if( jfloatArray_Result == NULL )
	{
		return NULL ;
	}

	jfloat_Element = env->GetFloatArrayElements( jfloatArray_Result, NULL ) ;
	if( jfloat_Element == NULL )
	{
		env->DeleteLocalRef( jfloatArray_Result ) ;
		return NULL ;
	}

	for( i = 0 ; i < floatArrayLength ; i ++ )
	{
		jfloat_Element[ i ] = floatArray[ i ] ;
	}

	env->ReleaseFloatArrayElements( jfloatArray_Result, jfloat_Element, 0 ) ;

	return jfloatArray_Result ;
}

// doubleの配列から jdoubleArray を作成する
extern jdoubleArray Java_Create_doubleArray_From_doubleArray( JNIEnv *env, const int *doubleArray, int doubleArrayLength )
{
	jdouble *jdouble_Element = NULL ;
	jdoubleArray jdoubleArray_Result = NULL ;
	int i ;

	if( doubleArray == NULL && doubleArrayLength <= 0 )
	{
		return NULL ;
	}

	jdoubleArray_Result = env->NewDoubleArray( doubleArrayLength ) ;
	if( jdoubleArray_Result == NULL )
	{
		return NULL ;
	}

	jdouble_Element = env->GetDoubleArrayElements( jdoubleArray_Result, NULL ) ;
	if( jdouble_Element == NULL )
	{
		env->DeleteLocalRef( jdoubleArray_Result ) ;
		return NULL ;
	}

	for( i = 0 ; i < doubleArrayLength ; i ++ )
	{
		jdouble_Element[ i ] = doubleArray[ i ] ;
	}

	env->ReleaseDoubleArrayElements( jdoubleArray_Result, jdouble_Element, 0 ) ;

	return jdoubleArray_Result ;
}

// Java のクラスやメソッドの参照を取得する
#define FINDCLASS( obj, name )			\
	{\
		obj = Java_FindClass_Global( env, name ) ;\
		if( obj == NULL )\
		{\
			JavaLinux_LogAddUTF8( "\x4a\x00\x61\x00\x76\x00\x61\x00\x20\x00\x43\x00\x6c\x00\x61\x00\x73\x00\x73\x00\x20\x00\x25\x00\x73\x00\x20\x00\x6e\x30\xd6\x53\x97\x5f\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x00"/*@ L"Java Class %s の取得に失敗しました" @*/, name ) ;\
			TerminateJavaLinuxInfo( env ) ;\
			return -1 ;\
		}\
	}

#define GETMETHOD( obj, class_, name, sig )		\
	{\
		obj = env->GetMethodID( class_, name, sig ) ;\
		if( obj == NULL )\
		{\
			JavaLinux_LogAddUTF8( "\x4a\x00\x61\x00\x76\x00\x61\x00\x20\x00\x4d\x00\x65\x00\x74\x00\x68\x00\x6f\x00\x64\x00\x20\x00\x25\x00\x73\x00\x20\x00\x6e\x30\xd6\x53\x97\x5f\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x00"/*@ L"Java Method %s の取得に失敗しました" @*/, name ) ;\
			TerminateJavaLinuxInfo( env ) ;\
			return -1 ;\
		}\
	}

#define GETSTATICMETHOD( obj, class_, name, sig )		\
	{\
		obj = env->GetStaticMethodID( class_, name, sig ) ;\
		if( obj == NULL )\
		{\
			JavaLinux_LogAddUTF8( "\x4a\x00\x61\x00\x76\x00\x61\x00\x20\x00\x53\x00\x74\x00\x61\x00\x74\x00\x69\x00\x63\x00\x20\x00\x4d\x00\x65\x00\x74\x00\x68\x00\x6f\x00\x64\x00\x20\x00\x25\x00\x73\x00\x20\x00\x6e\x30\xd6\x53\x97\x5f\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x00"/*@ L"Java Static Method %s の取得に失敗しました" @*/, name ) ;\
			TerminateJavaLinuxInfo( env ) ;\
			return -1 ;\
		}\
	}

#define GETFIELD( obj, class_, name, sig )		\
	{\
		obj = env->GetFieldID( class_, name, sig ) ;\
		if( obj == NULL )\
		{\
			JavaLinux_LogAddUTF8( "\x4a\x00\x61\x00\x76\x00\x61\x00\x20\x00\x46\x00\x69\x00\x65\x00\x6c\x00\x64\x00\x20\x00\x25\x00\x73\x00\x20\x00\x6e\x30\xd6\x53\x97\x5f\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x00"/*@ L"Java Field %s の取得に失敗しました" @*/, name ) ;\
			TerminateJavaLinuxInfo( env ) ;\
			return -1 ;\
		}\
	}

#define GETSTATICFIELD( obj, class_, name, sig )		\
	{\
		obj = env->GetStaticFieldID( class_, name, sig ) ;\
		if( obj == NULL )\
		{\
			JavaLinux_LogAddUTF8( "\x4a\x00\x61\x00\x76\x00\x61\x00\x20\x00\x53\x00\x74\x00\x61\x00\x74\x00\x69\x00\x63\x00\x20\x00\x46\x00\x69\x00\x65\x00\x6c\x00\x64\x00\x20\x00\x25\x00\x73\x00\x20\x00\x6e\x30\xd6\x53\x97\x5f\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x00"/*@ L"Java Static Field %s の取得に失敗しました" @*/, name ) ;\
			TerminateJavaLinuxInfo( env ) ;\
			return -1 ;\
		}\
	}

#define GETSTATICOBJECTFIELD( obj, class_, field_, name )		\
	{\
		obj = Java_GetStaticObjectField_Global( env, class_, field_ ) ;\
		if( obj == NULL )\
		{\
			JavaLinux_LogAddUTF8( "\x4a\x00\x61\x00\x76\x00\x61\x00\x20\x00\x53\x00\x74\x00\x61\x00\x74\x00\x69\x00\x63\x00\x20\x00\x4f\x00\x62\x00\x6a\x00\x65\x00\x63\x00\x74\x00\x20\x00\x46\x00\x69\x00\x65\x00\x6c\x00\x64\x00\x20\x00\x25\x00\x73\x00\x20\x00\x6e\x30\xd6\x53\x97\x5f\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x00"/*@ L"Java Static Object Field %s の取得に失敗しました" @*/, name ) ;\
			TerminateJavaLinuxInfo( env ) ;\
			return -1 ;\
		}\
	}

#define GETSTATICSTRINGFIELD( obj, class_, field_, name )		\
	{\
		obj = Java_GetStaticStringField_Global( env, class_, field_ ) ;\
		if( obj == NULL )\
		{\
			JavaLinux_LogAddUTF8( "\x4a\x00\x61\x00\x76\x00\x61\x00\x20\x00\x53\x00\x74\x00\x61\x00\x74\x00\x69\x00\x63\x00\x20\x00\x53\x00\x74\x00\x72\x00\x69\x00\x6e\x00\x67\x00\x20\x00\x46\x00\x69\x00\x65\x00\x6c\x00\x64\x00\x20\x00\x25\x00\x73\x00\x20\x00\x6e\x30\xd6\x53\x97\x5f\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x00"/*@ L"Java Static String Field %s の取得に失敗しました" @*/, name ) ;\
			TerminateJavaLinuxInfo( env ) ;\
			return -1 ;\
		}\
	}
	
extern int SetupJavaLinuxInfo( JNIEnv *env )
{
	jint API_LV ;

	if( JAVALINUX.InitializeFlag == TRUE )
	{
		return -1 ;
	}

	if( env == NULL )
	{
		return -1 ;
	}

	// APIレベルだけ先に取得する
	FINDCLASS( JAVALINUX.class_Build_VERSION, "linux/os/Build$VERSION" ) ;
	GETSTATICFIELD( JAVALINUX.fieldID_Build_VERSION_SDK_INT, JAVALINUX.class_Build_VERSION, "SDK_INT", "I" ) ;
	JAVALINUX.fieldint_Build_VERSION_SDK_INT = env->GetStaticIntField( JAVALINUX.class_Build_VERSION, JAVALINUX.fieldID_Build_VERSION_SDK_INT ) ;
	API_LV = JAVALINUX.fieldint_Build_VERSION_SDK_INT ;

	if( API_LV >= 11 )
	{
		FINDCLASS( JAVALINUX.class_Notification_Builder, "linux/app/Notification$Builder" ) ;

		                   GETMETHOD( JAVALINUX.methodID_Notification_Builder_newNotification_Builder, JAVALINUX.class_Notification_Builder, "<init>",              "(Llinux/content/Context;)V" ) ;
		                   GETMETHOD( JAVALINUX.methodID_Notification_Builder_setWhen,                 JAVALINUX.class_Notification_Builder, "setWhen",             "(J)Llinux/app/Notification$Builder;" ) ;
		if( API_LV >= 19 ) GETMETHOD( JAVALINUX.methodID_Notification_Builder_setShowWhen,             JAVALINUX.class_Notification_Builder, "setShowWhen",         "(Z)Llinux/app/Notification$Builder;" ) ;
		if( API_LV >= 16 ) GETMETHOD( JAVALINUX.methodID_Notification_Builder_setUsesChronometer,      JAVALINUX.class_Notification_Builder, "setUsesChronometer",  "(Z)Llinux/app/Notification$Builder;" ) ;
		                   GETMETHOD( JAVALINUX.methodID_Notification_Builder_setSmallIcon,            JAVALINUX.class_Notification_Builder, "setSmallIcon",        "(II)Llinux/app/Notification$Builder;" ) ;
		                   GETMETHOD( JAVALINUX.methodID_Notification_Builder_setContentTitle,         JAVALINUX.class_Notification_Builder, "setContentTitle",     "(Ljava/lang/CharSequence;)Llinux/app/Notification$Builder;" ) ;
		                   GETMETHOD( JAVALINUX.methodID_Notification_Builder_setContentText,          JAVALINUX.class_Notification_Builder, "setContentText",      "(Ljava/lang/CharSequence;)Llinux/app/Notification$Builder;" ) ;
		if( API_LV >= 16 ) GETMETHOD( JAVALINUX.methodID_Notification_Builder_setSubText,              JAVALINUX.class_Notification_Builder, "setSubText",          "(Ljava/lang/CharSequence;)Llinux/app/Notification$Builder;" ) ;
		                   GETMETHOD( JAVALINUX.methodID_Notification_Builder_setNumber,               JAVALINUX.class_Notification_Builder, "setNumber",           "(I)Llinux/app/Notification$Builder;" ) ;
		                   GETMETHOD( JAVALINUX.methodID_Notification_Builder_setContentInfo,          JAVALINUX.class_Notification_Builder, "setContentInfo",      "(Ljava/lang/CharSequence;)Llinux/app/Notification$Builder;" ) ;
		if( API_LV >= 14 ) GETMETHOD( JAVALINUX.methodID_Notification_Builder_setProgress,             JAVALINUX.class_Notification_Builder, "setProgress",         "(IIZ)Llinux/app/Notification$Builder;" ) ;
		                   GETMETHOD( JAVALINUX.methodID_Notification_Builder_setContent,              JAVALINUX.class_Notification_Builder, "setContent",          "(Llinux/widget/RemoteViews;)Llinux/app/Notification$Builder;" ) ;
		                   GETMETHOD( JAVALINUX.methodID_Notification_Builder_setContentIntent,        JAVALINUX.class_Notification_Builder, "setContentIntent",    "(Llinux/app/PendingIntent;)Llinux/app/Notification$Builder;" ) ;
		                   GETMETHOD( JAVALINUX.methodID_Notification_Builder_setDeleteIntent,         JAVALINUX.class_Notification_Builder, "setDeleteIntent",     "(Llinux/app/PendingIntent;)Llinux/app/Notification$Builder;" ) ;
		                   GETMETHOD( JAVALINUX.methodID_Notification_Builder_setFullScreenIntent,     JAVALINUX.class_Notification_Builder, "setFullScreenIntent", "(Llinux/app/PendingIntent;Z)Llinux/app/Notification$Builder;" ) ;
		                   GETMETHOD( JAVALINUX.methodID_Notification_Builder_setTicker,               JAVALINUX.class_Notification_Builder, "setTicker",           "(Ljava/lang/CharSequence;Llinux/widget/RemoteViews;)Llinux/app/Notification$Builder;" ) ;
		                   GETMETHOD( JAVALINUX.methodID_Notification_Builder_setLargeIcon,            JAVALINUX.class_Notification_Builder, "setLargeIcon",        "(Llinux/graphics/Bitmap;)Llinux/app/Notification$Builder;" ) ;
		                   GETMETHOD( JAVALINUX.methodID_Notification_Builder_setSound,                JAVALINUX.class_Notification_Builder, "setSound",            "(Llinux/net/Uri;I)Llinux/app/Notification$Builder;" ) ;
		                   GETMETHOD( JAVALINUX.methodID_Notification_Builder_setVibrate,              JAVALINUX.class_Notification_Builder, "setVibrate",          "([J)Llinux/app/Notification$Builder;" ) ;
		                   GETMETHOD( JAVALINUX.methodID_Notification_Builder_setLights,               JAVALINUX.class_Notification_Builder, "setLights",           "(III)Llinux/app/Notification$Builder;" ) ;
		                   GETMETHOD( JAVALINUX.methodID_Notification_Builder_setOngoing,              JAVALINUX.class_Notification_Builder, "setOngoing",          "(Z)Llinux/app/Notification$Builder;" ) ;
		                   GETMETHOD( JAVALINUX.methodID_Notification_Builder_setOnlyAlertOnce,        JAVALINUX.class_Notification_Builder, "setOnlyAlertOnce",    "(Z)Llinux/app/Notification$Builder;" ) ;
		                   GETMETHOD( JAVALINUX.methodID_Notification_Builder_setAutoCancel,           JAVALINUX.class_Notification_Builder, "setAutoCancel",       "(Z)Llinux/app/Notification$Builder;" ) ;
		if( API_LV >= 20 ) GETMETHOD( JAVALINUX.methodID_Notification_Builder_setLocalOnly,            JAVALINUX.class_Notification_Builder, "setLocalOnly",        "(Z)Llinux/app/Notification$Builder;" ) ;
		                   GETMETHOD( JAVALINUX.methodID_Notification_Builder_setDefaults,             JAVALINUX.class_Notification_Builder, "setDefaults",         "(I)Llinux/app/Notification$Builder;" ) ;
		if( API_LV >= 16 ) GETMETHOD( JAVALINUX.methodID_Notification_Builder_setPriority,             JAVALINUX.class_Notification_Builder, "setPriority",         "(I)Llinux/app/Notification$Builder;" ) ;
		if( API_LV >= 21 ) GETMETHOD( JAVALINUX.methodID_Notification_Builder_setCategory,             JAVALINUX.class_Notification_Builder, "setCategory",         "(Ljava/lang/String;)Llinux/app/Notification$Builder;" ) ;
		if( API_LV >= 21 ) GETMETHOD( JAVALINUX.methodID_Notification_Builder_addPerson,               JAVALINUX.class_Notification_Builder, "addPerson",           "(Ljava/lang/String;)Llinux/app/Notification$Builder;" ) ;
		if( API_LV >= 20 ) GETMETHOD( JAVALINUX.methodID_Notification_Builder_setGroup,                JAVALINUX.class_Notification_Builder, "setGroup",            "(Ljava/lang/String;)Llinux/app/Notification$Builder;" ) ;
		if( API_LV >= 20 ) GETMETHOD( JAVALINUX.methodID_Notification_Builder_setGroupSummary,         JAVALINUX.class_Notification_Builder, "setGroupSummary",     "(Z)Llinux/app/Notification$Builder;" ) ;
		if( API_LV >= 20 ) GETMETHOD( JAVALINUX.methodID_Notification_Builder_setSortKey,              JAVALINUX.class_Notification_Builder, "setSortKey",          "(Ljava/lang/String;)Llinux/app/Notification$Builder;" ) ;
		if( API_LV >= 20 ) GETMETHOD( JAVALINUX.methodID_Notification_Builder_addExtras,               JAVALINUX.class_Notification_Builder, "addExtras",           "(Llinux/os/Bundle;)Llinux/app/Notification$Builder;" ) ;
		if( API_LV >= 19 ) GETMETHOD( JAVALINUX.methodID_Notification_Builder_setExtras,               JAVALINUX.class_Notification_Builder, "setExtras",           "(Llinux/os/Bundle;)Llinux/app/Notification$Builder;" ) ;
		if( API_LV >= 21 ) GETMETHOD( JAVALINUX.methodID_Notification_Builder_getExtras,               JAVALINUX.class_Notification_Builder, "getExtras",           "()Llinux/os/Bundle;" ) ;
		if( API_LV >= 16 ) GETMETHOD( JAVALINUX.methodID_Notification_Builder_addAction,               JAVALINUX.class_Notification_Builder, "addAction",           "(ILjava/lang/CharSequence;Llinux/app/PendingIntent;)Llinux/app/Notification$Builder;" ) ;
		if( API_LV >= 22 ) GETMETHOD( JAVALINUX.methodID_Notification_Builder_setStyle,                JAVALINUX.class_Notification_Builder, "setStyle",            "(Llinux/app/Notification$Style;)Llinux/app/Notification$Builder;" ) ;
		if( API_LV >= 21 ) GETMETHOD( JAVALINUX.methodID_Notification_Builder_setVisibility,           JAVALINUX.class_Notification_Builder, "setVisibility",       "(I)Llinux/app/Notification$Builder;" ) ;
		if( API_LV >= 21 ) GETMETHOD( JAVALINUX.methodID_Notification_Builder_setPublicVersion,        JAVALINUX.class_Notification_Builder, "setPublicVersion",    "(Llinux/app/Notification;)Llinux/app/Notification$Builder;" ) ;
		if( API_LV >= 22 ) GETMETHOD( JAVALINUX.methodID_Notification_Builder_extend,                  JAVALINUX.class_Notification_Builder, "extend",              "(Llinux/app/Notification$Extender;)Llinux/app/Notification$Builder;" ) ;
		if( API_LV >= 21 ) GETMETHOD( JAVALINUX.methodID_Notification_Builder_setColor,                JAVALINUX.class_Notification_Builder, "setColor",            "(I)Llinux/app/Notification$Builder;" ) ;
		                   GETMETHOD( JAVALINUX.methodID_Notification_Builder_getNotification,         JAVALINUX.class_Notification_Builder, "getNotification",     "()Llinux/app/Notification;" ) ;
		if( API_LV >= 16 ) GETMETHOD( JAVALINUX.methodID_Notification_Builder_build,                   JAVALINUX.class_Notification_Builder, "build",               "()Llinux/app/Notification;" ) ;
	}

	FINDCLASS( JAVALINUX.class_Bitmap,               "linux/graphics/Bitmap"            ) ;
	FINDCLASS( JAVALINUX.class_BitmapConfig,         "linux/graphics/Bitmap$Config"     ) ;
	FINDCLASS( JAVALINUX.class_Canvas,               "linux/graphics/Canvas"            ) ;
	FINDCLASS( JAVALINUX.class_Typeface,             "linux/graphics/Typeface"          ) ;
	FINDCLASS( JAVALINUX.class_Rect,                 "linux/graphics/Rect"              ) ;
	FINDCLASS( JAVALINUX.class_Paint,                "linux/graphics/Paint"             ) ;
	FINDCLASS( JAVALINUX.class_PaintFontMetrics,     "linux/graphics/Paint$FontMetrics" ) ;
	FINDCLASS( JAVALINUX.class_Color,                "linux/graphics/Color"             ) ;

	if( API_LV >= 21 )
	{
		FINDCLASS( JAVALINUX.class_BaseBundle,       "linux/os/BaseBundle"              ) ;
	}
	FINDCLASS( JAVALINUX.class_Bundle,               "linux/os/Bundle"                  ) ;
	FINDCLASS( JAVALINUX.class_Debug_MemoryInfo,     "linux/os/Debug$MemoryInfo"        ) ;
	FINDCLASS( JAVALINUX.class_BatteryManager,       "linux/os/BatteryManager"          ) ;
	FINDCLASS( JAVALINUX.class_Environment,          "linux/os/Environment"             ) ;
	FINDCLASS( JAVALINUX.class_Vibrator,             "linux/os/Vibrator"                ) ;
	if( API_LV >= 26 )
	{
		FINDCLASS( JAVALINUX.class_VibrationEffect,  "linux/os/VibrationEffect"         ) ;
	}
	FINDCLASS( JAVALINUX.class_Process,              "linux/os/Process"                 ) ;
	FINDCLASS( JAVALINUX.class_View,                 "linux/view/View"                  ) ;
	if( API_LV >= 28 )
	{
		FINDCLASS( JAVALINUX.class_DisplayCutout,    "linux/view/DisplayCutout"         ) ;
	}
	FINDCLASS( JAVALINUX.class_Window,               "linux/view/Window"                ) ;
	FINDCLASS( JAVALINUX.class_TextView,             "linux/widget/TextView"            ) ;
	FINDCLASS( JAVALINUX.class_TextView_BufferType,  "linux/widget/TextView$BufferType" ) ;
	FINDCLASS( JAVALINUX.class_EditText,             "linux/widget/EditText"            ) ;
	FINDCLASS( JAVALINUX.class_InputStream,          "java/io/InputStream"                ) ;
	FINDCLASS( JAVALINUX.class_File,                 "java/io/File"                       ) ;
	FINDCLASS( JAVALINUX.class_CharSequence,         "java/lang/CharSequence"             ) ;
	FINDCLASS( JAVALINUX.class_Integer,              "java/lang/Integer"                  ) ;
	FINDCLASS( JAVALINUX.class_String,               "java/lang/String"                   ) ;
	FINDCLASS( JAVALINUX.class_Runtime,              "java/lang/Runtime"                  ) ;
	FINDCLASS( JAVALINUX.class_List,                 "java/util/List"                     ) ;
	FINDCLASS( JAVALINUX.class_Locale,               "java/util/Locale"                   ) ;
	FINDCLASS( JAVALINUX.class_Calendar,             "java/util/Calendar"                 ) ;
	FINDCLASS( JAVALINUX.class_URLConnection,        "java/net/URLConnection"             ) ;
	FINDCLASS( JAVALINUX.class_HttpURLConnection,    "java/net/HttpURLConnection"         ) ;
	FINDCLASS( JAVALINUX.class_URL,                  "java/net/URL"                       ) ;
	FINDCLASS( JAVALINUX.class_ActivityManager,      "linux/app/ActivityManager" ) ;
	FINDCLASS( JAVALINUX.class_ActivityManager_MemoryInfo,  "linux/app/ActivityManager$MemoryInfo" ) ;
	FINDCLASS( JAVALINUX.class_Dialog,               "linux/app/Dialog"                 ) ;
	FINDCLASS( JAVALINUX.class_AlertDialog,          "linux/app/AlertDialog"            ) ;
	FINDCLASS( JAVALINUX.class_AlertDialog_Builder,  "linux/app/AlertDialog$Builder"    ) ;
	FINDCLASS( JAVALINUX.class_Notification,         "linux/app/Notification"           ) ;
	FINDCLASS( JAVALINUX.class_NotificationManager,  "linux/app/NotificationManager"    ) ;
	FINDCLASS( JAVALINUX.class_PendingIntent,        "linux/app/PendingIntent"          ) ;
	FINDCLASS( JAVALINUX.class_Intent,               "linux/content/Intent"             ) ;
	FINDCLASS( JAVALINUX.class_IntentFilter,         "linux/content/IntentFilter"       ) ;
	FINDCLASS( JAVALINUX.class_BroadcastReceiver,    "linux/content/BroadcastReceiver"  ) ;
	FINDCLASS( JAVALINUX.class_ClipboardManager,     "linux/content/ClipboardManager"   ) ;
	FINDCLASS( JAVALINUX.class_ClipData,             "linux/content/ClipData"           ) ;
	FINDCLASS( JAVALINUX.class_Context,              "linux/content/Context"            ) ;
	FINDCLASS( JAVALINUX.class_Resources,            "linux/content/res/Resources"      ) ;
	FINDCLASS( JAVALINUX.class_ApplicationInfo,      "linux/content/pm/ApplicationInfo" ) ;
	FINDCLASS( JAVALINUX.class_PackageInfo,          "linux/content/pm/PackageInfo"     ) ;
	FINDCLASS( JAVALINUX.class_PackageItemInfo,      "linux/content/pm/PackageItemInfo" ) ;
	FINDCLASS( JAVALINUX.class_PackageManager,       "linux/content/pm/PackageManager"  ) ;
	FINDCLASS( JAVALINUX.class_SensorManager,        "linux/hardware/SensorManager"     ) ;
	FINDCLASS( JAVALINUX.class_R_mipmap,             "linux/R$mipmap"                   ) ;
	FINDCLASS( JAVALINUX.class_Uri,                  "linux/net/Uri"                    ) ;
	FINDCLASS( JAVALINUX.class_WifiInfo,             "linux/net/wifi/WifiInfo"          ) ;
	FINDCLASS( JAVALINUX.class_WifiManager,          "linux/net/wifi/WifiManager"       ) ;
	FINDCLASS( JAVALINUX.class_Activity,             "linux/app/Activity"               ) ;
	FINDCLASS( JAVALINUX.class_AudioManager,         "linux/media/AudioManager"         ) ;
	if( API_LV >= 20 )
	{
		FINDCLASS( JAVALINUX.class_WindowInsets,     "linux/view/WindowInsets"          ) ;
	}


	GETSTATICMETHOD( JAVALINUX.methodID_Bitmap_createBitmap                        , JAVALINUX.class_Bitmap,               "createBitmap",          "(IILlinux/graphics/Bitmap$Config;)Llinux/graphics/Bitmap;" ) ;
	GETMETHOD(       JAVALINUX.methodID_Bitmap_setPixels                           , JAVALINUX.class_Bitmap,               "setPixels",             "([IIIIIII)V" ) ;
	GETMETHOD(       JAVALINUX.methodID_Bitmap_getPixels                           , JAVALINUX.class_Bitmap,               "getPixels",             "([IIIIIII)V" ) ;
	GETMETHOD(       JAVALINUX.methodID_Canvas_newCanvas                           , JAVALINUX.class_Canvas,               "<init>",                "(Llinux/graphics/Bitmap;)V" ) ;
	GETMETHOD(       JAVALINUX.methodID_Canvas_drawText                            , JAVALINUX.class_Canvas,               "drawText",              "([CIIFFLlinux/graphics/Paint;)V" ) ;
	GETMETHOD(       JAVALINUX.methodID_Canvas_drawARGB                            , JAVALINUX.class_Canvas,               "drawARGB",              "(IIII)V" ) ;
	GETSTATICMETHOD( JAVALINUX.methodID_Typeface_create                            , JAVALINUX.class_Typeface,             "create",                "(Ljava/lang/String;I)Llinux/graphics/Typeface;" ) ;
	GETMETHOD(       JAVALINUX.methodID_Paint_newPaint                             , JAVALINUX.class_Paint,                "<init>",                "()V" ) ;
	GETMETHOD(       JAVALINUX.methodID_Paint_getFontMetrics                       , JAVALINUX.class_Paint,                "getFontMetrics",        "()Llinux/graphics/Paint$FontMetrics;" ) ;
	GETMETHOD(       JAVALINUX.methodID_Paint_setTextSize                          , JAVALINUX.class_Paint,                "setTextSize",           "(F)V" ) ;
	GETMETHOD(       JAVALINUX.methodID_Paint_setTypeface                          , JAVALINUX.class_Paint,                "setTypeface",           "(Llinux/graphics/Typeface;)Llinux/graphics/Typeface;" ) ;
	GETMETHOD(       JAVALINUX.methodID_Paint_setAntiAlias                         , JAVALINUX.class_Paint,                "setAntiAlias",          "(Z)V" ) ;
	GETMETHOD(       JAVALINUX.methodID_Paint_setARGB                              , JAVALINUX.class_Paint,                "setARGB",               "(IIII)V" ) ;
	GETMETHOD(       JAVALINUX.methodID_Paint_measureText                          , JAVALINUX.class_Paint,                "measureText",           "([CII)F" ) ;

	GETMETHOD(       JAVALINUX.methodID_EditText_newEditText                       , JAVALINUX.class_EditText,             "<init>",                "(Llinux/content/Context;)V" ) ;
	GETMETHOD(       JAVALINUX.methodID_EditText_setText                           , JAVALINUX.class_EditText,             "setText",               "(Ljava/lang/CharSequence;Llinux/widget/TextView$BufferType;)V" ) ;
	GETMETHOD(       JAVALINUX.methodID_TextView_getText                           , JAVALINUX.class_TextView,             "getText",               "()Ljava/lang/CharSequence;" ) ;
	if( API_LV >= 23 )
	{
		GETMETHOD(   JAVALINUX.methodID_View_getRootWindowInsets                   , JAVALINUX.class_View,                 "getRootWindowInsets",   "()Llinux/view/WindowInsets;" ) ;
	}
	if( API_LV >= 28 )
	{
		GETMETHOD(   JAVALINUX.methodID_DisplayCutout_getBoundingRects             , JAVALINUX.class_DisplayCutout,        "getBoundingRects",      "()Ljava/util/List;" ) ;
	}
	GETMETHOD(       JAVALINUX.methodID_Window_getCurrentFocus                     , JAVALINUX.class_Window,               "getCurrentFocus",       "()Llinux/view/View;" ) ;
	GETMETHOD(       JAVALINUX.methodID_Window_getDecorView                        , JAVALINUX.class_Window,               "getDecorView",          "()Llinux/view/View;" ) ;
	if( API_LV >= 28 )
	{
		GETMETHOD(   JAVALINUX.methodID_WindowInsets_getDisplayCutout              , JAVALINUX.class_WindowInsets,         "getDisplayCutout",      "()Llinux/view/DisplayCutout;" ) ;
	}
	if( API_LV >= 21 )
	{
		GETSTATICMETHOD( JAVALINUX.methodID_Environment_isExternalStorageRemovable , JAVALINUX.class_Environment,          "isExternalStorageRemovable",   "(Ljava/io/File;)Z" ) ;
	}
	GETMETHOD(       JAVALINUX.methodID_Vibrator_hasVibrator                       , JAVALINUX.class_Vibrator,             "hasVibrator",           "()Z" ) ;
	if( API_LV >= 26 )
	{
		GETMETHOD(   JAVALINUX.methodID_Vibrator_hasAmplitudeControl               , JAVALINUX.class_Vibrator,             "hasAmplitudeControl",   "()Z" ) ;
		GETMETHOD(   JAVALINUX.methodID_Vibrator_vibrate_vibEffect                 , JAVALINUX.class_Vibrator,             "vibrate",               "(Llinux/os/VibrationEffect;)V" ) ;
	}
	GETMETHOD(       JAVALINUX.methodID_Vibrator_vibrate_long                      , JAVALINUX.class_Vibrator,             "vibrate",               "(J)V" ) ;
	GETMETHOD(       JAVALINUX.methodID_Vibrator_vibrate_longArray_int             , JAVALINUX.class_Vibrator,             "vibrate",               "([JI)V" ) ;
	GETMETHOD(       JAVALINUX.methodID_Vibrator_cancel                            , JAVALINUX.class_Vibrator,             "cancel",                "()V" ) ;
	if( API_LV >= 26 )
	{
		GETSTATICMETHOD( JAVALINUX.methodID_VibrationEffect_createOneShot                  , JAVALINUX.class_VibrationEffect,   "createOneShot",    "(JI)Llinux/os/VibrationEffect;" ) ;
		GETSTATICMETHOD( JAVALINUX.methodID_VibrationEffect_createWaveform_withTimings     , JAVALINUX.class_VibrationEffect,   "createWaveform",   "([JI)Llinux/os/VibrationEffect;" ) ;
		GETSTATICMETHOD( JAVALINUX.methodID_VibrationEffect_createWaveform_withAmplitudes  , JAVALINUX.class_VibrationEffect,   "createWaveform",   "([J[II)Llinux/os/VibrationEffect;" ) ;
	}
	GETSTATICMETHOD( JAVALINUX.methodID_Process_myPid                              , JAVALINUX.class_Process,              "myPid",                 "()I" ) ;
	GETSTATICMETHOD( JAVALINUX.methodID_Process_killProcess                        , JAVALINUX.class_Process,              "killProcess",           "(I)V" ) ;
	GETMETHOD(       JAVALINUX.methodID_InputStream_read_0param                    , JAVALINUX.class_InputStream,          "read",                  "()I" ) ;
	GETMETHOD(       JAVALINUX.methodID_InputStream_read_1param                    , JAVALINUX.class_InputStream,          "read",                  "([B)I" ) ;
	GETMETHOD(       JAVALINUX.methodID_InputStream_close                          , JAVALINUX.class_InputStream,          "close",                 "()V" ) ;
	GETMETHOD(       JAVALINUX.methodID_File_getAbsolutePath                       , JAVALINUX.class_File,                 "getAbsolutePath",       "()Ljava/lang/String;" ) ;
	GETMETHOD(       JAVALINUX.methodID_CharSequence_toString                      , JAVALINUX.class_CharSequence,         "toString",              "()Ljava/lang/String;" ) ;
	GETSTATICMETHOD( JAVALINUX.methodID_Integer_parseInt                           , JAVALINUX.class_Integer,              "parseInt",              "(Ljava/lang/String;)I" ) ;
	GETMETHOD(       JAVALINUX.methodID_String_subSequence                         , JAVALINUX.class_String,               "subSequence",           "(II)Ljava/lang/CharSequence;" ) ;
	GETSTATICMETHOD( JAVALINUX.methodID_Runtime_getRuntime                         , JAVALINUX.class_Runtime,              "getRuntime",            "()Ljava/lang/Runtime;" ) ;
	GETMETHOD(       JAVALINUX.methodID_Runtime_freeMemory                         , JAVALINUX.class_Runtime,              "freeMemory",            "()J" ) ;
	GETMETHOD(       JAVALINUX.methodID_Runtime_totalMemory                        , JAVALINUX.class_Runtime,              "totalMemory",           "()J" ) ;
	GETMETHOD(       JAVALINUX.methodID_Runtime_maxMemory                          , JAVALINUX.class_Runtime,              "maxMemory",             "()J" ) ;
	GETMETHOD(       JAVALINUX.methodID_List_size                                  , JAVALINUX.class_List,                 "size",                  "()I" ) ;
	GETMETHOD(       JAVALINUX.methodID_List_get                                   , JAVALINUX.class_List,                 "get",                   "(I)Ljava/lang/Object;" ) ;
	GETSTATICMETHOD( JAVALINUX.methodID_Locale_getDefault                          , JAVALINUX.class_Locale,               "getDefault",            "()Ljava/util/Locale;" ) ;
	GETMETHOD(       JAVALINUX.methodID_Locale_getLanguage                         , JAVALINUX.class_Locale,               "getLanguage",           "()Ljava/lang/String;" ) ;
	GETMETHOD(       JAVALINUX.methodID_Locale_getCountry                          , JAVALINUX.class_Locale,               "getCountry",            "()Ljava/lang/String;" ) ;
	GETMETHOD(       JAVALINUX.methodID_Calendar_get                               , JAVALINUX.class_Calendar,             "get",                   "(I)I" ) ;
	GETSTATICMETHOD( JAVALINUX.methodID_Calendar_getInstance                       , JAVALINUX.class_Calendar,             "getInstance",           "()Ljava/util/Calendar;" ) ;
	GETMETHOD(       JAVALINUX.methodID_URLConnection_setDoInput                   , JAVALINUX.class_URLConnection,        "setDoInput",            "(Z)V" ) ;
	GETMETHOD(       JAVALINUX.methodID_URLConnection_setDoOutput                  , JAVALINUX.class_URLConnection,        "setDoOutput",           "(Z)V" ) ;
	GETMETHOD(       JAVALINUX.methodID_URLConnection_connect                      , JAVALINUX.class_URLConnection,        "connect",               "()V" ) ;
	GETMETHOD(       JAVALINUX.methodID_URLConnection_getInputStream               , JAVALINUX.class_URLConnection,        "getInputStream",        "()Ljava/io/InputStream;" ) ;
	GETMETHOD(       JAVALINUX.methodID_HttpURLConnection_newHttpURLConnection     , JAVALINUX.class_HttpURLConnection,    "<init>",                "(Ljava/net/URL;)V" ) ;
	GETMETHOD(       JAVALINUX.methodID_HttpURLConnection_setRequestMethod         , JAVALINUX.class_HttpURLConnection,    "setRequestMethod",      "(Ljava/lang/String;)V" ) ;
	GETMETHOD(       JAVALINUX.methodID_HttpURLConnection_setInstanceFollowRedirects, JAVALINUX.class_HttpURLConnection,   "setInstanceFollowRedirects", "(Z)V" ) ;
	GETMETHOD(       JAVALINUX.methodID_HttpURLConnection_getResponseCode          , JAVALINUX.class_HttpURLConnection,    "getResponseCode",       "()I" ) ;
	GETMETHOD(       JAVALINUX.methodID_URL_newURL                                 , JAVALINUX.class_URL,                  "<init>",                "(Ljava/lang/String;)V" ) ;
	GETMETHOD(       JAVALINUX.methodID_URL_openConnection                         , JAVALINUX.class_URL,                  "openConnection",        "()Ljava/net/URLConnection;" ) ;
	GETMETHOD(       JAVALINUX.methodID_ActivityManager_getMemoryInfo              , JAVALINUX.class_ActivityManager,      "getMemoryInfo",         "(Llinux/app/ActivityManager$MemoryInfo;)V" ) ;
	GETMETHOD(       JAVALINUX.methodID_ActivityManager_getProcessMemoryInfo       , JAVALINUX.class_ActivityManager,      "getProcessMemoryInfo",  "([I)[Llinux/os/Debug$MemoryInfo;" ) ;
	GETMETHOD(       JAVALINUX.methodID_ActivityManager_MemoryInfo_newActivityManager_MemoryInfo , JAVALINUX.class_ActivityManager_MemoryInfo, "<init>", "()V" ) ;
	GETMETHOD(       JAVALINUX.methodID_Dialog_isShowing                           , JAVALINUX.class_Dialog,               "isShowing",             "()Z" ) ;
	GETMETHOD(       JAVALINUX.methodID_AlertDialog_Builder_newAlertDialog_Builder , JAVALINUX.class_AlertDialog_Builder,  "<init>",                "(Llinux/content/Context;)V" ) ;
	GETMETHOD(       JAVALINUX.methodID_AlertDialog_Builder_setIcon                , JAVALINUX.class_AlertDialog_Builder,  "setIcon",               "(Llinux/graphics/drawable/Drawable;)Llinux/app/AlertDialog$Builder;" ) ;
	GETMETHOD(       JAVALINUX.methodID_AlertDialog_Builder_setTitle               , JAVALINUX.class_AlertDialog_Builder,  "setTitle",              "(Ljava/lang/CharSequence;)Llinux/app/AlertDialog$Builder;" ) ;
	GETMETHOD(       JAVALINUX.methodID_AlertDialog_Builder_setView                , JAVALINUX.class_AlertDialog_Builder,  "setView",               "(Llinux/view/View;)Llinux/app/AlertDialog$Builder;" ) ;
	GETMETHOD(       JAVALINUX.methodID_AlertDialog_Builder_setPositiveButton      , JAVALINUX.class_AlertDialog_Builder,  "setPositiveButton",     "(Ljava/lang/CharSequence;Llinux/content/DialogInterface$OnClickListener;)Llinux/app/AlertDialog$Builder;" ) ;
	GETMETHOD(       JAVALINUX.methodID_AlertDialog_Builder_setNegativeButton      , JAVALINUX.class_AlertDialog_Builder,  "setNegativeButton",     "(Ljava/lang/CharSequence;Llinux/content/DialogInterface$OnClickListener;)Llinux/app/AlertDialog$Builder;" ) ;
	GETMETHOD(       JAVALINUX.methodID_AlertDialog_Builder_show                   , JAVALINUX.class_AlertDialog_Builder,  "show",                  "()Llinux/app/AlertDialog;" ) ;
	GETMETHOD(       JAVALINUX.methodID_Notification_newNotification               , JAVALINUX.class_Notification,         "<init>",                "()V" ) ;
	if( API_LV < 11 )
	{
		GETMETHOD(   JAVALINUX.methodID_Notification_setLatestEventInfo            , JAVALINUX.class_Notification,         "setLatestEventInfo",    "(Llinux/content/Context;Ljava/lang/CharSequence;Ljava/lang/CharSequence;Llinux/app/PendingIntent;)V" ) ;
	}
	GETMETHOD(       JAVALINUX.methodID_NotificationManager_notify                 , JAVALINUX.class_NotificationManager,  "notify",                "(ILlinux/app/Notification;)V" ) ;
	GETMETHOD(       JAVALINUX.methodID_NotificationManager_cancel                 , JAVALINUX.class_NotificationManager,  "cancel",                "(I)V" ) ;
	GETMETHOD(       JAVALINUX.methodID_NotificationManager_cancelAll              , JAVALINUX.class_NotificationManager,  "cancelAll",             "()V" ) ;
	GETSTATICMETHOD( JAVALINUX.methodID_PendingIntent_getActivity                  , JAVALINUX.class_PendingIntent,        "getActivity",           "(Llinux/content/Context;ILlinux/content/Intent;I)Llinux/app/PendingIntent;" ) ;
	GETSTATICMETHOD( JAVALINUX.methodID_PendingIntent_getActivities                , JAVALINUX.class_PendingIntent,        "getActivities",         "(Llinux/content/Context;I[Llinux/content/Intent;I)Llinux/app/PendingIntent;" ) ;
	GETMETHOD(       JAVALINUX.methodID_Intent_newIntent                           , JAVALINUX.class_Intent,               "<init>",                "(Llinux/content/Context;Ljava/lang/Class;)V" ) ;
	GETMETHOD(       JAVALINUX.methodID_Intent_newIntent_NoParam                   , JAVALINUX.class_Intent,               "<init>",                "()V" ) ;
	GETMETHOD(       JAVALINUX.methodID_Intent_newIntent_Uri                       , JAVALINUX.class_Intent,               "<init>",                "(Ljava/lang/String;Llinux/net/Uri;)V" ) ;
	GETSTATICMETHOD( JAVALINUX.methodID_Intent_createChooser                       , JAVALINUX.class_Intent,               "createChooser",         "(Llinux/content/Intent;Ljava/lang/CharSequence;)Llinux/content/Intent;" ) ;
	GETMETHOD(       JAVALINUX.methodID_Intent_setFlags                            , JAVALINUX.class_Intent,               "setFlags",              "(I)Llinux/content/Intent;" ) ;
	GETMETHOD(       JAVALINUX.methodID_Intent_setClassName                        , JAVALINUX.class_Intent,               "setClassName",          "(Ljava/lang/String;Ljava/lang/String;)Llinux/content/Intent;" ) ;
	GETMETHOD(       JAVALINUX.methodID_Intent_getIntExtra                         , JAVALINUX.class_Intent,               "getIntExtra",           "(Ljava/lang/String;I)I" ) ;
	GETMETHOD(       JAVALINUX.methodID_Intent_setAction                           , JAVALINUX.class_Intent,               "setAction",             "(Ljava/lang/String;)Llinux/content/Intent;" ) ;
	GETMETHOD(       JAVALINUX.methodID_Intent_setData                             , JAVALINUX.class_Intent,               "setData",               "(Llinux/net/Uri;)Llinux/content/Intent;" ) ;
	GETMETHOD(       JAVALINUX.methodID_Intent_setType                             , JAVALINUX.class_Intent,               "setType",               "(Ljava/lang/String;)Llinux/content/Intent;" ) ;
	GETMETHOD(       JAVALINUX.methodID_Intent_addCategory                         , JAVALINUX.class_Intent,               "addCategory",           "(Ljava/lang/String;)Llinux/content/Intent;" ) ;
	GETMETHOD(       JAVALINUX.methodID_Intent_putExtra_boolean                    , JAVALINUX.class_Intent,               "putExtra",              "(Ljava/lang/String;Z)Llinux/content/Intent;" ) ;
	GETMETHOD(       JAVALINUX.methodID_Intent_putExtra_byte                       , JAVALINUX.class_Intent,               "putExtra",              "(Ljava/lang/String;B)Llinux/content/Intent;" ) ;
	GETMETHOD(       JAVALINUX.methodID_Intent_putExtra_char                       , JAVALINUX.class_Intent,               "putExtra",              "(Ljava/lang/String;C)Llinux/content/Intent;" ) ;
	GETMETHOD(       JAVALINUX.methodID_Intent_putExtra_short                      , JAVALINUX.class_Intent,               "putExtra",              "(Ljava/lang/String;S)Llinux/content/Intent;" ) ;
	GETMETHOD(       JAVALINUX.methodID_Intent_putExtra_int                        , JAVALINUX.class_Intent,               "putExtra",              "(Ljava/lang/String;I)Llinux/content/Intent;" ) ;
	GETMETHOD(       JAVALINUX.methodID_Intent_putExtra_long                       , JAVALINUX.class_Intent,               "putExtra",              "(Ljava/lang/String;J)Llinux/content/Intent;" ) ;
	GETMETHOD(       JAVALINUX.methodID_Intent_putExtra_float                      , JAVALINUX.class_Intent,               "putExtra",              "(Ljava/lang/String;F)Llinux/content/Intent;" ) ;
	GETMETHOD(       JAVALINUX.methodID_Intent_putExtra_double                     , JAVALINUX.class_Intent,               "putExtra",              "(Ljava/lang/String;D)Llinux/content/Intent;" ) ;
	GETMETHOD(       JAVALINUX.methodID_Intent_putExtra_string                     , JAVALINUX.class_Intent,               "putExtra",              "(Ljava/lang/String;Ljava/lang/String;)Llinux/content/Intent;" ) ;
	GETMETHOD(       JAVALINUX.methodID_Intent_putExtra_stringArray                , JAVALINUX.class_Intent,               "putExtra",              "(Ljava/lang/String;[Ljava/lang/String;)Llinux/content/Intent;" ) ;
	GETMETHOD(       JAVALINUX.methodID_IntentFilter_newIntentFilter               , JAVALINUX.class_IntentFilter,         "<init>",                "(Ljava/lang/String;)V" ) ;
	GETMETHOD(       JAVALINUX.methodID_ClipboardManager_setPrimaryClip            , JAVALINUX.class_ClipboardManager,     "setPrimaryClip",        "(Llinux/content/ClipData;)V" ) ;
	GETMETHOD(       JAVALINUX.methodID_ClipboardManager_getPrimaryClip            , JAVALINUX.class_ClipboardManager,     "getPrimaryClip",        "()Llinux/content/ClipData;" ) ;
	GETSTATICMETHOD( JAVALINUX.methodID_ClipData_newPlainText                      , JAVALINUX.class_ClipData,             "newPlainText",          "(Ljava/lang/CharSequence;Ljava/lang/CharSequence;)Llinux/content/ClipData;" ) ;
	GETMETHOD(       JAVALINUX.methodID_ClipData_toString                          , JAVALINUX.class_ClipData,             "toString",              "()Ljava/lang/String;" ) ;
	if( API_LV >= 21 )
	{
		GETMETHOD(   JAVALINUX.methodID_BaseBundle_getString                       , JAVALINUX.class_BaseBundle,           "getString",             "(Ljava/lang/String;)Ljava/lang/String;" ) ;
		GETMETHOD(   JAVALINUX.methodID_BaseBundle_getBoolean                      , JAVALINUX.class_BaseBundle,           "getBoolean",            "(Ljava/lang/String;)Z" ) ;
	}
	else
	{
		GETMETHOD(   JAVALINUX.methodID_Bundle_getString                           , JAVALINUX.class_Bundle,               "getString",             "(Ljava/lang/String;)Ljava/lang/String;" ) ;
		GETMETHOD(   JAVALINUX.methodID_Bundle_getBoolean                          , JAVALINUX.class_Bundle,               "getBoolean",            "(Ljava/lang/String;)Z" ) ;
	}
	GETMETHOD(       JAVALINUX.methodID_Debug_MemoryInfo_getTotalPss               , JAVALINUX.class_Debug_MemoryInfo,     "getTotalPss",            "()I" ) ;
	GETMETHOD(       JAVALINUX.methodID_Context_getResources                       , JAVALINUX.class_Context,              "getResources",          "()Llinux/content/res/Resources;" ) ;
	GETMETHOD(       JAVALINUX.methodID_Context_getPackageManager                  , JAVALINUX.class_Context,              "getPackageManager",     "()Llinux/content/pm/PackageManager;" ) ;
	GETMETHOD(       JAVALINUX.methodID_Context_getPackageName                     , JAVALINUX.class_Context,              "getPackageName",        "()Ljava/lang/String;" ) ;
	GETMETHOD(       JAVALINUX.methodID_Context_getApplicationContext              , JAVALINUX.class_Context,              "getApplicationContext", "()Llinux/content/Context;" ) ;
	if( API_LV >= 19 )
	{
		GETMETHOD(   JAVALINUX.methodID_Context_getExternalFilesDirs               , JAVALINUX.class_Context,              "getExternalFilesDirs",  "(Ljava/lang/String;)[Ljava/io/File;" ) ;
	}
	GETMETHOD(       JAVALINUX.methodID_Context_getSystemService                   , JAVALINUX.class_Context,              "getSystemService",      "(Ljava/lang/String;)Ljava/lang/Object;" ) ;
	GETMETHOD(       JAVALINUX.methodID_Context_registerReceiver                   , JAVALINUX.class_Context,              "registerReceiver",      "(Llinux/content/BroadcastReceiver;Llinux/content/IntentFilter;)Llinux/content/Intent;" ) ;
	GETMETHOD(       JAVALINUX.methodID_Resources_getString                        , JAVALINUX.class_Resources,            "getString",             "(I)Ljava/lang/String;" ) ;
	GETMETHOD(       JAVALINUX.methodID_Resources_getResourceName                  , JAVALINUX.class_Resources,            "getResourceName",       "(I)Ljava/lang/String;" ) ;
	GETMETHOD(       JAVALINUX.methodID_Resources_getIdentifier                    , JAVALINUX.class_Resources,            "getIdentifier",         "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)I" ) ;
	GETMETHOD(       JAVALINUX.methodID_PackageManager_getApplicationInfo          , JAVALINUX.class_PackageManager,       "getApplicationInfo",    "(Ljava/lang/String;I)Llinux/content/pm/ApplicationInfo;" ) ;
	GETMETHOD(       JAVALINUX.methodID_PackageManager_hasSystemFeature            , JAVALINUX.class_PackageManager,       "hasSystemFeature",      "(Ljava/lang/String;)Z" ) ;
	GETSTATICMETHOD( JAVALINUX.methodID_SensorManager_getRotationMatrix            , JAVALINUX.class_SensorManager,        "getRotationMatrix",     "([F[F[F[F)Z" ) ;
	GETSTATICMETHOD( JAVALINUX.methodID_SensorManager_remapCoordinateSystem        , JAVALINUX.class_SensorManager,        "remapCoordinateSystem", "([FII[F)Z" ) ;
	GETSTATICMETHOD( JAVALINUX.methodID_SensorManager_getOrientation               , JAVALINUX.class_SensorManager,        "getOrientation",        "([F[F)[F" ) ;
	GETSTATICMETHOD( JAVALINUX.methodID_Uri_parse                                  , JAVALINUX.class_Uri,                  "parse",                 "(Ljava/lang/String;)Llinux/net/Uri;" ) ;
	GETMETHOD(       JAVALINUX.methodID_WifiInfo_getSSID                           , JAVALINUX.class_WifiInfo,             "getSSID",               "()Ljava/lang/String;" ) ;
	GETMETHOD(       JAVALINUX.methodID_WifiInfo_getBSSID                          , JAVALINUX.class_WifiInfo,             "getBSSID",              "()Ljava/lang/String;" ) ;
	GETMETHOD(       JAVALINUX.methodID_WifiInfo_getRssi                           , JAVALINUX.class_WifiInfo,             "getRssi",               "()I" ) ;
	GETMETHOD(       JAVALINUX.methodID_WifiInfo_getLinkSpeed                      , JAVALINUX.class_WifiInfo,             "getLinkSpeed",          "()I" ) ;
	if( API_LV >= 21 )
	{
		GETMETHOD(   JAVALINUX.methodID_WifiInfo_getFrequency                      , JAVALINUX.class_WifiInfo,             "getFrequency",          "()I" ) ;
	}
	GETMETHOD(       JAVALINUX.methodID_WifiInfo_getMacAddress                     , JAVALINUX.class_WifiInfo,             "getMacAddress",         "()Ljava/lang/String;" ) ;
	GETMETHOD(       JAVALINUX.methodID_WifiInfo_getNetworkId                      , JAVALINUX.class_WifiInfo,             "getNetworkId",          "()I" ) ;
	GETMETHOD(       JAVALINUX.methodID_WifiInfo_getIpAddress                      , JAVALINUX.class_WifiInfo,             "getIpAddress",          "()I" ) ;
	GETMETHOD(       JAVALINUX.methodID_WifiInfo_getHiddenSSID                     , JAVALINUX.class_WifiInfo,             "getHiddenSSID",         "()Z" ) ;
	GETMETHOD(       JAVALINUX.methodID_WifiInfo_toString                          , JAVALINUX.class_WifiInfo,             "toString",              "()Ljava/lang/String;" ) ;
	GETMETHOD(       JAVALINUX.methodID_WifiInfo_describeContents                  , JAVALINUX.class_WifiInfo,             "describeContents",      "()I" ) ;
	GETSTATICMETHOD( JAVALINUX.methodID_WifiManager_calculateSignalLevel           , JAVALINUX.class_WifiManager,          "calculateSignalLevel",  "(II)I" ) ;
	GETMETHOD(       JAVALINUX.methodID_WifiManager_getConnectionInfo              , JAVALINUX.class_WifiManager,          "getConnectionInfo",     "()Llinux/net/wifi/WifiInfo;" ) ;
	GETMETHOD(       JAVALINUX.methodID_Activity_startActivity                     , JAVALINUX.class_Activity,             "startActivity",         "(Llinux/content/Intent;)V" ) ;
	GETMETHOD(       JAVALINUX.methodID_Activity_finish                            , JAVALINUX.class_Activity,             "finish",                "()V" ) ;
	if( API_LV >= 21 )
	{
		GETMETHOD(   JAVALINUX.methodID_Activity_finishAndRemoveTask               , JAVALINUX.class_Activity,             "finishAndRemoveTask",   "()V" ) ;
		GETMETHOD(   JAVALINUX.methodID_Activity_releaseInstance                   , JAVALINUX.class_Activity,             "releaseInstance",       "()Z" ) ;
	}
	GETMETHOD(       JAVALINUX.methodID_Activity_getCurrentFocus                   , JAVALINUX.class_Activity,             "getCurrentFocus",       "()Llinux/view/View;" ) ;
	GETMETHOD(       JAVALINUX.methodID_Activity_getWindow                         , JAVALINUX.class_Activity,             "getWindow",             "()Llinux/view/Window;" ) ;
	if( API_LV >= 17 )
	{
		GETMETHOD(   JAVALINUX.methodID_AudioManager_getProperty                   , JAVALINUX.class_AudioManager,         "getProperty",           "(Ljava/lang/String;)Ljava/lang/String;" ) ;
	}


	GETSTATICFIELD( JAVALINUX.fieldID_BitmapConfig_ARGB_8888                     , JAVALINUX.class_BitmapConfig,            "ARGB_8888",                           "Llinux/graphics/Bitmap$Config;" ) ;
	GETFIELD(       JAVALINUX.fieldID_PaintFontMetrics_ascent                    , JAVALINUX.class_PaintFontMetrics,        "ascent",                              "F" ) ;
	GETFIELD(       JAVALINUX.fieldID_PaintFontMetrics_bottom                    , JAVALINUX.class_PaintFontMetrics,        "bottom",                              "F" ) ;
	GETFIELD(       JAVALINUX.fieldID_PaintFontMetrics_descent                   , JAVALINUX.class_PaintFontMetrics,        "descent",                             "F" ) ;
	GETFIELD(       JAVALINUX.fieldID_PaintFontMetrics_leading                   , JAVALINUX.class_PaintFontMetrics,        "leading",                             "F" ) ;
	GETFIELD(       JAVALINUX.fieldID_PaintFontMetrics_top                       , JAVALINUX.class_PaintFontMetrics,        "top",                                 "F" ) ;
	GETSTATICFIELD( JAVALINUX.fieldID_Typeface_NORMAL                            , JAVALINUX.class_Typeface,                "NORMAL",                              "I" ) ;
	GETSTATICFIELD( JAVALINUX.fieldID_Typeface_BOLD                              , JAVALINUX.class_Typeface,                "BOLD",                                "I" ) ;
	GETSTATICFIELD( JAVALINUX.fieldID_Typeface_ITALIC                            , JAVALINUX.class_Typeface,                "ITALIC",                              "I" ) ;
	GETSTATICFIELD( JAVALINUX.fieldID_Typeface_BOLD_ITALIC                       , JAVALINUX.class_Typeface,                "BOLD_ITALIC",                         "I" ) ;
	GETSTATICFIELD( JAVALINUX.fieldID_Color_BLACK                                , JAVALINUX.class_Color,                   "BLACK",                               "I" ) ;
	GETSTATICFIELD( JAVALINUX.fieldID_Color_BLUE                                 , JAVALINUX.class_Color,                   "BLUE",                                "I" ) ;
	GETSTATICFIELD( JAVALINUX.fieldID_Color_CYAN                                 , JAVALINUX.class_Color,                   "CYAN",                                "I" ) ;
	GETSTATICFIELD( JAVALINUX.fieldID_Color_DKGRAY                               , JAVALINUX.class_Color,                   "DKGRAY",                              "I" ) ;
	GETSTATICFIELD( JAVALINUX.fieldID_Color_GRAY                                 , JAVALINUX.class_Color,                   "GRAY",                                "I" ) ;
	GETSTATICFIELD( JAVALINUX.fieldID_Color_GREEN                                , JAVALINUX.class_Color,                   "GREEN",                               "I" ) ;
	GETSTATICFIELD( JAVALINUX.fieldID_Color_LTGRAY                               , JAVALINUX.class_Color,                   "LTGRAY",                              "I" ) ;
	GETSTATICFIELD( JAVALINUX.fieldID_Color_MAGENTA                              , JAVALINUX.class_Color,                   "MAGENTA",                             "I" ) ;
	GETSTATICFIELD( JAVALINUX.fieldID_Color_RED                                  , JAVALINUX.class_Color,                   "RED",                                 "I" ) ;
	GETSTATICFIELD( JAVALINUX.fieldID_Color_TRANSPARENT                          , JAVALINUX.class_Color,                   "TRANSPARENT",                         "I" ) ;
	GETSTATICFIELD( JAVALINUX.fieldID_Color_WHITE                                , JAVALINUX.class_Color,                   "WHITE",                               "I" ) ;
	GETSTATICFIELD( JAVALINUX.fieldID_Color_YELLOW                               , JAVALINUX.class_Color,                   "YELLOW",                              "I" ) ;
	GETFIELD(       JAVALINUX.fieldID_Rect_bottom                                , JAVALINUX.class_Rect,                    "bottom",                              "I" ) ;
	GETFIELD(       JAVALINUX.fieldID_Rect_left                                  , JAVALINUX.class_Rect,                    "left",                                "I" ) ;
	GETFIELD(       JAVALINUX.fieldID_Rect_right                                 , JAVALINUX.class_Rect,                    "right",                               "I" ) ;
	GETFIELD(       JAVALINUX.fieldID_Rect_top                                   , JAVALINUX.class_Rect,                    "top",                                 "I" ) ;

	GETSTATICFIELD( JAVALINUX.fieldID_BatteryManager_EXTRA_LEVEL                 , JAVALINUX.class_BatteryManager,          "EXTRA_LEVEL",                         "Ljava/lang/String;" ) ;
	GETSTATICFIELD( JAVALINUX.fieldID_BatteryManager_EXTRA_SCALE                 , JAVALINUX.class_BatteryManager,          "EXTRA_SCALE",                         "Ljava/lang/String;" ) ;

	GETSTATICFIELD( JAVALINUX.fieldID_TextView_BufferType_EDITABLE               , JAVALINUX.class_TextView_BufferType,     "EDITABLE",                            "Llinux/widget/TextView$BufferType;" ) ;
	GETSTATICFIELD( JAVALINUX.fieldID_TextView_BufferType_NORMAL                 , JAVALINUX.class_TextView_BufferType,     "NORMAL",                              "Llinux/widget/TextView$BufferType;" ) ;
	GETSTATICFIELD( JAVALINUX.fieldID_TextView_BufferType_SPANNABLE              , JAVALINUX.class_TextView_BufferType,     "SPANNABLE",                           "Llinux/widget/TextView$BufferType;" ) ;

	GETFIELD(       JAVALINUX.fieldID_ActivityManager_MemoryInfo_availMem        , JAVALINUX.class_ActivityManager_MemoryInfo, "availMem",                         "J" ) ;
	GETFIELD(       JAVALINUX.fieldID_ActivityManager_MemoryInfo_lowMemory       , JAVALINUX.class_ActivityManager_MemoryInfo, "lowMemory",                        "Z" ) ;
	GETFIELD(       JAVALINUX.fieldID_ActivityManager_MemoryInfo_threshold       , JAVALINUX.class_ActivityManager_MemoryInfo, "threshold",                        "J" ) ;
	if( API_LV >= 16 )
	{
		GETFIELD(   JAVALINUX.fieldID_ActivityManager_MemoryInfo_totalMem        , JAVALINUX.class_ActivityManager_MemoryInfo, "totalMem",                         "J" ) ;
	}

	GETSTATICFIELD( JAVALINUX.fieldID_Notification_DEFAULT_LIGHTS                , JAVALINUX.class_Notification,            "DEFAULT_LIGHTS",                      "I" ) ;
	GETSTATICFIELD( JAVALINUX.fieldID_Notification_DEFAULT_VIBRATE               , JAVALINUX.class_Notification,            "DEFAULT_VIBRATE",                     "I" ) ;
	GETSTATICFIELD( JAVALINUX.fieldID_Notification_FLAG_SHOW_LIGHTS              , JAVALINUX.class_Notification,            "FLAG_SHOW_LIGHTS",                    "I" ) ;
	GETFIELD(       JAVALINUX.fieldID_Notification_icon                          , JAVALINUX.class_Notification,            "icon",                                "I" ) ;
	GETFIELD(       JAVALINUX.fieldID_Notification_tickerText                    , JAVALINUX.class_Notification,            "tickerText",                          "Ljava/lang/CharSequence;" ) ;
	GETFIELD(       JAVALINUX.fieldID_Notification_defaults                      , JAVALINUX.class_Notification,            "defaults",                            "I" ) ;
	GETFIELD(       JAVALINUX.fieldID_Notification_vibrate                       , JAVALINUX.class_Notification,            "vibrate",                             "[J" ) ;
	GETFIELD(       JAVALINUX.fieldID_Notification_ledARGB                       , JAVALINUX.class_Notification,            "ledARGB",                             "I" ) ;
	GETFIELD(       JAVALINUX.fieldID_Notification_ledOffMS                      , JAVALINUX.class_Notification,            "ledOffMS",                            "I" ) ;
	GETFIELD(       JAVALINUX.fieldID_Notification_ledOnMS                       , JAVALINUX.class_Notification,            "ledOnMS",                             "I" ) ;
	GETFIELD(       JAVALINUX.fieldID_Notification_flags                         , JAVALINUX.class_Notification,            "flags",                               "I" ) ;
	GETSTATICFIELD( JAVALINUX.fieldID_PendingIntent_FLAG_CANCEL_CURRENT	        , JAVALINUX.class_PendingIntent,           "FLAG_CANCEL_CURRENT",                 "I" ) ;
	GETSTATICFIELD( JAVALINUX.fieldID_PendingIntent_FLAG_NO_CREATE               , JAVALINUX.class_PendingIntent,           "FLAG_NO_CREATE",                      "I" ) ;
	GETSTATICFIELD( JAVALINUX.fieldID_PendingIntent_FLAG_ONE_SHOT                , JAVALINUX.class_PendingIntent,           "FLAG_ONE_SHOT",                       "I" ) ;
	GETSTATICFIELD( JAVALINUX.fieldID_PendingIntent_FLAG_UPDATE_CURRENT          , JAVALINUX.class_PendingIntent,           "FLAG_UPDATE_CURRENT",                 "I" ) ;
	GETSTATICFIELD( JAVALINUX.fieldID_Intent_ACTION_BATTERY_CHANGED              , JAVALINUX.class_Intent,                  "ACTION_BATTERY_CHANGED",              "Ljava/lang/String;" ) ;
	GETSTATICFIELD( JAVALINUX.fieldID_Intent_ACTION_OPEN_DOCUMENT                , JAVALINUX.class_Intent,                  "ACTION_OPEN_DOCUMENT",                "Ljava/lang/String;" ) ;
	GETSTATICFIELD( JAVALINUX.fieldID_Intent_ACTION_SEND                         , JAVALINUX.class_Intent,                  "ACTION_SEND",                         "Ljava/lang/String;" ) ;
	GETSTATICFIELD( JAVALINUX.fieldID_Intent_ACTION_SENDTO                       , JAVALINUX.class_Intent,                  "ACTION_SENDTO",                       "Ljava/lang/String;" ) ;
	GETSTATICFIELD( JAVALINUX.fieldID_Intent_ACTION_VIEW                         , JAVALINUX.class_Intent,                  "ACTION_VIEW",                         "Ljava/lang/String;" ) ;
	GETSTATICFIELD( JAVALINUX.fieldID_Intent_EXTRA_EMAIL                         , JAVALINUX.class_Intent,                  "EXTRA_EMAIL",                         "Ljava/lang/String;" ) ;
	GETSTATICFIELD( JAVALINUX.fieldID_Intent_EXTRA_STREAM                        , JAVALINUX.class_Intent,                  "EXTRA_STREAM",                        "Ljava/lang/String;" ) ;
	GETSTATICFIELD( JAVALINUX.fieldID_Intent_EXTRA_SUBJECT                       , JAVALINUX.class_Intent,                  "EXTRA_SUBJECT",                       "Ljava/lang/String;" ) ;
	GETSTATICFIELD( JAVALINUX.fieldID_Intent_EXTRA_TEXT                          , JAVALINUX.class_Intent,                  "EXTRA_TEXT",                          "Ljava/lang/String;" ) ;
	GETSTATICFIELD( JAVALINUX.fieldID_Intent_EXTRA_CC                            , JAVALINUX.class_Intent,                  "EXTRA_CC",                            "Ljava/lang/String;" ) ;
	GETSTATICFIELD( JAVALINUX.fieldID_Intent_EXTRA_BCC                           , JAVALINUX.class_Intent,                  "EXTRA_BCC",                           "Ljava/lang/String;" ) ;
	GETSTATICFIELD( JAVALINUX.fieldID_Intent_CATEGORY_OPENABLE                   , JAVALINUX.class_Intent,                  "CATEGORY_OPENABLE",                   "Ljava/lang/String;" ) ;
	GETSTATICFIELD( JAVALINUX.fieldID_Intent_FLAG_ACTIVITY_NEW_TASK              , JAVALINUX.class_Intent,                  "FLAG_ACTIVITY_NEW_TASK",              "I" ) ;
	GETSTATICFIELD( JAVALINUX.fieldID_Intent_FLAG_ACTIVITY_SINGLE_TOP            , JAVALINUX.class_Intent,                  "FLAG_ACTIVITY_SINGLE_TOP",            "I" ) ;
	GETSTATICFIELD( JAVALINUX.fieldID_Intent_FLAG_ACTIVITY_CLEAR_TOP             , JAVALINUX.class_Intent,                  "FLAG_ACTIVITY_CLEAR_TOP",             "I" ) ;
	if( API_LV >= 11 )
	{
		GETSTATICFIELD( JAVALINUX.fieldID_Intent_FLAG_ACTIVITY_CLEAR_TASK            , JAVALINUX.class_Intent,                  "FLAG_ACTIVITY_CLEAR_TASK",            "I" ) ;
	}
	GETSTATICFIELD( JAVALINUX.fieldID_Intent_FLAG_ACTIVITY_NO_ANIMATION          , JAVALINUX.class_Intent,                  "FLAG_ACTIVITY_NO_ANIMATION",          "I" ) ;
	GETSTATICFIELD( JAVALINUX.fieldID_Intent_FLAG_ACTIVITY_NO_HISTORY            , JAVALINUX.class_Intent,                  "FLAG_ACTIVITY_NO_HISTORY",            "I" ) ;
	GETSTATICFIELD( JAVALINUX.fieldID_Intent_FLAG_ACTIVITY_LAUNCHED_FROM_HISTORY , JAVALINUX.class_Intent,                  "FLAG_ACTIVITY_LAUNCHED_FROM_HISTORY", "I" ) ;
	GETSTATICFIELD( JAVALINUX.fieldID_Intent_FLAG_ACTIVITY_EXCLUDE_FROM_RECENTS  , JAVALINUX.class_Intent,                  "FLAG_ACTIVITY_EXCLUDE_FROM_RECENTS",  "I" ) ;
	GETSTATICFIELD( JAVALINUX.fieldID_Intent_FLAG_ACTIVITY_PREVIOUS_IS_TOP       , JAVALINUX.class_Intent,                  "FLAG_ACTIVITY_PREVIOUS_IS_TOP",       "I" ) ;
	GETSTATICFIELD( JAVALINUX.fieldID_Intent_FLAG_ACTIVITY_REORDER_TO_FRONT      , JAVALINUX.class_Intent,                  "FLAG_ACTIVITY_REORDER_TO_FRONT",      "I" ) ;
	GETSTATICFIELD( JAVALINUX.fieldID_Context_NOTIFICATION_SERVICE               , JAVALINUX.class_Context,                 "NOTIFICATION_SERVICE",                "Ljava/lang/String;" ) ;
	GETSTATICFIELD( JAVALINUX.fieldID_Context_AUDIO_SERVICE                      , JAVALINUX.class_Context,                 "AUDIO_SERVICE",                       "Ljava/lang/String;" ) ;
	GETSTATICFIELD( JAVALINUX.fieldID_Context_ACTIVITY_SERVICE                   , JAVALINUX.class_Context,                 "ACTIVITY_SERVICE",                    "Ljava/lang/String;" ) ;
	GETSTATICFIELD( JAVALINUX.fieldID_Context_VIBRATOR_SERVICE                   , JAVALINUX.class_Context,                 "VIBRATOR_SERVICE",                    "Ljava/lang/String;" ) ;
	GETSTATICFIELD( JAVALINUX.fieldID_Context_SENSOR_SERVICE                     , JAVALINUX.class_Context,                 "SENSOR_SERVICE",                      "Ljava/lang/String;" ) ;
	GETSTATICFIELD( JAVALINUX.fieldID_Context_WIFI_SERVICE                       , JAVALINUX.class_Context,                 "WIFI_SERVICE",                        "Ljava/lang/String;" ) ;
	if( API_LV >= 18 )
	{
		GETSTATICFIELD( JAVALINUX.fieldID_Context_BLUETOOTH_SERVICE                  , JAVALINUX.class_Context,                 "BLUETOOTH_SERVICE",                   "Ljava/lang/String;" ) ;
	}
	if( API_LV >= 21 )
	{
		GETSTATICFIELD( JAVALINUX.fieldID_Context_CAMERA_SERVICE                     , JAVALINUX.class_Context,                 "CAMERA_SERVICE",                      "Ljava/lang/String;" ) ;
	}
	if( API_LV >= 19 )
	{
		GETSTATICFIELD( JAVALINUX.fieldID_Context_CAPTIONING_SERVICE                 , JAVALINUX.class_Context,                 "CAPTIONING_SERVICE",                  "Ljava/lang/String;" ) ;
	}
	GETSTATICFIELD( JAVALINUX.fieldID_Context_CLIPBOARD_SERVICE                  , JAVALINUX.class_Context,                 "CLIPBOARD_SERVICE",                   "Ljava/lang/String;" ) ;
	GETSTATICFIELD( JAVALINUX.fieldID_Context_CONNECTIVITY_SERVICE               , JAVALINUX.class_Context,                 "CONNECTIVITY_SERVICE",                "Ljava/lang/String;" ) ;
	if( API_LV >= 19 )
	{
		GETSTATICFIELD( JAVALINUX.fieldID_Context_CONSUMER_IR_SERVICE                , JAVALINUX.class_Context,                 "CONSUMER_IR_SERVICE",                 "Ljava/lang/String;" ) ;
	}
	GETFIELD(       JAVALINUX.fieldID_PackageItemInfo_metaData                   , JAVALINUX.class_PackageItemInfo,         "metaData",                            "Llinux/os/Bundle;" ) ;
	GETSTATICFIELD( JAVALINUX.fieldID_PackageManager_GET_META_DATA               , JAVALINUX.class_PackageManager,          "GET_META_DATA",                       "I" ) ;
	GETSTATICFIELD( JAVALINUX.fieldID_SensorManager_AXIS_MINUS_X                 , JAVALINUX.class_SensorManager,           "AXIS_MINUS_X",                        "I" ) ;
	GETSTATICFIELD( JAVALINUX.fieldID_SensorManager_AXIS_MINUS_Y                 , JAVALINUX.class_SensorManager,           "AXIS_MINUS_Y",                        "I" ) ;
	GETSTATICFIELD( JAVALINUX.fieldID_SensorManager_AXIS_MINUS_Z                 , JAVALINUX.class_SensorManager,           "AXIS_MINUS_Z",                        "I" ) ;
	GETSTATICFIELD( JAVALINUX.fieldID_SensorManager_AXIS_X                       , JAVALINUX.class_SensorManager,           "AXIS_X",                              "I" ) ;
	GETSTATICFIELD( JAVALINUX.fieldID_SensorManager_AXIS_Y                       , JAVALINUX.class_SensorManager,           "AXIS_Y",                              "I" ) ;
	GETSTATICFIELD( JAVALINUX.fieldID_SensorManager_AXIS_Z                       , JAVALINUX.class_SensorManager,           "AXIS_Z",                              "I" ) ;
	if( API_LV >= 26 )
	{
		GETSTATICFIELD( JAVALINUX.fieldID_VibrationEffect_DEFAULT_AMPLITUDE      , JAVALINUX.class_VibrationEffect,         "DEFAULT_AMPLITUDE",                   "I" ) ;
	}
	GETSTATICFIELD( JAVALINUX.fieldID_R_mipmap_sym_def_app_icon                  , JAVALINUX.class_R_mipmap,                "sym_def_app_icon",                    "I" ) ;
	GETSTATICFIELD( JAVALINUX.fieldID_Calendar_DAY_OF_WEEK                       , JAVALINUX.class_Calendar,                "DAY_OF_WEEK",                         "I" ) ;
	GETSTATICFIELD( JAVALINUX.fieldID_Calendar_WEEK_OF_MONTH                     , JAVALINUX.class_Calendar,                "WEEK_OF_MONTH",                       "I" ) ;
	GETSTATICFIELD( JAVALINUX.fieldID_Calendar_DAY_OF_WEEK_IN_MONTH              , JAVALINUX.class_Calendar,                "DAY_OF_WEEK_IN_MONTH",                "I" ) ;
	GETSTATICFIELD( JAVALINUX.fieldID_HttpURLConnection_HTTP_OK                  , JAVALINUX.class_HttpURLConnection,       "HTTP_OK",                             "I" ) ;
	GETSTATICFIELD( JAVALINUX.fieldID_HttpURLConnection_HTTP_NO_CONTENT          , JAVALINUX.class_HttpURLConnection,       "HTTP_NO_CONTENT",                     "I" ) ;
	GETSTATICFIELD( JAVALINUX.fieldID_HttpURLConnection_HTTP_NOT_FOUND           , JAVALINUX.class_HttpURLConnection,       "HTTP_NOT_FOUND",                      "I" ) ;
	GETSTATICFIELD( JAVALINUX.fieldID_HttpURLConnection_HTTP_BAD_METHOD          , JAVALINUX.class_HttpURLConnection,       "HTTP_BAD_METHOD",                     "I" ) ;
	GETSTATICFIELD( JAVALINUX.fieldID_HttpURLConnection_HTTP_NOT_ACCEPTABLE      , JAVALINUX.class_HttpURLConnection,       "HTTP_NOT_ACCEPTABLE",                 "I" ) ;
	GETSTATICFIELD( JAVALINUX.fieldID_HttpURLConnection_HTTP_BAD_GATEWAY         , JAVALINUX.class_HttpURLConnection,       "HTTP_BAD_GATEWAY",                    "I" ) ;
	GETSTATICFIELD( JAVALINUX.fieldID_HttpURLConnection_HTTP_NOT_IMPLEMENTED     , JAVALINUX.class_HttpURLConnection,       "HTTP_NOT_IMPLEMENTED",                "I" ) ;
	GETSTATICFIELD( JAVALINUX.fieldID_HttpURLConnection_HTTP_SERVER_ERROR        , JAVALINUX.class_HttpURLConnection,       "HTTP_SERVER_ERROR",                   "I" ) ;
	GETSTATICFIELD( JAVALINUX.fieldID_HttpURLConnection_HTTP_GATEWAY_TIMEOUT     , JAVALINUX.class_HttpURLConnection,       "HTTP_GATEWAY_TIMEOUT",                "I" ) ;
	GETSTATICFIELD( JAVALINUX.fieldID_HttpURLConnection_HTTP_UNAVAILABLE         , JAVALINUX.class_HttpURLConnection,       "HTTP_UNAVAILABLE",                    "I" ) ;
	GETSTATICFIELD( JAVALINUX.fieldID_HttpURLConnection_HTTP_UNSUPPORTED_TYPE    , JAVALINUX.class_HttpURLConnection,       "HTTP_UNSUPPORTED_TYPE",               "I" ) ;
	GETSTATICFIELD( JAVALINUX.fieldID_HttpURLConnection_HTTP_PRECON_FAILED       , JAVALINUX.class_HttpURLConnection,       "HTTP_PRECON_FAILED",                  "I" ) ;
	GETSTATICFIELD( JAVALINUX.fieldID_HttpURLConnection_HTTP_CONFLICT            , JAVALINUX.class_HttpURLConnection,       "HTTP_CONFLICT",                       "I" ) ;
	GETSTATICFIELD( JAVALINUX.fieldID_HttpURLConnection_HTTP_UNAUTHORIZED        , JAVALINUX.class_HttpURLConnection,       "HTTP_UNAUTHORIZED",                   "I" ) ;
	GETSTATICFIELD( JAVALINUX.fieldID_HttpURLConnection_HTTP_BAD_REQUEST         , JAVALINUX.class_HttpURLConnection,       "HTTP_BAD_REQUEST",                    "I" ) ;
	if( API_LV >= 17 )
	{
		GETSTATICFIELD( JAVALINUX.fieldID_AudioManager_PROPERTY_OUTPUT_FRAMES_PER_BUFFER , JAVALINUX.class_AudioManager,    "PROPERTY_OUTPUT_FRAMES_PER_BUFFER",   "Ljava/lang/String;" ) ;
		GETSTATICFIELD( JAVALINUX.fieldID_AudioManager_PROPERTY_OUTPUT_SAMPLE_RATE       , JAVALINUX.class_AudioManager,    "PROPERTY_OUTPUT_SAMPLE_RATE",         "Ljava/lang/String;" ) ;
	}
	


	GETSTATICOBJECTFIELD( JAVALINUX.fieldobject_BitmapConfig_ARGB_8888,        JAVALINUX.class_BitmapConfig,        JAVALINUX.fieldID_BitmapConfig_ARGB_8888, "BitmapConfig_ARGB_8888" ) ;
	JAVALINUX.fieldint_Typeface_NORMAL           = env->GetStaticIntField(     JAVALINUX.class_Typeface,            JAVALINUX.fieldID_Typeface_NORMAL      ) ;
	JAVALINUX.fieldint_Typeface_BOLD             = env->GetStaticIntField(     JAVALINUX.class_Typeface,            JAVALINUX.fieldID_Typeface_BOLD        ) ;
	JAVALINUX.fieldint_Typeface_ITALIC           = env->GetStaticIntField(     JAVALINUX.class_Typeface,            JAVALINUX.fieldID_Typeface_ITALIC      ) ;
	JAVALINUX.fieldint_Typeface_BOLD_ITALIC      = env->GetStaticIntField(     JAVALINUX.class_Typeface,            JAVALINUX.fieldID_Typeface_BOLD_ITALIC ) ;
	JAVALINUX.fieldint_Color_BLACK               = env->GetStaticIntField(     JAVALINUX.class_Color,               JAVALINUX.fieldID_Color_BLACK          ) ;
	JAVALINUX.fieldint_Color_BLUE                = env->GetStaticIntField(     JAVALINUX.class_Color,               JAVALINUX.fieldID_Color_BLUE           ) ;
	JAVALINUX.fieldint_Color_CYAN                = env->GetStaticIntField(     JAVALINUX.class_Color,               JAVALINUX.fieldID_Color_CYAN           ) ;
	JAVALINUX.fieldint_Color_DKGRAY              = env->GetStaticIntField(     JAVALINUX.class_Color,               JAVALINUX.fieldID_Color_DKGRAY         ) ;
	JAVALINUX.fieldint_Color_GRAY                = env->GetStaticIntField(     JAVALINUX.class_Color,               JAVALINUX.fieldID_Color_GRAY           ) ;
	JAVALINUX.fieldint_Color_GREEN               = env->GetStaticIntField(     JAVALINUX.class_Color,               JAVALINUX.fieldID_Color_GREEN          ) ;
	JAVALINUX.fieldint_Color_LTGRAY              = env->GetStaticIntField(     JAVALINUX.class_Color,               JAVALINUX.fieldID_Color_LTGRAY         ) ;
	JAVALINUX.fieldint_Color_MAGENTA             = env->GetStaticIntField(     JAVALINUX.class_Color,               JAVALINUX.fieldID_Color_MAGENTA        ) ;
	JAVALINUX.fieldint_Color_RED                 = env->GetStaticIntField(     JAVALINUX.class_Color,               JAVALINUX.fieldID_Color_RED            ) ;
	JAVALINUX.fieldint_Color_TRANSPARENT         = env->GetStaticIntField(     JAVALINUX.class_Color,               JAVALINUX.fieldID_Color_TRANSPARENT    ) ;
	JAVALINUX.fieldint_Color_WHITE               = env->GetStaticIntField(     JAVALINUX.class_Color,               JAVALINUX.fieldID_Color_WHITE          ) ;
	JAVALINUX.fieldint_Color_YELLOW              = env->GetStaticIntField(     JAVALINUX.class_Color,               JAVALINUX.fieldID_Color_YELLOW         ) ;

	GETSTATICSTRINGFIELD( JAVALINUX.fieldstring_BatteryManager_EXTRA_LEVEL,    JAVALINUX.class_BatteryManager,      JAVALINUX.fieldID_BatteryManager_EXTRA_LEVEL, "Ljava/lang/String;" ) ;
	GETSTATICSTRINGFIELD( JAVALINUX.fieldstring_BatteryManager_EXTRA_SCALE,    JAVALINUX.class_BatteryManager,      JAVALINUX.fieldID_BatteryManager_EXTRA_SCALE, "Ljava/lang/String;" ) ;

	GETSTATICOBJECTFIELD( JAVALINUX.fieldobject_TextView_BufferType_EDITABLE,  JAVALINUX.class_TextView_BufferType, JAVALINUX.fieldID_TextView_BufferType_EDITABLE,  "TextView_BufferType_EDITABLE"  ) ;
	GETSTATICOBJECTFIELD( JAVALINUX.fieldobject_TextView_BufferType_NORMAL,    JAVALINUX.class_TextView_BufferType, JAVALINUX.fieldID_TextView_BufferType_NORMAL,    "TextView_BufferType_NORMAL"    ) ;
	GETSTATICOBJECTFIELD( JAVALINUX.fieldobject_TextView_BufferType_SPANNABLE, JAVALINUX.class_TextView_BufferType, JAVALINUX.fieldID_TextView_BufferType_SPANNABLE, "TextView_BufferType_SPANNABLE" ) ;

	JAVALINUX.fieldint_Notification_DEFAULT_LIGHTS                = env->GetStaticIntField( JAVALINUX.class_Notification,        JAVALINUX.fieldID_Notification_DEFAULT_LIGHTS   ) ;
	JAVALINUX.fieldint_Notification_DEFAULT_VIBRATE               = env->GetStaticIntField( JAVALINUX.class_Notification,        JAVALINUX.fieldID_Notification_DEFAULT_VIBRATE  ) ;
	JAVALINUX.fieldint_Notification_FLAG_SHOW_LIGHTS              = env->GetStaticIntField( JAVALINUX.class_Notification,        JAVALINUX.fieldID_Notification_FLAG_SHOW_LIGHTS ) ;
	JAVALINUX.fieldint_PendingIntent_FLAG_CANCEL_CURRENT          = env->GetStaticIntField( JAVALINUX.class_PendingIntent,       JAVALINUX.fieldID_PendingIntent_FLAG_CANCEL_CURRENT ) ;
	JAVALINUX.fieldint_PendingIntent_FLAG_NO_CREATE               = env->GetStaticIntField( JAVALINUX.class_PendingIntent,       JAVALINUX.fieldID_PendingIntent_FLAG_NO_CREATE      ) ;
	JAVALINUX.fieldint_PendingIntent_FLAG_ONE_SHOT                = env->GetStaticIntField( JAVALINUX.class_PendingIntent,       JAVALINUX.fieldID_PendingIntent_FLAG_ONE_SHOT       ) ;
	JAVALINUX.fieldint_PendingIntent_FLAG_UPDATE_CURRENT          = env->GetStaticIntField( JAVALINUX.class_PendingIntent,       JAVALINUX.fieldID_PendingIntent_FLAG_UPDATE_CURRENT ) ;
	GETSTATICSTRINGFIELD( JAVALINUX.fieldstring_Intent_ACTION_BATTERY_CHANGED,   JAVALINUX.class_Intent,              JAVALINUX.fieldID_Intent_ACTION_BATTERY_CHANGED, "Ljava/lang/String;" ) ;
	GETSTATICSTRINGFIELD( JAVALINUX.fieldstring_Intent_ACTION_OPEN_DOCUMENT,     JAVALINUX.class_Intent,              JAVALINUX.fieldID_Intent_ACTION_OPEN_DOCUMENT,   "Ljava/lang/String;" ) ;
	GETSTATICSTRINGFIELD( JAVALINUX.fieldstring_Intent_ACTION_SEND,              JAVALINUX.class_Intent,              JAVALINUX.fieldID_Intent_ACTION_SEND,            "Ljava/lang/String;" ) ;
	GETSTATICSTRINGFIELD( JAVALINUX.fieldstring_Intent_ACTION_SENDTO,            JAVALINUX.class_Intent,              JAVALINUX.fieldID_Intent_ACTION_SENDTO,          "Ljava/lang/String;" ) ;
	GETSTATICSTRINGFIELD( JAVALINUX.fieldstring_Intent_ACTION_VIEW,              JAVALINUX.class_Intent,              JAVALINUX.fieldID_Intent_ACTION_VIEW,            "Ljava/lang/String;" ) ;
	GETSTATICSTRINGFIELD( JAVALINUX.fieldstring_Intent_EXTRA_EMAIL,              JAVALINUX.class_Intent,              JAVALINUX.fieldID_Intent_EXTRA_EMAIL,            "Ljava/lang/String;" ) ;
	GETSTATICSTRINGFIELD( JAVALINUX.fieldstring_Intent_EXTRA_STREAM,             JAVALINUX.class_Intent,              JAVALINUX.fieldID_Intent_EXTRA_STREAM,           "Ljava/lang/String;" ) ;
	GETSTATICSTRINGFIELD( JAVALINUX.fieldstring_Intent_EXTRA_SUBJECT,            JAVALINUX.class_Intent,              JAVALINUX.fieldID_Intent_EXTRA_SUBJECT,          "Ljava/lang/String;" ) ;
	GETSTATICSTRINGFIELD( JAVALINUX.fieldstring_Intent_EXTRA_TEXT,               JAVALINUX.class_Intent,              JAVALINUX.fieldID_Intent_EXTRA_TEXT,             "Ljava/lang/String;" ) ;
	GETSTATICSTRINGFIELD( JAVALINUX.fieldstring_Intent_EXTRA_CC,                 JAVALINUX.class_Intent,              JAVALINUX.fieldID_Intent_EXTRA_CC,               "Ljava/lang/String;" ) ;
	GETSTATICSTRINGFIELD( JAVALINUX.fieldstring_Intent_EXTRA_BCC,                JAVALINUX.class_Intent,              JAVALINUX.fieldID_Intent_EXTRA_BCC,              "Ljava/lang/String;" ) ;
	GETSTATICSTRINGFIELD( JAVALINUX.fieldstring_Intent_CATEGORY_OPENABLE,        JAVALINUX.class_Intent,              JAVALINUX.fieldID_Intent_CATEGORY_OPENABLE,      "Ljava/lang/String;" ) ;
	JAVALINUX.fieldint_Intent_FLAG_ACTIVITY_NEW_TASK              = env->GetStaticIntField( JAVALINUX.class_Intent,              JAVALINUX.fieldID_Intent_FLAG_ACTIVITY_NEW_TASK     ) ;
	JAVALINUX.fieldint_Intent_FLAG_ACTIVITY_SINGLE_TOP            = env->GetStaticIntField( JAVALINUX.class_Intent,              JAVALINUX.fieldID_Intent_FLAG_ACTIVITY_SINGLE_TOP   ) ;
	JAVALINUX.fieldint_Intent_FLAG_ACTIVITY_CLEAR_TOP             = env->GetStaticIntField( JAVALINUX.class_Intent,              JAVALINUX.fieldID_Intent_FLAG_ACTIVITY_CLEAR_TOP    ) ;
	if( API_LV >= 11 )
	{
		JAVALINUX.fieldint_Intent_FLAG_ACTIVITY_CLEAR_TASK        = env->GetStaticIntField( JAVALINUX.class_Intent,              JAVALINUX.fieldID_Intent_FLAG_ACTIVITY_CLEAR_TASK            ) ;
	}
	JAVALINUX.fieldint_Intent_FLAG_ACTIVITY_NO_ANIMATION          = env->GetStaticIntField( JAVALINUX.class_Intent,              JAVALINUX.fieldID_Intent_FLAG_ACTIVITY_NO_ANIMATION          ) ;
	JAVALINUX.fieldint_Intent_FLAG_ACTIVITY_NO_HISTORY            = env->GetStaticIntField( JAVALINUX.class_Intent,              JAVALINUX.fieldID_Intent_FLAG_ACTIVITY_NO_HISTORY            ) ;
	JAVALINUX.fieldint_Intent_FLAG_ACTIVITY_LAUNCHED_FROM_HISTORY = env->GetStaticIntField( JAVALINUX.class_Intent,              JAVALINUX.fieldID_Intent_FLAG_ACTIVITY_LAUNCHED_FROM_HISTORY ) ;
	JAVALINUX.fieldint_Intent_FLAG_ACTIVITY_EXCLUDE_FROM_RECENTS  = env->GetStaticIntField( JAVALINUX.class_Intent,              JAVALINUX.fieldID_Intent_FLAG_ACTIVITY_EXCLUDE_FROM_RECENTS  ) ;
	JAVALINUX.fieldint_Intent_FLAG_ACTIVITY_PREVIOUS_IS_TOP       = env->GetStaticIntField( JAVALINUX.class_Intent,              JAVALINUX.fieldID_Intent_FLAG_ACTIVITY_PREVIOUS_IS_TOP       ) ;
	JAVALINUX.fieldint_Intent_FLAG_ACTIVITY_REORDER_TO_FRONT      = env->GetStaticIntField( JAVALINUX.class_Intent,              JAVALINUX.fieldID_Intent_FLAG_ACTIVITY_REORDER_TO_FRONT      ) ;
	GETSTATICOBJECTFIELD( JAVALINUX.fieldobject_Context_NOTIFICATION_SERVICE,    JAVALINUX.class_Context,             JAVALINUX.fieldID_Context_NOTIFICATION_SERVICE,  "Context_NOTIFICATION_SERVICE" ) ;
	GETSTATICOBJECTFIELD( JAVALINUX.fieldobject_Context_AUDIO_SERVICE,           JAVALINUX.class_Context,             JAVALINUX.fieldID_Context_AUDIO_SERVICE,         "Context_AUDIO_SERVICE" ) ;
	GETSTATICOBJECTFIELD( JAVALINUX.fieldobject_Context_ACTIVITY_SERVICE,        JAVALINUX.class_Context,             JAVALINUX.fieldID_Context_ACTIVITY_SERVICE,      "Context_ACTIVITY_SERVICE" ) ;
	GETSTATICOBJECTFIELD( JAVALINUX.fieldobject_Context_VIBRATOR_SERVICE,        JAVALINUX.class_Context,             JAVALINUX.fieldID_Context_VIBRATOR_SERVICE,      "Context_VIBRATOR_SERVICE"     ) ;
	GETSTATICOBJECTFIELD( JAVALINUX.fieldobject_Context_SENSOR_SERVICE,          JAVALINUX.class_Context,             JAVALINUX.fieldID_Context_SENSOR_SERVICE,        "Context_SENSOR_SERVICE"       ) ;
	GETSTATICOBJECTFIELD( JAVALINUX.fieldobject_Context_WIFI_SERVICE,            JAVALINUX.class_Context,             JAVALINUX.fieldID_Context_WIFI_SERVICE,          "Context_WIFI_SERVICE"         ) ;
	if( API_LV >= 18 )
	{
		GETSTATICOBJECTFIELD( JAVALINUX.fieldobject_Context_BLUETOOTH_SERVICE,       JAVALINUX.class_Context,             JAVALINUX.fieldID_Context_BLUETOOTH_SERVICE,     "Context_BLUETOOTH_SERVICE"    ) ;
	}
	if( API_LV >= 21 )
	{
		GETSTATICOBJECTFIELD( JAVALINUX.fieldobject_Context_CAMERA_SERVICE,          JAVALINUX.class_Context,             JAVALINUX.fieldID_Context_CAMERA_SERVICE,        "Context_CAMERA_SERVICE"       ) ;
	}
	if( API_LV >= 19 )
	{
		GETSTATICOBJECTFIELD( JAVALINUX.fieldobject_Context_CAPTIONING_SERVICE,      JAVALINUX.class_Context,             JAVALINUX.fieldID_Context_CAPTIONING_SERVICE,    "Context_CAPTIONING_SERVICE"   ) ;
	}
	GETSTATICOBJECTFIELD( JAVALINUX.fieldobject_Context_CLIPBOARD_SERVICE,       JAVALINUX.class_Context,             JAVALINUX.fieldID_Context_CLIPBOARD_SERVICE,     "Context_CLIPBOARD_SERVICE"    ) ;
	GETSTATICOBJECTFIELD( JAVALINUX.fieldobject_Context_CONNECTIVITY_SERVICE,    JAVALINUX.class_Context,             JAVALINUX.fieldID_Context_CONNECTIVITY_SERVICE,  "Context_CONNECTIVITY_SERVICE" ) ;
	if( API_LV >= 19 )
	{
		GETSTATICOBJECTFIELD( JAVALINUX.fieldobject_Context_CONSUMER_IR_SERVICE,     JAVALINUX.class_Context,             JAVALINUX.fieldID_Context_CONSUMER_IR_SERVICE,   "Context_CONSUMER_IR_SERVICE"  ) ;
	}
	JAVALINUX.fieldint_PackageManager_GET_META_DATA               = env->GetStaticIntField( JAVALINUX.class_PackageManager,      JAVALINUX.fieldID_PackageManager_GET_META_DATA ) ;
	JAVALINUX.fieldint_SensorManager_AXIS_MINUS_X                 = env->GetStaticIntField( JAVALINUX.class_SensorManager,       JAVALINUX.fieldID_SensorManager_AXIS_MINUS_X   ) ;
	JAVALINUX.fieldint_SensorManager_AXIS_MINUS_Y                 = env->GetStaticIntField( JAVALINUX.class_SensorManager,       JAVALINUX.fieldID_SensorManager_AXIS_MINUS_Y   ) ;
	JAVALINUX.fieldint_SensorManager_AXIS_MINUS_Z                 = env->GetStaticIntField( JAVALINUX.class_SensorManager,       JAVALINUX.fieldID_SensorManager_AXIS_MINUS_Z   ) ;
	JAVALINUX.fieldint_SensorManager_AXIS_X                       = env->GetStaticIntField( JAVALINUX.class_SensorManager,       JAVALINUX.fieldID_SensorManager_AXIS_X         ) ;
	JAVALINUX.fieldint_SensorManager_AXIS_Y                       = env->GetStaticIntField( JAVALINUX.class_SensorManager,       JAVALINUX.fieldID_SensorManager_AXIS_Y         ) ;
	JAVALINUX.fieldint_SensorManager_AXIS_Z                       = env->GetStaticIntField( JAVALINUX.class_SensorManager,       JAVALINUX.fieldID_SensorManager_AXIS_Z         ) ;
	if( API_LV >= 26 )
	{
		JAVALINUX.fieldint_VibrationEffect_DEFAULT_AMPLITUDE      = env->GetStaticIntField( JAVALINUX.class_VibrationEffect,     JAVALINUX.fieldID_VibrationEffect_DEFAULT_AMPLITUDE ) ;
	}
	JAVALINUX.fieldint_R_mipmap_sym_def_app_icon                  = env->GetStaticIntField( JAVALINUX.class_R_mipmap,            JAVALINUX.fieldID_R_mipmap_sym_def_app_icon  ) ;
	JAVALINUX.fieldint_Calendar_DAY_OF_WEEK                       = env->GetStaticIntField( JAVALINUX.class_Calendar,            JAVALINUX.fieldID_Calendar_DAY_OF_WEEK          ) ;
	JAVALINUX.fieldint_Calendar_WEEK_OF_MONTH                     = env->GetStaticIntField( JAVALINUX.class_Calendar,            JAVALINUX.fieldID_Calendar_WEEK_OF_MONTH        ) ;
	JAVALINUX.fieldint_Calendar_DAY_OF_WEEK_IN_MONTH              = env->GetStaticIntField( JAVALINUX.class_Calendar,            JAVALINUX.fieldID_Calendar_DAY_OF_WEEK_IN_MONTH ) ;
	JAVALINUX.fieldint_HttpURLConnection_HTTP_OK                  = env->GetStaticIntField( JAVALINUX.class_HttpURLConnection,   JAVALINUX.fieldID_HttpURLConnection_HTTP_OK               ) ;
	JAVALINUX.fieldint_HttpURLConnection_HTTP_NO_CONTENT          = env->GetStaticIntField( JAVALINUX.class_HttpURLConnection,   JAVALINUX.fieldID_HttpURLConnection_HTTP_NO_CONTENT       ) ;
	JAVALINUX.fieldint_HttpURLConnection_HTTP_NOT_FOUND           = env->GetStaticIntField( JAVALINUX.class_HttpURLConnection,   JAVALINUX.fieldID_HttpURLConnection_HTTP_NOT_FOUND        ) ;
	JAVALINUX.fieldint_HttpURLConnection_HTTP_BAD_METHOD          = env->GetStaticIntField( JAVALINUX.class_HttpURLConnection,   JAVALINUX.fieldID_HttpURLConnection_HTTP_BAD_METHOD       ) ;
	JAVALINUX.fieldint_HttpURLConnection_HTTP_NOT_ACCEPTABLE      = env->GetStaticIntField( JAVALINUX.class_HttpURLConnection,   JAVALINUX.fieldID_HttpURLConnection_HTTP_NOT_ACCEPTABLE   ) ;
	JAVALINUX.fieldint_HttpURLConnection_HTTP_BAD_GATEWAY         = env->GetStaticIntField( JAVALINUX.class_HttpURLConnection,   JAVALINUX.fieldID_HttpURLConnection_HTTP_BAD_GATEWAY      ) ;
	JAVALINUX.fieldint_HttpURLConnection_HTTP_NOT_IMPLEMENTED     = env->GetStaticIntField( JAVALINUX.class_HttpURLConnection,   JAVALINUX.fieldID_HttpURLConnection_HTTP_NOT_IMPLEMENTED  ) ;
	JAVALINUX.fieldint_HttpURLConnection_HTTP_SERVER_ERROR        = env->GetStaticIntField( JAVALINUX.class_HttpURLConnection,   JAVALINUX.fieldID_HttpURLConnection_HTTP_SERVER_ERROR     ) ;
	JAVALINUX.fieldint_HttpURLConnection_HTTP_GATEWAY_TIMEOUT     = env->GetStaticIntField( JAVALINUX.class_HttpURLConnection,   JAVALINUX.fieldID_HttpURLConnection_HTTP_GATEWAY_TIMEOUT  ) ;
	JAVALINUX.fieldint_HttpURLConnection_HTTP_UNAVAILABLE         = env->GetStaticIntField( JAVALINUX.class_HttpURLConnection,   JAVALINUX.fieldID_HttpURLConnection_HTTP_UNAVAILABLE      ) ;
	JAVALINUX.fieldint_HttpURLConnection_HTTP_UNSUPPORTED_TYPE    = env->GetStaticIntField( JAVALINUX.class_HttpURLConnection,   JAVALINUX.fieldID_HttpURLConnection_HTTP_UNSUPPORTED_TYPE ) ;
	JAVALINUX.fieldint_HttpURLConnection_HTTP_PRECON_FAILED       = env->GetStaticIntField( JAVALINUX.class_HttpURLConnection,   JAVALINUX.fieldID_HttpURLConnection_HTTP_PRECON_FAILED    ) ;
	JAVALINUX.fieldint_HttpURLConnection_HTTP_CONFLICT            = env->GetStaticIntField( JAVALINUX.class_HttpURLConnection,   JAVALINUX.fieldID_HttpURLConnection_HTTP_CONFLICT         ) ;
	JAVALINUX.fieldint_HttpURLConnection_HTTP_UNAUTHORIZED        = env->GetStaticIntField( JAVALINUX.class_HttpURLConnection,   JAVALINUX.fieldID_HttpURLConnection_HTTP_UNAUTHORIZED     ) ;
	JAVALINUX.fieldint_HttpURLConnection_HTTP_BAD_REQUEST         = env->GetStaticIntField( JAVALINUX.class_HttpURLConnection,   JAVALINUX.fieldID_HttpURLConnection_HTTP_BAD_REQUEST      ) ;
	if( API_LV >= 17 )
	{
		GETSTATICSTRINGFIELD( JAVALINUX.fieldstring_AudioManager_PROPERTY_OUTPUT_FRAMES_PER_BUFFER,JAVALINUX.class_AudioManager, JAVALINUX.fieldID_AudioManager_PROPERTY_OUTPUT_FRAMES_PER_BUFFER, "Ljava/lang/String;" ) ;
		GETSTATICSTRINGFIELD( JAVALINUX.fieldstring_AudioManager_PROPERTY_OUTPUT_SAMPLE_RATE,      JAVALINUX.class_AudioManager, JAVALINUX.fieldID_AudioManager_PROPERTY_OUTPUT_SAMPLE_RATE,       "Ljava/lang/String;" ) ;
	}


	JAVALINUX.InitializeFlag = TRUE ;

	// 正常終了
	return 0 ;
}
#undef FINDCLASS
#undef GETMETHOD
#undef GETSTATICMETHOD
#undef GETFIELD
#undef GETSTATICFIELD

// Java のクラスやメソッドの参照の後始末をする
#define JAVA_DELETE_GLOBAL_REF( object )\
	if( object != NULL )\
	{\
		env->DeleteGlobalRef( object ) ;\
		object = NULL ;\
	}
extern int TerminateJavaLinuxInfo( JNIEnv *env )
{
	if( JAVALINUX.InitializeFlag == FALSE )
	{
		return -1 ;
	}

	JAVA_DELETE_GLOBAL_REF( JAVALINUX.class_Build_VERSION ) ;
	JAVA_DELETE_GLOBAL_REF( JAVALINUX.class_Bitmap ) ;
	JAVA_DELETE_GLOBAL_REF( JAVALINUX.class_BitmapConfig ) ;
	JAVA_DELETE_GLOBAL_REF( JAVALINUX.class_Canvas ) ;
	JAVA_DELETE_GLOBAL_REF( JAVALINUX.class_Typeface ) ;
	JAVA_DELETE_GLOBAL_REF( JAVALINUX.class_Paint ) ;
	JAVA_DELETE_GLOBAL_REF( JAVALINUX.class_PaintFontMetrics ) ;
	JAVA_DELETE_GLOBAL_REF( JAVALINUX.class_Color ) ;
	JAVA_DELETE_GLOBAL_REF( JAVALINUX.class_BaseBundle ) ;
	JAVA_DELETE_GLOBAL_REF( JAVALINUX.class_Bundle ) ;
	JAVA_DELETE_GLOBAL_REF( JAVALINUX.class_BatteryManager ) ;
	JAVA_DELETE_GLOBAL_REF( JAVALINUX.class_Environment ) ;
	JAVA_DELETE_GLOBAL_REF( JAVALINUX.class_Vibrator ) ;
	JAVA_DELETE_GLOBAL_REF( JAVALINUX.class_VibrationEffect ) ;
	JAVA_DELETE_GLOBAL_REF( JAVALINUX.class_Process ) ;
	JAVA_DELETE_GLOBAL_REF( JAVALINUX.class_TextView ) ;
	JAVA_DELETE_GLOBAL_REF( JAVALINUX.class_TextView_BufferType ) ;
	JAVA_DELETE_GLOBAL_REF( JAVALINUX.class_EditText ) ;
	JAVA_DELETE_GLOBAL_REF( JAVALINUX.class_Integer ) ;
	JAVA_DELETE_GLOBAL_REF( JAVALINUX.class_String ) ;
	JAVA_DELETE_GLOBAL_REF( JAVALINUX.class_Runtime ) ;
	JAVA_DELETE_GLOBAL_REF( JAVALINUX.class_Locale ) ;
	JAVA_DELETE_GLOBAL_REF( JAVALINUX.class_File ) ;
	JAVA_DELETE_GLOBAL_REF( JAVALINUX.class_CharSequence ) ;
	JAVA_DELETE_GLOBAL_REF( JAVALINUX.class_ActivityManager ) ;
	JAVA_DELETE_GLOBAL_REF( JAVALINUX.class_ActivityManager_MemoryInfo ) ;
	JAVA_DELETE_GLOBAL_REF( JAVALINUX.class_Dialog ) ;
	JAVA_DELETE_GLOBAL_REF( JAVALINUX.class_AlertDialog ) ;
	JAVA_DELETE_GLOBAL_REF( JAVALINUX.class_AlertDialog_Builder ) ;
	JAVA_DELETE_GLOBAL_REF( JAVALINUX.class_Notification ) ;
	JAVA_DELETE_GLOBAL_REF( JAVALINUX.class_NotificationManager ) ;
	JAVA_DELETE_GLOBAL_REF( JAVALINUX.class_Notification_Builder ) ;
	JAVA_DELETE_GLOBAL_REF( JAVALINUX.class_PendingIntent ) ;
	JAVA_DELETE_GLOBAL_REF( JAVALINUX.class_Intent ) ;
	JAVA_DELETE_GLOBAL_REF( JAVALINUX.class_IntentFilter ) ;
	JAVA_DELETE_GLOBAL_REF( JAVALINUX.class_BroadcastReceiver ) ;
	JAVA_DELETE_GLOBAL_REF( JAVALINUX.class_Context ) ;
	JAVA_DELETE_GLOBAL_REF( JAVALINUX.class_Resources ) ;
	JAVA_DELETE_GLOBAL_REF( JAVALINUX.class_PackageInfo ) ;
	JAVA_DELETE_GLOBAL_REF( JAVALINUX.class_PackageItemInfo ) ;
	JAVA_DELETE_GLOBAL_REF( JAVALINUX.class_ApplicationInfo ) ;
	JAVA_DELETE_GLOBAL_REF( JAVALINUX.class_PackageManager ) ;
	JAVA_DELETE_GLOBAL_REF( JAVALINUX.class_SensorManager ) ;
	JAVA_DELETE_GLOBAL_REF( JAVALINUX.class_R_mipmap ) ;
	JAVA_DELETE_GLOBAL_REF( JAVALINUX.class_Uri ) ;
	JAVA_DELETE_GLOBAL_REF( JAVALINUX.class_Activity ) ;
	JAVA_DELETE_GLOBAL_REF( JAVALINUX.class_AudioManager ) ;

	JAVA_DELETE_GLOBAL_REF( JAVALINUX.fieldobject_BitmapConfig_ARGB_8888 ) ;
	JAVA_DELETE_GLOBAL_REF( JAVALINUX.fieldobject_TextView_BufferType_EDITABLE ) ;
	JAVA_DELETE_GLOBAL_REF( JAVALINUX.fieldobject_TextView_BufferType_NORMAL ) ;
	JAVA_DELETE_GLOBAL_REF( JAVALINUX.fieldobject_TextView_BufferType_SPANNABLE ) ;
	JAVA_DELETE_GLOBAL_REF( JAVALINUX.fieldobject_Context_NOTIFICATION_SERVICE ) ;
	JAVA_DELETE_GLOBAL_REF( JAVALINUX.fieldobject_Context_ACTIVITY_SERVICE ) ;

	JAVALINUX.InitializeFlag = FALSE ;

	return 0 ;
}
#undef JAVA_DELETE_GLOBAL_REF

#ifndef DX_NON_NAMESPACE

}

#endif // DX_NON_NAMESPACE
