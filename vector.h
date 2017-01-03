#pragma once
#include "vector_iterator.h"
namespace xmh {
	template <class T>
	class vector {
		typedef miterator<T> iterator;
		typedef const miterator<T> const_iterator;


	public:
		vector()
		{
			_buf = NULL;
			_size=_capacity = 0;
		}
		explicit vector(int capa, const T& val = T())
		{
			initData(capa);
			newCapacity(_capacity);
			for (int i = 0; i < _size; i++)
				_buf[i] = val;
		}
		vector(T* t_)
		{
			_buf = t_;
		}
		/*拷贝构造函数*/
		vector(const vector& bbb)
		{
			_size = bbb.size();
			_capacity = bbb._capacity;
			if (_capacity) {
				_buf = new T[_capacity];
				memcpy(_buf, bbb._buf, sizeof(T)*_capacity);
			}
		}

		/*复制构造函数*/
		vector& operator=(const vector& bbb)
		{
			//自复制
			if (this == &bbb)
				return *this;

			//删除原有空间
			delete[]_buf;

			_size = bbb.size();
			_capacity = bbb._capacity;
			if (_capacity) {
				_buf = new T[_capacity];
				memcpy(_buf, bbb._buf, sizeof(T)*_capacity);
			}
			return *this;
		}

		~vector()
		{
			if (_buf)
			{
				delete[] _buf;
				_buf = NULL;
			}
			_size = _capacity = 0;
		}

		void push_back(const T& t)
		{
			if (_size == _capacity)
			{
				int capa = calculateCapacity();
				newCapacity(capa);
			}
			_buf[_size++] = t;
		}

		void pop_back()
		{
			if (!empty())
				erase(end() - 1);
		}

		iterator begin() const
		{
			return iterator(&_buf[0]);
		}

		iterator end() const
		{
			return iterator(&_buf[_size]);
		}

		iterator erase(const_iterator iter)
		{
			int index = iter - begin();
			if (index < _size && _size>0)
			{
				memmove(_buf + index, _buf + index + 1, (_size - index)*sizeof(T));
				_buf[--_size] = T();
			}
			return iterator(iter);
		}
	
		T& operator[](int index) const
		{
			assert(_size > 0 && index >= 0 && index < _size);
			return _buf[index];
		}

		int size() const
		{
			return _size;
		}

	private:
		int _size;
		int _capacity;
		T* _buf;

		void newCapacity(int capa)
		{
			_capacity = capa;
			T* newBuf = new T[_capacity];
			if (_buf)
			{
				memcpy(newBuf, _buf, _size*sizeof(T));
				delete[] _buf;
			}
			_buf = newBuf;
		}

		int calculateCapacity()
		{
			return _capacity * 3 / 2 + 1;
		}

		void initData(int capa)
		{
			_buf = NULL;
			_size = _capacity = capa > 0 ? capa : 0;
		}

	};
};