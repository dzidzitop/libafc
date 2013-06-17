#ifndef AFC_SINGLETON_H_
#define AFC_SINGLETON_H_

#include "thread.h"
#include "Exception.h"

namespace afc
{
	template<typename T> class Singleton
	{
	public:
		inline Singleton() throw(IllegalStateException);
		inline ~Singleton() {instanceCreated = false;}
	private:
		static Mutex mutex;
		static volatile bool instanceCreated;
	};
}

template<typename T> afc::Mutex afc::Singleton<T>::mutex;
template<typename T> volatile bool afc::Singleton<T>::instanceCreated = false;

template<typename T> afc::Singleton<T>::Singleton() throw(IllegalStateException)
{ Lock l(mutex);
	if (instanceCreated) {
		throwException<IllegalStateException>("A singleton can have at most one instance.");
	}
	instanceCreated = true;
}

#endif /*AFC_SINGLETON_H_*/
