#pragma once#include <URegistrar.h>template <class T>class PPFactory{public:		static T	*CreateFromStream(LStream *inStream)		{			return new T(inStream);		}		static bool	RegisterClass()		{			URegistrar::RegisterClass(T::class_ID, (ClassCreatorFunc)CreateFromStream);			return true;		}		static bool	RegisterClass(unsigned long class_ID)		{			URegistrar::RegisterClass(class_ID, (ClassCreatorFunc)CreateFromStream);			return true;		}};