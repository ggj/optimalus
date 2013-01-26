#ifndef _ENTITY_FACTORY_H
#define _ENTITY_FACTORY_H

#include <string>

class Entity;

namespace EntityFactory
{
	typedef Entity *(*CreateEntityProc_t)();

	Entity *CreateEntity(const std::string &name);
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

#endif
