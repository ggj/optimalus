#include "guimanager.h"
#include "../scene/gamescene.h"
#include "../gameflow.h"
#include <Rocket/Debugger.h>
#include <Rocket/Controls.h>

GuiManager *gGui = NULL;

GuiManager::GuiManager()
	: pRocket(NULL)
	, pContext(NULL)
	, pDoc(NULL)
{
	gGui = this;
}

GuiManager::~GuiManager()
{
	gGui = NULL;
}

bool GuiManager::Initialize()
{
	pScreen->EnableCursor(true);
	return this->InitializeGUI();
}

bool GuiManager::Shutdown()
{
	RocketEventManager::RemoveListener(this);
	this->ReleaseGUI();
	return true;
}

bool GuiManager::UnloadGUI()
{
	Log("Unloading GUI Document");
	if (pDoc)
	{
		pDoc->Hide();
		pContext->UnloadDocument(pDoc);
		pDoc->RemoveReference();
		pDoc = NULL;
	}

	return true;
}

bool GuiManager::ReloadGUI()
{
	return this->LoadGUI(sDocument);
}

bool GuiManager::LoadGUI(const String &doc)
{
	if (this->UnloadGUI())
	{
		Log("Loading GUI Document");
		pDoc = pContext->LoadDocument(doc.c_str());
		if (pDoc != NULL)
		{
			Rocket::Core::Element *title = pDoc->GetElementById("title");
			if (title != NULL)
				title->SetInnerRML(pDoc->GetTitle());

			pDoc->Focus();
			pDoc->Show();

			if (pDoc->GetElementById("lifes") != NULL)
				pElementLife = pDoc->GetElementById("lifes");

			if (pDoc->GetElementById("time") != NULL)
				pElementTime = pDoc->GetElementById("time");

			if (pDoc->GetElementById("hostages") != NULL)
				pElementHostage = pDoc->GetElementById("hostages");

			if (pDoc->GetElementById("sfx") != NULL && gGameData->IsSfxEnabled())
				pDoc->GetElementById("sfx")->SetAttribute("checked", "");

			if (pDoc->GetElementById("bgm") != NULL && gGameData->IsBgmEnabled())
				pDoc->GetElementById("bgm")->SetAttribute("checked", "");
		}

		sDocument = doc;
	}

	return true;
}

bool GuiManager::InitializeGUI()
{
	pRocket = New(RocketInterface());
	pRocket->sName = "GUI";
	Rocket::Core::SetRenderInterface(pRocket);
	Rocket::Core::SetFileInterface(pRocket);
	Rocket::Core::SetSystemInterface(pRocket);
	Rocket::Core::Initialise();
	Rocket::Controls::Initialise();

	pContext = Rocket::Core::CreateContext("main", Rocket::Core::Vector2i(pScreen->GetWidth(), pScreen->GetHeight()));
	if (pContext == NULL)
	{
		Rocket::Core::Shutdown();
		return false;
	}

	Rocket::Core::String fonts[4];
	fonts[0] = "fonts/Delicious-Roman.otf";
	fonts[1] = "fonts/Delicious-Italic.otf";
	fonts[2] = "fonts/Delicious-Bold.otf";
	fonts[3] = "fonts/Delicious-BoldItalic.otf";

	for (unsigned int i = 0; i < sizeof(fonts) / sizeof(Rocket::Core::String); i++)
		Rocket::Core::FontDatabase::LoadFontFace(fonts[i]);

	Rocket::Debugger::Initialise(pContext);
	pRocket->SetCurrentContext(pContext);
	pInput->AddKeyboardListener(pRocket);
	pInput->AddPointerListener(pRocket);
	pRocket->SetZ(-1000.0f);

	RocketEventManager::AddListener(this);
	return true;
}

