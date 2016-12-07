#ifndef __MVECTOR_H__
#define __MVECTOR_H__

template <class T>
class mvector {
public:
	mvector() {
		buf_size = INIT_SIZE;
		len = 0;
		buf = new T[buf_size];
	}
	~mvector() {
		delete buf;
	}
	void push_back(const T& e) {
		if (len >= buf_size) {
			expand();
		}
		buf[len ++] = e;
	}
	int size() { return len; }
	T& operator [](int index) { return buf[index]; }

private:
	T *buf;
	int len, buf_size;
	static const int INIT_SIZE = 20;
	void expand() {
		T *tmp = new T[buf_size * 2];
		for (int i = 0; i < buf_size; i ++) {
			tmp[i] = buf[i];
		}
		buf = tmp;
		buf_size *= 2;
	}
};



#endif
