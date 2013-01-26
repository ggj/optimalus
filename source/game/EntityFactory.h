#ifndef _ENTITY_FACTORY_H
#define _ENTITY_FACTORY_H

#include "../defines.h"

class Entity;

namespace EntityFactory
{
	typedef Entity *(*CreateEntityProc_t)();

    Entity *CreateEntity(const String &name);
    Entity *CreateEntity(const char *name);

	void AddCreator(const char *name, CreateEntityProc_t proc);

	class AutoCreator
	{
		public:
			inline AutoCreator(const char *name, CreateEntityProc_t proc)
			{
				AddCreator(name, proc);
			}
	};
}

#define ENTITY_CREATOR(NAME, CLASS)												\
	namespace																	\
	{																			\
		Entity *Create##CLASS()													\
		{																		\
			return New(CLASS());												\
		}																		\
		EntityFactory::AutoCreator clAutoCreator##CLASS_g(NAME, Create##CLASS);	\
    }

#endif
