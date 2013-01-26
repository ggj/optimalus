#ifndef _ENTITY_H_
#define _ENTITY_H_

#include <Seed.h>
#include <string>

class Entity
{
	public:
		Entity();

		inline const std::string &GetName() const;

	private:
		std::string strName;
};

const std::string &Entity::GetName() const
{
	return strName;
}

#endif
