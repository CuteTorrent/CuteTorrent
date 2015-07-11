#ifndef _SINGLETON_INCLUDED_
#define _SINGLETON_INCLUDED_
#include <boost/smart_ptr.hpp>

#define DEFINE_PTR_CALSS(T) class T;\
	typedef boost::shared_ptr<T> T##Ptr;

template<typename T>
class Singleton
{

public:
	static boost::shared_ptr<T> getInstance();
private:
	static boost::weak_ptr<T> m_pInstance;
};

template <typename T>
boost::weak_ptr<T>  Singleton<T>::m_pInstance;

template <typename T>
boost::shared_ptr<T> Singleton<T>::getInstance()
{
	boost::shared_ptr<T> instance = m_pInstance.lock();

	if (!instance)
	{
		instance.reset(new T());
		m_pInstance = instance;
	}

	return instance;
}

#endif