void GuiManager::ReleaseGUI()
{
	RocketEventManager::RemoveListener(this);

	pInput->RemovePointerListener(pRocket);
	pInput->RemoveKeyboardListener(pRocket);

	pContext->UnloadAllDocuments();

	if (pDoc)
	{
		pDoc->RemoveReference();
		pDoc = NULL;
	}

	pContext->RemoveReference();

	Rocket::Core::Shutdown();
	Delete(pRocket);
}

void GuiManager::OnGuiEvent(Rocket::Core::Event &ev, const Rocket::Core::String &script)
{
	UNUSED(ev)
	Rocket::Core::StringList commands;
	Rocket::Core::StringUtilities::ExpandString(commands, script, ';');
	for (size_t i = 0; i < commands.size(); ++i)
	{
		Rocket::Core::StringList values;
		Rocket::Core::StringUtilities::ExpandString(values, commands[i], ' ');
		if (values.empty())
			return;

		if (values[0] == "goto" && values.size() > 1)
		{
			if (values[1] == "credits")
				gFlow->Credits();
			else if (values[1] == "menu")
				gFlow->Menu();
			else if (values[1] == "options")
				gFlow->Options();
			else if (values[1] == "game")
				gFlow->DoLoad("game_level1.scene");
		}
		else if (values[0] == "exit")
		{
			pSystem->Shutdown();
		}
		else if (values[0] == "toggle" && values.size() > 1)
		{
			if (values[1] == "sfx")
			{
				if (gGameData->IsSfxEnabled())
				{
					gGameData->SetSfxVolume(pSoundSystem->GetSfxVolume());
					pSoundSystem->SetSfxVolume(0.0f);
					gGameData->SetSfxEnabled(false);
				}
				else
				{
					pSoundSystem->SetSfxVolume(gGameData->GetSfxVolume());
					gGameData->SetSfxEnabled(true);
				}
			}
			else if (values[1] == "bgm")
			{
				if (gGameData->IsBgmEnabled())
				{
					gGameData->SetBgmVolume(pSoundSystem->GetMusicVolume());
					pSoundSystem->SetMusicVolume(0.0f);
					gGameData->SetBgmEnabled(false);
				}
				else
				{
					pSoundSystem->SetMusicVolume(gGameData->GetBgmVolume());
					gGameData->SetBgmEnabled(true);
				}
			}
			else if (values[1] == "fullscreen")
			{
				//this->ReleaseGUI();

				if (gGameData->IsFullScreenEnabled())
				{
					gGameData->SetFullScreenEnabled(false);
					//pScreen->ToggleFullscreen();
					pSystem->Shutdown();
				}
				else
				{
					gGameData->SetFullScreenEnabled(true);
					//pScreen->ToggleFullscreen();
					pSystem->Shutdown();
				}

				//this->InitializeGUI();
				//this->ReloadGUI();
			}
		}
	}
}

// GUI Elements
void GuiManager::SetLife(u32 life)
{
	char x[100];
	snprintf(x, 100, "%d", life);
	pElementLife->SetInnerRML(Rocket::Core::String(x));
	gGameData->SetLife(life);
}

void GuiManager::RemoveLife()
{
	gGameData->RemoveLife();
	this->SetLife(gGameData->GetLife());
}

void GuiManager::SetTime(u32 time)
{
	char x[100];
	snprintf(x, 100, "%d", time);
	pElementTime->SetInnerRML(Rocket::Core::String(x));
	gGameData->SetTime(time);
}

void GuiManager::PrintHostage(u32 hostage)
{
	char x[100];
	snprintf(x, 100, "%d", hostage);
	pElementHostage->SetInnerRML(Rocket::Core::String(x));
}

void GuiManager::SetHostage(u32 hostage)
{
	this->PrintHostage(hostage);
	gGameData->SetHostage(hostage);
}

void GuiManager::RemoveHostage()
{
	gGameData->RemoveHostage();
	this->PrintHostage(gGameData->GetHostage());
}

ISceneObject *GuiManager::GetSceneObject() const
{
	return pRocket;
}
