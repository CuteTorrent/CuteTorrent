#ifndef _SINGLETON_INCLUDED_
#define _SINGLETON_INCLUDED_
#ifndef Q_MOC_RUN
#include <boost/smart_ptr.hpp>
#endif

#define DEFINE_PTR_CALSS(T) class T;\
	typedef boost::shared_ptr<T> T##Ptr;

template <typename T>
class Singleton
{
public:
	static boost::shared_ptr<T> getInstance();
private:
	static boost::shared_ptr<T> m_pInstance;
};

template <typename T>
boost::shared_ptr<T> Singleton<T>::m_pInstance;

template <typename T>
boost::shared_ptr<T> Singleton<T>::getInstance()
{
	if (!m_pInstance)
	{
		m_pInstance.reset(new T());
	}

	return m_pInstance;
}

#endif

