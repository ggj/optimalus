#ifndef _GUIMANAGER_H_
#define _GUIMANAGER_H_

#include "defines.h"
#include <api/rocket/RocketInterface.h>
#include <Rocket/Core.h>

using namespace Seed;
using namespace Seed::RocketGui;

class GuiManager;
extern GuiManager *gGui;

class GuiManager : public IRocketEventListener
{
	public:
		GuiManager();
		virtual ~GuiManager();

		bool Initialize();
		bool Shutdown();

		ISceneObject *GetSceneObject() const;

		// GUI
		bool LoadGUI(const String &doc);
		bool ReloadGUI();
		bool UnloadGUI();
		void ReleaseGUI();

		void SetLife(u32 life);
		void SetTime(u32 time);
		void SetHostage(u32 hostage);

		void RemoveLife();
		void RemoveHostage();

		// IRocketEventListener
		virtual void OnGuiEvent(Rocket::Core::Event &ev, const Rocket::Core::String &script);

	private:
		bool InitializeGUI();
		void PrintHostage(u32 hostage);

	private:
		SEED_DISABLE_COPY(GuiManager);

		String				sDocument;

		// GUI
		RocketInterface			*pRocket;
		Rocket::Core::Context	*pContext;
		Rocket::Core::ElementDocument *pDoc;

		// GUI Elements
		Rocket::Core::Element	*pElementLife;
		Rocket::Core::Element	*pElementTime;
		Rocket::Core::Element	*pElementHostage;
		Rocket::Core::Element	*pElementSfx;
		Rocket::Core::Element	*pElementBgm;
};

#endif // _GUIMANAGER_H_
