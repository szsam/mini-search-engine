#include "http_demo_Search.h"
#include "mvector.h"
#include <iostream>
#include <string>
#include <sstream>
#include <unordered_map>
using namespace std;

JNIEXPORT void JNICALL Java_http_demo_Search_initData
(JNIEnv *, jobject){
	// TODO: initialization
	return;
}

unordered_map<string, mvector<int>> umap;

mvector<int>& getIndex(string key) {
	return umap[key];
}

struct Result {
	string url;
	int index;
	int pagerank;
};

int setResults(Result *&results,	// output
		string *keyArr, int len); 

JNIEXPORT jobjectArray JNICALL Java_http_demo_Search_searchResults
(JNIEnv *env, jobject jobj, jobjectArray jstrArr){
	jint len = env->GetArrayLength(jstrArr);	
	string *keyArr = new string[len];
	for (jint i = 0; i < len; i ++) {
		jstring jstr = (jstring)env->GetObjectArrayElement(jstrArr, i);
		const char *key = env->GetStringUTFChars(jstr, 0);
		keyArr[i] = key;
	}
	Result *results;
	int resLen = setResults(results, keyArr, len);
	jclass jresult = env->FindClass("http/demo/Result");
	jobjectArray jresults = env->NewObjectArray(resLen, jresult, 0);
	jmethodID newID = env->GetMethodID(jresult, "<init>", "(Ljava/lang/String;II)V");
	//jmethodID newID = env->GetMethodID(jresult, "<init>", "(Ljava/lang/String;I)V");
	//jmethodID newID = env->GetMethodID(jresult, "<init>", "(II)V");
	for (int i = 0; i < resLen; i ++) {
		jstring jstr = env->NewStringUTF(results[i].url.c_str());
		//jobject jres = env->NewObject(jresult, newID, jstr, results[i].pagerank);
		//jobject jres = env->NewObject(jresult, newID, results[i].index, results[i].pagerank);
		jobject jres = env->NewObject(jresult, newID, jstr, results[i].index, results[i].pagerank);
		env->SetObjectArrayElement(jresults, i, jres);
	}
	return jresults;
}

const string AND = "and", OR = "or", NOT = "not";
const int PRI_N = 1, PRI_A = 2, PRI_O = 3;
mvector<int> logic_n(mvector<int> a, mvector<int> b) {
	int ia = 0, ib = 0;
	mvector<int> iv;
	while (ia < a.size() && ib < b.size()) {
		if (a[ia] == b[ib]) { ia ++; ib ++; }
		else if (a[ia] < b[ib]) { iv.push_back(a[ia ++]); }
		else { ib ++; }
	}
	while (ia ++ < a.size()) { iv.push_back(a[ia]); }
	return iv;
}
mvector<int> logic_a(mvector<int> a, mvector<int> b) {
	int ia = 0, ib = 0;
	mvector<int> iv;
	while (ia < a.size() && ib < b.size()) {
		if (a[ia] == b[ib]) { iv.push_back(a[ia ++]); ib ++; }
		else if (a[ia] < b[ib]) { ia ++; }
		else { ib ++; }
	}
	return iv;
}
mvector<int> logic_o(mvector<int> a, mvector<int> b) {
	int ia = 0, ib = 0;
	mvector<int> iv;
	while (ia < a.size() || ib < b.size()) {
		if (ia == a.size()) { iv.push_back(b[ib ++]); }
		else if (ib == a.size()) { iv.push_back(a[ia ++]); }
		else if (a[ia] == b[ib]) { iv.push_back(a[ia ++]); ib ++; }
		else if (a[ia] < b[ib]) { iv.push_back(a[ia ++]); }
		else { iv.push_back(b[ib ++]); }
	}
	return iv;
}

mvector<int> retrieval(string *keyArr, int len) {
	mvector<int> iv;
	if (len <= 0) { return iv; }
	if (len == 1) { return getIndex(keyArr[0]); }
	int level = 0, prio = 99, pos = -1;
	for (int i = 0; i < len; i ++) {
		if (keyArr[i] == "(") { level --; }
		else if (keyArr[i] == ")") { level ++; }
		else if (level < 0) { continue; }
		else if (keyArr[i] == NOT && prio > PRI_N) { pos = i; prio = PRI_N; }
		else if (keyArr[i] == AND && prio > PRI_A) { pos = i; prio = PRI_A; }
		else if (keyArr[i] == OR && prio > PRI_O) { pos = i; prio = PRI_O; }
	}
	if (pos == -1) { return retrieval(keyArr + 1, len - 1); }
	switch (prio) {
		case PRI_N: return logic_n(retrieval(keyArr, pos), retrieval(keyArr + pos + 1, len - pos - 1)); break;
		case PRI_A: return logic_a(retrieval(keyArr, pos), retrieval(keyArr + pos + 1, len - pos - 1)); break;
		case PRI_O: return logic_o(retrieval(keyArr, pos), retrieval(keyArr + pos + 1, len - pos - 1)); break;
		default: cout << "bad expression!" << endl;
	}
	return iv;
}

int setResults(Result *&results,	// output
		string *keyArr, int len) { // lower-case alpha
	mvector<int> iv = retrieval(keyArr, len);
	results = new Result[iv.size()];
	for (int i = 0; i < iv.size(); i ++) {
		results[i].index = iv[i];
		// TODO: set pagerank
		// TODO: set url
	}
	return iv.size();
}
