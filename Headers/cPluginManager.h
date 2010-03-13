#ifndef CPLUGINMANAGER_H_INCLUDED
#define CPLUGINMANAGER_H_INCLUDED

#include <string>
#include <map>
#include <vector>

#include "../include/IPluginManager.h"
#include "../include/IAudioPlugin.h"
#include "../Headers/cMutex.h"
#include "../Headers/cSTLAllocator.h"

#ifdef CAUDIO_COMPILE_WITH_PLUGIN_SUPPORT

#ifdef CAUDIO_COMPILE_WITH_DYNAMIC_PLUGIN_SUPPORT

#ifdef CAUDIO_PLATFORM_WIN
#	define WIN32_LEAN_AND_MEAN
#	include <windows.h>

struct HINSTANCE__;
typedef struct HINSTANCE__* hInstance;

#	define DYNLIB_HANDLE hInstance
#	define DYNLIB_LOAD( a ) LoadLibraryEx( a, NULL, LOAD_WITH_ALTERED_SEARCH_PATH )
#	define DYNLIB_GETSYM( a, b ) GetProcAddress( a, b )
#	define DYNLIB_UNLOAD( a ) !FreeLibrary( a )
#endif

#ifdef CAUDIO_PLATFORM_MAC
#   include <dlfcn.h>
#	define DYNLIB_HANDLE void*
#	define DYNLIB_LOAD( a ) dlopen( a, RTLD_LAZY | RTLD_GLOBAL)
#	define DYNLIB_GETSYM( a, b ) dlsym( a, b )
#	define DYNLIB_UNLOAD( a ) dlclose( a )
#endif

#ifdef CAUDIO_PLATFORM_LINUX
#	include <dlfcn.h>
#	define DYNLIB_HANDLE void*
#	define DYNLIB_LOAD( a ) dlopen( a, RTLD_LAZY | RTLD_GLOBAL)
#	define DYNLIB_GETSYM( a, b ) dlsym( a, b )
#	define DYNLIB_UNLOAD( a ) dlclose( a )
#endif

#endif //! CAUDIO_COMPILE_WITH_DYNAMIC_PLUGIN_SUPPORT

namespace cAudio
{
	class cPluginManager : public IPluginManager
	{
	public:
		cPluginManager();
		~cPluginManager();

		static cPluginManager* Instance()
		{
			static cPluginManager theInstance;
			return &theInstance;
		}

		virtual bool installPlugin(IAudioPlugin* plugin, const char* name);
		virtual bool installPlugin(const char* filename, const char* name);

		virtual bool checkForPlugin(const char* name);
		virtual IAudioPlugin* getPlugin(const char* name);
		virtual unsigned int getPluginCount();
		std::vector<IAudioPlugin*> getPluginList();

		virtual void uninstallPlugin(IAudioPlugin* plugin);
		virtual void uninstallPlugin(const char* name);

#ifdef CAUDIO_COMPILE_WITH_DYNAMIC_PLUGIN_SUPPORT
		void autoLoadPlugins();
#endif
	protected:
		std::map<std::string, IAudioPlugin*, std::less<std::string>, cSTLAllocator<std::pair<std::string, IAudioPlugin*> > > RegisteredPlugins;
		typedef std::map<std::string, IAudioPlugin*, std::less<std::string>, cSTLAllocator<std::pair<std::string, IAudioPlugin*> > >::iterator RegisteredPluginsIterator;
#ifdef CAUDIO_COMPILE_WITH_DYNAMIC_PLUGIN_SUPPORT
		std::map<IAudioPlugin*, DYNLIB_HANDLE, std::less<IAudioPlugin*>, cSTLAllocator<std::pair<IAudioPlugin*, DYNLIB_HANDLE> > > DynamicallyLoadedPlugins;
		typedef std::map<IAudioPlugin*, DYNLIB_HANDLE, std::less<IAudioPlugin*>, cSTLAllocator<std::pair<IAudioPlugin*, DYNLIB_HANDLE> > >::iterator DynamicallyLoadedPluginsIterator;
#endif
	};
};

#endif //! CAUDIO_COMPILE_WITH_PLUGIN_SUPPORT

#endif //! CPLUGINMANAGER_H_INCLUDED