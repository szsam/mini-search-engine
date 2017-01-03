#pragma once
template<typename T>
class miterator :public std::iterator<std::forward_iterator_tag, T>
{

private://迭代器在此处就是一个指针而已
	T* t;


public:
		miterator()
		{
			t = NULL;
		}
		miterator(T* t_)
		{
			t = t_;
		}
		miterator(const miterator& bbb)
		{
			t = bbb.t;
		}
		miterator& operator=(const miterator& bbb)
		{
			t = bbb.t;
			return *this;
		}

		miterator& operator++()
		{
			t++;
			return *this;
		}
		miterator operator++(int)
		{
			miterator iter = *this;
			t++;
			return iter;
		}
		miterator operator+(int count)
		{
			miterator iter = *this;
			iter.t += count;
			return iter;
		}
		miterator& operator--()
		{
			t--;
			return *this;
		}
		miterator operator--(int)
		{
			miterator iter = *this;
			t--;
			return iter;
		}
		miterator operator-(int count)
		{
			miterator iter = *this;
			iter.t -= count;
			return iter;
		}

		int operator-(const miterator& bbb)
		{
			return t - bbb.t;
		}
		int operator-(const miterator& bbb)const
		{
			return t - bbb.t;
		}

/*指针相关*/
/************************************************************************/
		T& operator*()
		{
			return *t;
		}
		const T& operator*() const
		{
			return *t;
		}

		T* operator->()
		{
			return t;
		}
		const T* operator->() const
		{
			return t;
		}
/************************************************************************/
		 bool operator!=(const miterator& bbb)
		{
			return t != bbb.t;
		}
		 bool operator!=(const miterator& bbb)const
		{
			return t != bbb.t;
		}

		 bool operator==(const miterator& bbb)
		{
			return t == bbb.t;
		}
		 bool operator==(const miterator& bbb)const
		{
			return t == bbb.t;
		}

		 bool operator<(const miterator& bbb)
		{
			return t<bbb.t;
		}
		 bool operator<(const miterator& bbb)const
		{
			return t<bbb.t;
		}

		 bool operator<=(const miterator& bbb)
		{
			return t <= bbb.t;
		}
		 bool operator<=(const miterator& bbb)const
		{
			return t <= bbb.t;
		}

		 bool operator>(const miterator& bbb)
		{
			return t > bbb.t;
		}
		 bool operator>(const miterator& bbb)const
		{
			return t > bbb.t;
		}

		 bool operator>=(const miterator& bbb)
		{
			return t >= bbb.t;
		}
		 bool operator>=(const miterator& bbb)const
		{
			return t >= bbb.t;
		}

};