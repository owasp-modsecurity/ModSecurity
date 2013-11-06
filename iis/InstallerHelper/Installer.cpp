// Installer.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <ahadmin.h>
#include <string>
#include "XUnzip.h"

// Logging level definition.
#define EVENT_ERROR       100
#define EVENT_CRITICAL    75
#define EVENT_IMPORTANT   50
#define EVENT_NORMAL      25
#define EVENT_LEVEL_MASK  0xFFFF

#define EVENT_APPLOG      (1 << 17)
#define EVENT_SYSLOG      (1 << 18)
#define EVENT_STDOUT      (1 << 19)
#define EVENT_STDERR      (1 << 20)

// This class uese IIS7.0 native API to operate on
// %windir%\system32\inetsrv\config\applicationHost.config
class Iis7Config
{
 public:
  Iis7Config() {}
  virtual ~Iis7Config() {}

  // Defines the IIS running mode.   
  enum AppMode {     
	  MODE_X64,  // IIS runs under x86_64 mode.     
	  MODE_X86,  // IIS runs under i386 mode.     
	  MODE_UNKNOWN,  // IIS run under an unknow mode.     
	  MODE_FAIL  // Failed to retrieve app mode.   
  };   

	static void Log(int event_meta, const char *event_pattern, ...) {
	  const int     kErrorLen = 2048;
	  char          event_message[kErrorLen];
	  va_list argptr;

	  // If current event is less than log_level_, return directly.
	  int event_level = event_meta & EVENT_LEVEL_MASK;
	  //if (event_level < log_level_) return;

	  // Combile log type with default log type.
	  int event_type = event_meta;// | log_type_;

	  va_start(argptr, event_pattern);
	  vsnprintf(event_message, kErrorLen, event_pattern, argptr);
	  va_end(argptr);

//	#ifdef WIN32
	  // Only write error and critical event to windows event log
//	  if ((event_type & EVENT_SYSLOG) != 0 && event_level >= EVENT_CRITICAL) {
		WORD windows_event_type;
		if (event_level == EVENT_ERROR) {
		  windows_event_type = EVENTLOG_ERROR_TYPE;
		} else {  //  if (event_level == EVENT_CRITICAL)
		  windows_event_type= EVENTLOG_INFORMATION_TYPE;
		}

		LPCSTR event_message_pointer = event_message;

		// Get a handle to the event log.
		HANDLE h = RegisterEventSource(NULL, L"ModSecurityIIS Installer");
		if (h != NULL) {
		  // Report the event.
		  ReportEventA(h,                       // event log handle
					   windows_event_type,      // event type
					   0,                       // event category
					   1,                       // event identifier
					   NULL,                    // no user security identifier
					   1,                       // number of strings
					   0,                       // no data
					   &event_message_pointer,  // pointer to strings
					   NULL);                   // no data
		  DeregisterEventSource(h);
		}
//	  }
//	#endif  // WIN32

	  // Output log to file
/*	  if ((event_type & EVENT_APPLOG) != 0) {
		if (CreateLogFile()) {
		  FILE* file = fopen(log_file_name_.c_str(), "a+");
		  if (file != NULL) {
			fputs(FormatW3CTime(time(NULL)).c_str(), file);
			fputs(": ", file);
			fputs(event_message, file);
			fputs("\n", file);
			fclose(file);
		  } else if ((event_type & EVENT_STDERR) != 0) {
			fprintf(stderr, "%s: Failed to open log file (%d).\n",
					FormatW3CTime(time(NULL)).c_str(), errno);
		  }
		} else if ((event_type & EVENT_STDERR) != 0) {
		  fprintf(stderr, "%s: Failed to create log file (%d)\n",
				  FormatW3CTime(time(NULL)).c_str(), errno);
		}
	  }

	  // Output to std err.
	  if ((event_type & EVENT_STDERR) != 0) {
		fprintf(stderr, "%s: %s\n",
				FormatW3CTime(time(NULL)).c_str(), event_message);
	  }*/
	}

	static void Log(int event_meta, const wchar_t *event_pattern, ...) {
	  const int     kErrorLen = 2048;
	  wchar_t          event_message[kErrorLen];
	  va_list argptr;

	  // If current event is less than log_level_, return directly.
	  int event_level = event_meta & EVENT_LEVEL_MASK;
	  //if (event_level < log_level_) return;

	  // Combile log type with default log type.
	  int event_type = event_meta;// | log_type_;

	  va_start(argptr, event_pattern);
	  _vsnwprintf(event_message, kErrorLen, event_pattern, argptr);
	  va_end(argptr);

//	#ifdef WIN32
	  // Only write error and critical event to windows event log
//	  if ((event_type & EVENT_SYSLOG) != 0 && event_level >= EVENT_CRITICAL) {
		WORD windows_event_type;
		if (event_level == EVENT_ERROR) {
		  windows_event_type = EVENTLOG_ERROR_TYPE;
		} else {  //  if (event_level == EVENT_CRITICAL)
		  windows_event_type= EVENTLOG_INFORMATION_TYPE;
		}

		LPCWSTR event_message_pointer = event_message;

		// Get a handle to the event log.
		HANDLE h = RegisterEventSource(NULL, L"ModSecurityIIS Installer");
		if (h != NULL) {
		  // Report the event.
		  ReportEventW(h,                       // event log handle
					   windows_event_type,      // event type
					   0,                       // event category
					   1,                       // event identifier
					   NULL,                    // no user security identifier
					   1,                       // number of strings
					   0,                       // no data
					   &event_message_pointer,  // pointer to strings
					   NULL);                   // no data
		  DeregisterEventSource(h);
		}
//	  }
//	#endif  // WIN32

	  // Output log to file
/*	  if ((event_type & EVENT_APPLOG) != 0) {
		if (CreateLogFile()) {
		  FILE* file = fopen(log_file_name_.c_str(), "a+");
		  if (file != NULL) {
			fputs(FormatW3CTime(time(NULL)).c_str(), file);
			fputs(": ", file);
			fputs(event_message, file);
			fputs("\n", file);
			fclose(file);
		  } else if ((event_type & EVENT_STDERR) != 0) {
			fprintf(stderr, "%s: Failed to open log file (%d).\n",
					FormatW3CTime(time(NULL)).c_str(), errno);
		  }
		} else if ((event_type & EVENT_STDERR) != 0) {
		  fprintf(stderr, "%s: Failed to create log file (%d)\n",
				  FormatW3CTime(time(NULL)).c_str(), errno);
		}
	  }

	  // Output to std err.
	  if ((event_type & EVENT_STDERR) != 0) {
		fprintf(stderr, "%s: %s\n",
				FormatW3CTime(time(NULL)).c_str(), event_message);
	  }*/
	}

  // Install/uninstall filters in IIS.
  // Actually, add/remove entry to/from <system.webServer/globalModules>
  // adn <system.webServer/modules> sections.
  static bool InstallFilter(const wchar_t* dll_file);
  static bool UninstallFilter();

  // Install/uninstall a site for Admin Console.
  // Permissions for CGI exe are set/unset accordingly.
/*  static bool InstallAdminConsole(const char* site_root, const char* cgi_path);
  static bool UninstallAdminConsole(const char* site_root,
                                    const char* cgi_path);
*/
  // Determine whether version 7 mode supported.
  static bool IsSupported();

  // Load configuration from IIS
  // Actually, load from <system.applicationHost/sites> section.
  virtual bool Load();

  // Get IIS 7 running mode.
  static AppMode GetAppMode();

 private:
  // Module name
  static BSTR kModuleName;
  static BSTR kConfigName;
  
  // Admin Console site name, "Google Sitemap Generator Admin Console".
  //static BSTR kSiteName;

  static bool Iis7Config::AddToConfigSections(IAppHostWritableAdminManager* manager,
											 BSTR config);

  // Add an entry to <system.webServer/globalModules> section, like:
  // <add name="ModSecurityIIS" image="path\ModSecurityIIS.dll" />
  static bool AddToGlobalModules(IAppHostWritableAdminManager* manager,
                                 BSTR image);

  // Add an entry to <system.webServer/modules> section, like:
  // <add name="ModSecurityIIS" />
  static bool AddToModules(IAppHostWritableAdminManager* manager);

  // All CGI exe to be executed.
  // "cgi_path" is the executable file path.
  //static bool SetCGIRestriction(IAppHostWritableAdminManager* manager,
  //                              BSTR cgi_path);

  // Create Admin Console site.
  // "site_root" is the site root directory.
  //static bool CreateAdminConsoleSite(IAppHostWritableAdminManager *manager,
  //                                   BSTR site_root);

  // Remove customization for Admin Console site.
  // This customzation is added in CustomizeAdminConsole method.
  //static bool RemoveAdminConsoleCustomization(IAppHostWritableAdminManager* manager);

  // Add customization for Admin Console site.
  // Actually, it adds a <location> tag for the Admin Console site.
  //static bool CustomizeAdminConsole(IAppHostWritableAdminManager* manager,
  //                                  BSTR cgi_path);

  // Set property value.
  static bool SetProperty(IAppHostElement* element, BSTR name, BSTR value);

  // Get property value.
  static bool GetProperty(IAppHostElement* element, BSTR name, VARIANT* value);

  // Get the index from given collection, whose key=value.
  // *index == -1 indicates nothing found.
  static bool GetFromCollection(IAppHostElementCollection* collection,
                                BSTR property_key, BSTR property_value,
                                short* index);

  // Get the element from given colletion.
  // The element should contains a propery, whose key/value is given by arg.
  // "*element = NULL" indicates nothing can be found.
  static bool GetFromCollection(IAppHostElementCollection* collection,
                                BSTR property_key, BSTR property_value,
                                IAppHostElement** element);

  // Get child's index from given collection, whose key = value.
  // *index == -1 indicates no such child.
  static bool GetFromChildren(IAppHostChildElementCollection* children,
                              BSTR child_name, short* index);

  // Get child element form given child collection.
  // The child element should contains a property, whose key/value is given by
  // arg. *element = NULL indicates no such child.
  static bool GetFromChildren(IAppHostChildElementCollection* children,
                              BSTR child_name, IAppHostElement** element);

  // Remove an entry from given section.
  static bool RemoveFromCollection(IAppHostWritableAdminManager* manager,
                                  BSTR section, BSTR property_name, BSTR property_value);

  // Load site configuration from an element.
  //bool LoadSite(IAppHostElement* site_element);

	static int Iis7Config::GetNativeSystemInfo(SYSTEM_INFO* system_info);
};

BSTR Iis7Config::kModuleName = L"ModSecurityIIS";
BSTR Iis7Config::kConfigName = L"ModSecurity";
//BSTR Iis7Config::kSiteName = L"ModSecurityIIS Admin Console";

#define CHECK_HRESULT(expression, msg) \
{ \
HRESULT _hr = (expression); \
if (FAILED(_hr)) { \
  Log(EVENT_ERROR, msg " (0x%0x)", _hr); \
  break; \
} \
}
#define CHECK_BOOL(expression, msg) \
  if (!expression) { \
    Log(EVENT_ERROR, msg); \
    break; \
  }

bool Iis7Config::Load() {
  IAppHostAdminManager*   admin_manager = NULL;
  IAppHostElement*        parent = NULL;
  IAppHostElementCollection* collection = NULL;
  IAppHostElement*        element = NULL;

  //BSTR  section_name = SysAllocString(L"system.applicationHost/sites");
  BSTR bstr_config_path = SysAllocString(L"MACHINE/WEBROOT/APPHOST");

  HRESULT hresult = S_OK;
  bool result = false;
  do {
    // Initialize com
    hresult = CoInitializeEx(NULL, COINIT_MULTITHREADED);
    if (FAILED(hresult)) {
      Log(EVENT_ERROR, "Failed to intialize COM.");
      break;
    }

    // Create an admin manager
    hresult = CoCreateInstance(__uuidof(AppHostAdminManager), NULL, 
              CLSCTX_INPROC_SERVER,
              __uuidof(IAppHostAdminManager),
              reinterpret_cast<LPVOID*>(&admin_manager));
    if (FAILED(hresult)) {
      Log(EVENT_ERROR, "Failed to create AppHostAdminManager.");
      break;
    }
/*
    // Get <sites> section
    hresult = admin_manager->GetAdminSection(section_name, bstr_config_path, &parent);
    if (FAILED(hresult)) {
      Log(EVENT_ERROR, "Unable to access sites configuration.");
      break;
    } else if (&parent == NULL) {
      Log(EVENT_ERROR, "Unable to get sites configuration."); 
      break;
    }

    // Get all the child elements of <sites>
    hresult = parent->get_Collection(&collection);
    if (FAILED(hresult)) {
      Log(EVENT_ERROR, "Unable to access <sites> child collection.");
      break;
    } else if (&collection == NULL) {
      Log(EVENT_ERROR, "Unable to get <sites> child collection."); 
      break;
    }

    // Iterate all the children of <sites>
    DWORD element_count = 0;
    hresult = collection->get_Count(&element_count);
    for(USHORT i = 0; i < element_count; ++i) {
      VARIANT item_index;
      item_index.vt = VT_I2;
      item_index.iVal = i;

      // Get the child element
      hresult = collection->get_Item(item_index, &element);
      if (FAILED(hresult) || &element == NULL) {
        Log(EVENT_ERROR, "Failed to iterate child (%d) of sites.", i);
        continue;
      }

      if (!LoadSite(element)) {
        Log(EVENT_ERROR, "Failed to load site from site (%d).", i);
        continue;
      }

      element->Release();
      element = NULL;
    }
*/
    result = true;
  } while (false);

  // Exiting / Unwinding
  if (element != NULL) element->Release();
  if (collection != NULL) collection->Release();
  if (parent != NULL) parent->Release();
  if (admin_manager != NULL) admin_manager->Release();

  //SysFreeString(section_name);
  SysFreeString(bstr_config_path);
  CoUninitialize();

  return result;
}

bool Iis7Config::InstallFilter(const wchar_t* dll_file) {
  IAppHostWritableAdminManager*   admin_manager = NULL;
  BSTR bstr_config_path = SysAllocString(L"MACHINE/WEBROOT/APPHOST");

  // Build the full path of ModSecurity module DLL.
  BSTR module_wpath = SysAllocString(dll_file);
  BSTR config = SysAllocString(kConfigName);

  HRESULT hresult = S_OK;
  bool result = false;
  do {
    // Initialize com
    hresult = CoInitializeEx(NULL, COINIT_MULTITHREADED);
    if (FAILED(hresult)) {
      Log(EVENT_ERROR, "Failed to intialize COM.");
      break;
    }

    // Create a writable admin manager
    hresult = CoCreateInstance(__uuidof(AppHostWritableAdminManager), NULL, 
              CLSCTX_INPROC_SERVER,
              __uuidof(IAppHostWritableAdminManager),
              reinterpret_cast<LPVOID*>(&admin_manager));
    if (FAILED(hresult)) {
      Log(EVENT_ERROR, "Failed to create AppHostWritableAdminManager.");
      break;
    }

    // set commit path
    hresult = admin_manager->put_CommitPath(L"MACHINE/WEBROOT/APPHOST");
    if (FAILED(hresult)) {
      Log(EVENT_ERROR, "Failed to put commit path.");
      break;
    }

    // Add ModSecurity entry to <system.webServer/globalModules> section
    if (!AddToGlobalModules(admin_manager, module_wpath)) {
      Log(EVENT_ERROR, "Failed to add entry to globalModules.");
      break;
    }

    // Add ModSecurity entry to <system.webServer/modules> section
    if (!AddToModules(admin_manager)) {
      Log(EVENT_ERROR, "Failed to add entry to modules.");
      break;
    }

	if(!AddToConfigSections(admin_manager, config)) {
      Log(EVENT_ERROR, "Failed to add entry to config sections.");
      break;
    }

    // Save changes.
    hresult = admin_manager->CommitChanges();
    if (FAILED(hresult)) {
      Log(EVENT_ERROR, "Failed to save changes to install module.");
      break;
    }

    result = true;
  } while (false);

  // Exiting / Unwinding
  if (admin_manager != NULL) admin_manager->Release();

  SysFreeString(module_wpath);
  SysFreeString(bstr_config_path);
  SysFreeString(config);

  CoUninitialize();

  return result;
}

bool Iis7Config::UninstallFilter() {
  IAppHostWritableAdminManager*   admin_manager = NULL;
  BSTR bstr_config_path = SysAllocString(L"MACHINE/WEBROOT/APPHOST");

  HRESULT hresult = S_OK;
  bool result = false;
  do {
    // Initialize com
    hresult = CoInitializeEx(NULL, COINIT_MULTITHREADED);
    if (FAILED(hresult)) {
      Log(EVENT_ERROR, "Failed to intialize COM.");
      break;
    }

    // Create a writable admin manager
    hresult = CoCreateInstance(__uuidof(AppHostWritableAdminManager), NULL, 
              CLSCTX_INPROC_SERVER,
              __uuidof(IAppHostWritableAdminManager),
              reinterpret_cast<LPVOID*>(&admin_manager));
    if (FAILED(hresult)) {
      Log(EVENT_ERROR, "Failed to create AppHostWritableAdminManager.");
      break;
    }

    // set commit path
    hresult = admin_manager->put_CommitPath(bstr_config_path);
    if (FAILED(hresult)) {
      Log(EVENT_ERROR, "Failed to put commit path.");
      break;
    }

    // Remove ModSecurity entry from <system.webServer/globalModules> section.
    if (!RemoveFromCollection(admin_manager, L"system.webServer/globalModules", L"name", kModuleName)) {
      Log(EVENT_ERROR, "Failed to remove entry from globalModules.");
      break;
    }

    // Remove ModSecurity entry from <system.webServer/modules> section.
    if (!RemoveFromCollection(admin_manager, L"system.webServer/modules", L"name", kModuleName)) {
      Log(EVENT_ERROR, "Failed to remove entry from modules.");
      break;
    }

    // Save changes.
    hresult = admin_manager->CommitChanges();
    if (FAILED(hresult)) {
      Log(EVENT_ERROR, "Failed to save changes to install module.");
      break;
    }

    result = true;
  } while (false);

  // Exiting / Unwinding
  if (admin_manager != NULL) admin_manager->Release();
  SysFreeString(bstr_config_path);

  CoUninitialize();

  return result;
}

// Add section to <configSections/sectionGroup "system.webServer">, like:
// <section name="ModSecurity" overrideModeDefault="Allow" allowDefinition="Everywhere" />
bool Iis7Config::AddToConfigSections(IAppHostWritableAdminManager* manager,
                                         BSTR config) {
	HRESULT		hr;
	bool		result = false;
    IAppHostConfigManager               * pCfgMgr     = NULL;
    IAppHostConfigFile                  * pCfgFile    = NULL; 
    IAppHostSectionGroup                * pRtSctnGrp  = NULL;
    IAppHostSectionGroup                * pSctnGrp    = NULL;
    IAppHostSectionDefinitionCollection * pSctnDefCol = NULL;
    IAppHostSectionDefinition           * pSctnDef    = NULL;

    BSTR bstrConfigCommitPath = SysAllocString(L"MACHINE/WEBROOT/APPHOST");
    BSTR bstrSctnGrpName      = SysAllocString(L"system.webServer");
    BSTR bstrDeny             = SysAllocString(L"Allow");
    BSTR bstrAppHostOnly      = SysAllocString(L"Everywhere");

    // Get an IAppHostConfigManager
    hr = manager -> get_ConfigManager ( &pCfgMgr );
    if ( FAILED( hr ) || ( &pCfgMgr == NULL ) )
    {
        Log(EVENT_ERROR, "ERROR: Unable to get a config manager.\n" );
        goto exit;
    }

    // Get an IAppHostConfigFile
    hr = pCfgMgr -> GetConfigFile ( bstrConfigCommitPath, &pCfgFile );
    if ( FAILED ( hr ) || ( &pCfgFile == NULL ) )
    {
        if ( E_ACCESSDENIED == hr )
        {
            Log(EVENT_ERROR, "ERROR: Access to configuration denied.\n" );
            Log(EVENT_ERROR, "       Run sample as an administrator.\n" );
        }
        else
        {
            Log(EVENT_ERROR, "ERROR: Unable to get config file.\n" );
        }
        goto exit;
    }

    // Get the root section group
    hr = pCfgFile -> get_RootSectionGroup ( &pRtSctnGrp );
    if ( FAILED ( hr ) || ( &pRtSctnGrp == NULL ) )
    {
        Log(EVENT_ERROR, "ERROR: Unable to access root section group\n" );
        goto exit;
    }

	ULONG i, cnt;

	hr = pRtSctnGrp ->get_Count(&cnt);
    if ( FAILED ( hr ))
    {
        Log(EVENT_ERROR, "ERROR: Unable to enumerate root section group\n" );
        goto exit;
    }

	for(i = 0; i < cnt; i++)
	{
		VARIANT v;
		BSTR name;

		v.vt = VT_I4;
		v.intVal = i;

		hr = pRtSctnGrp->get_Item(v, &pSctnGrp);
		if ( FAILED ( hr ) || ( &pSctnGrp == NULL ) )
		{
			Log(EVENT_ERROR, "ERROR: Unable to access root section item\n" );
			goto exit;
		}

		hr = pSctnGrp->get_Name(&name);
		if ( FAILED ( hr ))
		{
			Log(EVENT_ERROR, "ERROR: Unable to get root section name\n" );
			goto exit;
		}

		if(wcscmp(name, L"system.webServer") == 0)
			break;

		pSctnGrp->Release();
	}

	if(i == cnt)
	{
		Log(EVENT_ERROR, "ERROR: Unable to find system.webServer section group\n");
		goto exit;
	}

    // Get the section collection
    hr = pSctnGrp -> get_Sections ( &pSctnDefCol );
    if ( FAILED ( hr ) || ( &pSctnDefCol == NULL ) )
    {
        Log(EVENT_ERROR, "ERROR: Unable to access section collection\n" );
        goto exit;
    }

	hr = pSctnDefCol->get_Count(&cnt);
    if ( FAILED ( hr ))
    {
        Log(EVENT_ERROR, "ERROR: Unable to enumerate section collection\n" );
        goto exit;
    }

	for(i = 0; i < cnt; i++)
	{
		VARIANT v;
		BSTR name;

		v.vt = VT_I4;
		v.intVal = i;

		hr = pSctnDefCol->get_Item(v, &pSctnDef);
		if ( FAILED ( hr ) || ( &pSctnDef == NULL ) )
		{
			Log(EVENT_ERROR, "ERROR: Unable to access section item\n" );
			goto exit;
		}

		hr = pSctnDef->get_Name(&name);
		if ( FAILED ( hr ))
		{
			Log(EVENT_ERROR, "ERROR: Unable to get section name\n" );
			goto exit;
		}

		if(wcscmp(name, config) == 0)
			break;

		pSctnDef->Release();
	}

	if(i == cnt)
	{
		// Add the new section
		hr = pSctnDefCol -> AddSection ( config, &pSctnDef );
		if ( FAILED ( hr ) || ( &pSctnDef == NULL ) )
		{
			Log(EVENT_ERROR, "ERROR: Unable to add new section\n" );
			goto exit;
		}
	}

    // Set the section attributes
    pSctnDef -> put_OverrideModeDefault ( bstrDeny );
    pSctnDef -> put_AllowDefinition ( bstrAppHostOnly );

	result = true;
exit:
    // Exiting / Unwinding
    if ( pRtSctnGrp != NULL )
    {
        pRtSctnGrp->Release(); 
        pRtSctnGrp = NULL;
    }
    if ( pSctnGrp    != NULL )
    {
        pSctnGrp->Release(); 
        pSctnGrp = NULL;
    }
    if ( pSctnDefCol != NULL )
    {
        pSctnDefCol->Release(); 
        pSctnDefCol = NULL;
    }
    if ( pSctnDef != NULL )
    {
        pSctnDef->Release(); 
        pSctnDef = NULL;
    }
    if ( pCfgFile != NULL )
    {
        pCfgFile->Release();
        pCfgFile = NULL;
    }
    if ( pCfgMgr != NULL )
    {
        pCfgMgr->Release();
        pCfgMgr = NULL;
    }

    SysFreeString( bstrConfigCommitPath );
    SysFreeString( bstrSctnGrpName );
    SysFreeString( bstrDeny );
    SysFreeString( bstrAppHostOnly );

    return result;
}

// Add an entry to <system.webServer/globalModules> section, like:
// <add name="ModSecurityIIS" image="${image}" />
bool Iis7Config::AddToGlobalModules(IAppHostWritableAdminManager* manager,
                                         BSTR image) {
  IAppHostElement*        parent = NULL;
  IAppHostElementCollection* collection = NULL;
  IAppHostElement*        element = NULL;

  BSTR bstr_section_name = SysAllocString(L"system.webServer/globalModules");
  BSTR bstr_config_path = SysAllocString(L"MACHINE/WEBROOT/APPHOST");
  BSTR bstr_element_name = SysAllocString(L"add");

  HRESULT hresult = S_OK;
  bool result = false;
  do {
    // Get <system.webServer/globalModules> section
    hresult = manager->GetAdminSection(bstr_section_name,
                                       bstr_config_path, &parent);
    if (FAILED(hresult)) {
      Log(EVENT_ERROR, "Unable to access globalModules configuration.");
      break;
    } else if (&parent == NULL) {
      Log(EVENT_ERROR, "Unable to get globalModules configuration."); 
      break;
    }

    // Get collection of <system.webServer/globalModules>
    hresult = parent->get_Collection(&collection);
    if (FAILED(hresult)) {
      Log(EVENT_ERROR, "Unable to access globalModules child collection.");
      break;
    } else if (&collection == NULL) {
      Log(EVENT_ERROR, "Unable to get globalModules child collection."); 
      break;
    }

    // create a global modules child element, like:
    // <add name="ModSecurityIIS", image="ModSecurityIIS.dll" />

    // First to detect old modules.
    if (!GetFromCollection(collection, L"name", kModuleName, &element)) {
      Log(EVENT_ERROR, "Failed to try detect old modules.");
      break;
    }

    // No old ModSecurity module, create/add one.
    if (element == NULL) {
      hresult = collection->CreateNewElement(bstr_element_name, &element);
      if (FAILED(hresult)) {
        Log(EVENT_ERROR, "Failed to create globalModules/add element.");
        break;
      }

      if (!SetProperty(element, L"name", kModuleName)) {
        Log(EVENT_ERROR, "Failed to set name property.");
        break;
      }
      // Add the new element to collection
      hresult = collection->AddElement(element);
      if (FAILED(hresult)) {
        Log(EVENT_ERROR, "Failed to add globalModule/add element.");
        break;
      }
    }

    if (!SetProperty(element, L"image", image)) {
      Log(EVENT_ERROR, "Failed to set image property.");
      break;
    }

    result = true;
  } while (false);

  // Exiting / Unwinding
  if (element != NULL) element->Release();
  if (collection != NULL) collection->Release();
  if (parent != NULL) parent->Release();

  SysFreeString(bstr_section_name);
  SysFreeString(bstr_config_path);
  SysFreeString(bstr_element_name);

  return result;
}


// Add an entry to <system.webServer/modules> section, like:
// <add name="ModSecurityIIS" />
bool Iis7Config::AddToModules(IAppHostWritableAdminManager* manager) {
  IAppHostElement*        parent = NULL;
  IAppHostElementCollection* collection = NULL;
  IAppHostElement*        element = NULL;

  BSTR bstr_section_name = SysAllocString(L"system.webServer/modules");
  BSTR bstr_config_path = SysAllocString(L"MACHINE/WEBROOT/APPHOST");
  BSTR bstr_element_name = SysAllocString(L"add");

  HRESULT hresult = S_OK;
  bool result = false;
  do {
    // Get <system.webServer/modules> section
    hresult = manager->GetAdminSection(bstr_section_name,
                                       bstr_config_path, &parent);
    if (FAILED(hresult)) {
      Log(EVENT_ERROR, "Unable to access modules configuration.");
      break;
    } else if (&parent == NULL) {
      Log(EVENT_ERROR, "Unable to get modules configuration."); 
      break;
    }

    // Get collection of <system.webServer/modules>
    hresult = parent->get_Collection(&collection);
    if (FAILED(hresult)) {
      Log(EVENT_ERROR, "Unable to access modules child collection.");
      break;
    } else if (&collection == NULL) {
      Log(EVENT_ERROR, "Unable to get modules child collection."); 
      break;
    }

    // Try to detect old ModSecurity module.
    if (!GetFromCollection(collection, L"name", kModuleName, &element)) {
      Log(EVENT_ERROR, "Failed to try detect old modules.");
      break;
    }

    // No old ModSecurity module exists.
    if (element == NULL) {
      hresult = collection->CreateNewElement(bstr_element_name, &element);
      if (FAILED(hresult)) {
        Log(EVENT_ERROR, "Failed to create modules/add element.");
        break;
      }

      if (!SetProperty(element, L"name", kModuleName)) {
        Log(EVENT_ERROR, "Failed to set name property.");
        break;
      }

      // Add the new element to collection
      hresult = collection->AddElement(element);
      if (FAILED(hresult)) {
        Log(EVENT_ERROR, "Failed to add modules/add element.");
        break;
      }
    }

    result = true;
  } while (false);

  // Exiting / Unwinding
  if (element != NULL) element->Release();
  if (collection != NULL) collection->Release();
  if (parent != NULL) parent->Release();

  SysFreeString(bstr_section_name);
  SysFreeString(bstr_config_path);
  SysFreeString(bstr_element_name);

  return result;
}

bool Iis7Config::RemoveFromCollection(IAppHostWritableAdminManager* manager,
                                     BSTR section, BSTR property_name, BSTR property_value) {
  IAppHostElement*        parent = NULL;
  IAppHostElementCollection* collection = NULL;
  IAppHostElement*        element = NULL;
  IAppHostProperty*       name_property = NULL;

  BSTR bstr_section_name = SysAllocString(section);
  BSTR bstr_config_path = SysAllocString(L"MACHINE/WEBROOT/APPHOST");

  HRESULT hresult = S_OK;
  bool result = false;
  do {
    // Get section contains ModSecurityModule entry
    hresult = manager->GetAdminSection(bstr_section_name,
                                       bstr_config_path, &parent);
    if (FAILED(hresult)) {
      Log(EVENT_ERROR, "Unable to access section to remove module.");
      break;
    } else if (&parent == NULL) {
      Log(EVENT_ERROR, "Unable to get section to remove module."); 
      break;
    }

    // Get child collection of the section
    hresult = parent->get_Collection(&collection);
    if (FAILED(hresult)) {
      Log(EVENT_ERROR, "Unable to access collection to remove module.");
      break;
    } else if (&collection == NULL) {
      Log(EVENT_ERROR, "Unable to get collection to remove module."); 
      break;
    }

    // Get number of children.
    DWORD count;
    hresult = collection->get_Count(&count);
    if (FAILED(hresult)) {
      Log(EVENT_ERROR, "Unable to get the count of collection.");
      break;
    }

    short sitemap_index;
    if (!GetFromCollection(collection, property_name, property_value, &sitemap_index)) {
      Log(EVENT_ERROR, "Failed to find ModSecurity module.");
      break;
    }

    if (sitemap_index != -1) {
      VARIANT var_index;
      var_index.vt = VT_I2;
      var_index.iVal = sitemap_index;

      hresult = collection->DeleteElement(var_index);
      if (FAILED(hresult)) {
        Log(EVENT_ERROR, "Failed to remove ModSecurity module.");
      }
    } else {
      Log(EVENT_IMPORTANT, "No ModSecurity module is found.");
    }

    result = SUCCEEDED(hresult);
  } while (false);

  // Exiting / Unwinding
  if (name_property != NULL) name_property->Release();
  if (element != NULL) element->Release();
  if (collection != NULL) collection->Release();
  if (parent != NULL) parent->Release();

  SysFreeString(bstr_section_name);
  SysFreeString(bstr_config_path);

  return result;
}
/*
bool Iis7Config::LoadSite(IAppHostElement* element) {
  std::string name, id, phys_path, log_path;

  // Get name.
  VARIANT name_var;
  if (!GetProperty(element, L"name", &name_var)) {
    Log(EVENT_ERROR, "Failed to get name property value.");
    return false;
  } else if (name_var.vt != VT_BSTR) {
    Log(EVENT_ERROR, "site name should be VT_BSTR");
    return false;
  } else {
    name = static_cast<const char*>(bstr_t(name_var.bstrVal));
  }

  // Get id.
  VARIANT id_var;
  if (!GetProperty(element, L"id", &id_var)) {
    Log(EVENT_ERROR, "Failed to get id value.");
    return false;
  } else if (id_var.vt == VT_I4) {
    char buffer[16];
    itoa(id_var.intVal, buffer, 10);
    id = buffer;
  } else if (id_var.vt == VT_UI4) {
    char buffer[16];
    itoa(id_var.uintVal, buffer, 10);
    id = buffer;
  } else if (id_var.vt == VT_BSTR) {
    id = static_cast<const char*>(bstr_t(id_var.bstrVal));
  } else {
    Log(EVENT_ERROR, "site id should be VT_BSTR or VT_I4");
  }

  // Get logfile directory.
  IAppHostElement* logfile_element = NULL;
  HRESULT hresult = element->GetElementByName(L"logfile", &logfile_element);
  if (FAILED(hresult)) {
    Log(EVENT_ERROR, "Failed to get logfile element.");
  } else {
    VARIANT dir_var;
    if (!GetProperty(logfile_element, L"directory", &dir_var)) {
      Log(EVENT_ERROR, "Failed to get directory value.");
    } else if (dir_var.vt == VT_BSTR) {
      log_path = static_cast<const char*>(bstr_t(dir_var.bstrVal));
      log_path.append("\\W3SVC").append(id);
    } else {
      Log(EVENT_ERROR, "directory should be VT_BSTR");
    }
  }

  // Get pysical path.
  IAppHostElementCollection* application_collection = NULL;
  IAppHostElementCollection* virtualdir_collection = NULL;
  IAppHostElement* application_element = NULL;
  IAppHostElement* virtualdir_element = NULL;

  hresult = S_OK;
  bool result = false;
  do {
    // Get <application> collection in <site> element
    hresult = element->get_Collection(&application_collection);
    if (FAILED(hresult)) {
      Log(EVENT_ERROR, "Failed to get application collection.");
      break;
    }

    // Find <application> element wiht path="/"
    short root_app = -1;
    if (!GetFromCollection(application_collection, L"path", L"/", &root_app)) {
      Log(EVENT_ERROR, "Failed to get root application index.");
      break;
    } else if (root_app == -1) {
      Log(EVENT_ERROR, "No root application defined.");
      break;
    }

    // Get the root <application> element
    VARIANT var_rootapp;
    var_rootapp.vt = VT_I2;
    var_rootapp.iVal = root_app;
    hresult = application_collection->get_Item(var_rootapp, &application_element);
    if (FAILED(hresult)) {
      Log(EVENT_ERROR, "Failed to get root application element.");
      break;
    }

    // Get <virtualDirectory> collection in <application> element.
    hresult = application_element->get_Collection(&virtualdir_collection);
    if (FAILED(hresult)) {
      Log(EVENT_ERROR, "Failed to get virtualdir collection.");
      break;
    }

    // Find <virtualDirectory> element with path="/"
    short root_dir = -1;
    if (!GetFromCollection(virtualdir_collection, L"path", L"/", &root_dir)) {
      Log(EVENT_ERROR, "Failed to get root virtualDirectory index.");
      break;
    } else if (root_dir == -1) {
      Log(EVENT_ERROR, "No root virtualDirectory defined.");
      break;
    }

    // Get the root <virtualDirectory> element.
    VARIANT var_rootdir;
    var_rootdir.vt = VT_I2;
    var_rootdir.iVal = root_dir;
    hresult = virtualdir_collection->get_Item(var_rootdir, &virtualdir_element);
    if (FAILED(hresult)) {
      Log(EVENT_ERROR, "Failed to get root virtualDirectory element.");
      break;
    }

    VARIANT var_path;
    if (!GetProperty(virtualdir_element, L"physicalPath", &var_path)) {
      Log(EVENT_ERROR, "Failed to get physicalPath property.");
      break;
    } else if (var_path.vt != VT_BSTR) {
      Log(EVENT_ERROR, "physicalPath should be instance of VT_BSTR.");
      break;
    }

    phys_path = static_cast<const char*>(bstr_t(var_path.bstrVal));
    result = true;
  } while (false);

  if (logfile_element != NULL) logfile_element->Release();
  if (application_collection != NULL) application_collection->Release();
  if (virtualdir_collection != NULL) virtualdir_collection->Release();
  if (application_element != NULL) application_element->Release();
  if (virtualdir_element != NULL) virtualdir_element->Release();

  // At last we store all the values.
  if (result) {
    site_ids_.push_back(id);
    names_.push_back(name);
    physical_paths_.push_back(phys_path);
    log_paths_.push_back(log_path);
    host_urls_.push_back("");
  }

  return result;
}
*/
bool Iis7Config::GetFromCollection(IAppHostElementCollection* collection,
                                   BSTR property_key, BSTR property_value,
                                   short* index) {
    IAppHostProperty* property = NULL;
    IAppHostElement* element = NULL;

    HRESULT hresult = S_OK;
    *index = -1;
    do {
      // Get number of children.
      DWORD count;
      hresult = collection->get_Count(&count);
      if (FAILED(hresult)) {
        Log(EVENT_ERROR, "Unable to get the count of collection.");
        break;
      }

      // Iterate every child to check whether it should be removed.
      for (USHORT i = 0; i < count && SUCCEEDED(hresult); ++i) {
        // Get item at index i.
        VARIANT idx;
        idx.vt = VT_I2;
        idx.iVal = i;
        hresult = collection->get_Item(idx, &element);
        if (FAILED(hresult)) {
          Log(EVENT_ERROR, "Unable to get item (%d).", i);
          break;
        }

        // Get name property of item i.
        VARIANT var_value;
        if (!GetProperty(element, property_key, &var_value)) {
          Log(EVENT_ERROR, "Failed to get property value.");
          hresult = S_FALSE;
          break;
        }

        // Check the property value
        if (wcscmp(property_value, var_value.bstrVal) == 0) {
          *index = static_cast<short>(i);
          break;
        }

        element->Release();
        element = NULL;
      }
    } while (false);

    if (property != NULL) property->Release();
    if (element != NULL) element->Release();

    return SUCCEEDED(hresult);
}

bool Iis7Config::GetFromCollection(IAppHostElementCollection* collection,
                                   BSTR property_key, BSTR property_value,
                                   IAppHostElement** element) {
  short idx;
  if (!GetFromCollection(collection, property_key, property_value, &idx)) {
    Log(EVENT_ERROR, "Failed to get child from collection.");
  }

  if (idx != -1) {
    VARIANT idx_var;
    idx_var.vt = VT_I2;
    idx_var.iVal = idx;

    HRESULT hresult = collection->get_Item(idx_var, element);
    if (FAILED(hresult)) {
      Log(EVENT_ERROR, "Failed to get element from collection.");
      return false;
    } else {
      return true;
    }
  } else {
    *element = NULL;
    return true;
  }
}


bool Iis7Config::SetProperty(IAppHostElement* element, BSTR name, BSTR value) {
  IAppHostProperty* property = NULL;
  BSTR bstr_name = SysAllocString(name);
  BSTR bstr_value = SysAllocString(value);

  HRESULT hresult = S_OK;
  do {
	  // Get the property by name.
	  hresult = element->GetPropertyByName(bstr_name, &property);
	  if (FAILED(hresult)) {
		  Log(EVENT_ERROR, "Failed to get property.");
       break;
	  }

    // Set the property value.
    VARIANT value_variant;
    value_variant.vt = VT_BSTR;
    value_variant.bstrVal = bstr_value;
	  hresult = property->put_Value(value_variant);
    if (FAILED(hresult)) {
      Log(EVENT_ERROR, "Failed to set property value.");
	    break;
    }
  } while (false);

  if (property != NULL) property->Release();
  SysFreeString(bstr_name);
  SysFreeString(bstr_value);

  return SUCCEEDED(hresult);
}

bool Iis7Config::GetProperty(IAppHostElement* element, BSTR name, VARIANT* value) {
  IAppHostProperty* property = NULL;
  BSTR bstr_name = SysAllocString(name);

  HRESULT hresult = S_OK;
  do {
	  // Get the property by name.
	  hresult = element->GetPropertyByName(bstr_name, &property);
	  if (FAILED(hresult)) {
		  Log(EVENT_ERROR, "Failed to get property.");
       break;
	  }

    // Get the property value.
	  hresult = property->get_Value(value);
    if (FAILED(hresult)) {
      Log(EVENT_ERROR, "Failed to get property value.");
	    break;
    }
  } while (false);

  if (property != NULL) property->Release();
  SysFreeString(bstr_name);

  return SUCCEEDED(hresult);
}


bool Iis7Config::IsSupported() {
  HRESULT hr = CoInitializeEx(NULL, COINIT_MULTITHREADED);
  if (FAILED(hr)) {
    Log(EVENT_ERROR, "Failed to check IIS 7 compatibility. 0x%0x", hr);
    return false;
  }

  // Try with IIS 7.0...
  IAppHostAdminManager* admin_manager = NULL;
  hr = CoCreateInstance(__uuidof(AppHostAdminManager), NULL, 
    CLSCTX_INPROC_SERVER, __uuidof(IAppHostAdminManager),
    reinterpret_cast<LPVOID*>(&admin_manager));
  if (SUCCEEDED(hr)) {
    admin_manager->Release();
  }

  CoUninitialize();

  return SUCCEEDED(hr);
}

int Iis7Config::GetNativeSystemInfo(SYSTEM_INFO* system_info) {
  HMODULE module = GetModuleHandle(L"Kernel32.dll");
  if (module == NULL) {
    Log(EVENT_ERROR, "Failed to GetModuleHandle of Kernel32.dll. (%d)",
              GetLastError());
    return -1;
  }

  typedef void (WINAPI *Procedure)(LPSYSTEM_INFO);
  Procedure proc = (Procedure) GetProcAddress(module, "GetNativeSystemInfo");
  if (proc == NULL) {
    if (GetLastError() == ERROR_PROC_NOT_FOUND) {
      return 0;
    } else {
      Log(EVENT_ERROR, "Failed to GetProcAddress (%d).", GetLastError());
      return -1;
    }
    return 0;
  } else {
    proc(system_info);
    return 1;
  }
}

Iis7Config::AppMode Iis7Config::GetAppMode() {
  SYSTEM_INFO sys_info;
  if (Iis7Config::GetNativeSystemInfo(&sys_info) != 1) {
    Log(EVENT_ERROR, "Failed to GetNativeSystemInfo for IIS7.");
    return MODE_FAIL;
  }

  if (sys_info.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_INTEL) {
    return MODE_X86;
  } else if (sys_info.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_AMD64) {
    return MODE_X64;
  } else {
    return MODE_UNKNOWN;
  }
}
/*
bool Iis7Config::InstallAdminConsole(const char* site_root,
                                     const char* cgi_path) {
  IAppHostWritableAdminManager*   admin_manager = NULL;
  bool result = false;
  do {
    // Initialize com
    CHECK_HRESULT(CoInitializeEx(NULL, COINIT_MULTITHREADED), "Failed to intialize COM.");

    // Create a writable admin manager
    CHECK_HRESULT(CoCreateInstance(__uuidof(AppHostWritableAdminManager), NULL, 
              CLSCTX_INPROC_SERVER,
              __uuidof(IAppHostWritableAdminManager),
              reinterpret_cast<LPVOID*>(&admin_manager)),
              "Failed to create AppHostWritableAdminManager.");

    // set commit path
    CHECK_HRESULT(admin_manager->put_CommitPath(L"MACHINE/WEBROOT/APPHOST"),
                  "Failed to put commit path.");

    
    CHECK_BOOL(CreateAdminConsoleSite(admin_manager, bstr_t(site_root)),
               "Failed to create AdminConsole Site.");
    CHECK_BOOL(SetCGIRestriction(admin_manager, bstr_t(cgi_path)),
               "Failed to set CGI restriction.");
    CHECK_BOOL(CustomizeAdminConsole(admin_manager, bstr_t(cgi_path)),
               "Failed to customize admin console.");

    // Save changes.
    CHECK_HRESULT(admin_manager->CommitChanges(),
                  "Failed to save changes to install module.");

    Log(EVENT_CRITICAL, "Admin Console installed successfully.");
    result = true;
  } while (false);

  // Exiting / Unwinding
  if (admin_manager != NULL) admin_manager->Release();

  CoUninitialize();

  return result;
}

bool Iis7Config::UninstallAdminConsole(const char* site_root,
                                       const char* cgi_path) {
  IAppHostWritableAdminManager*   admin_manager = NULL;
  BSTR bstr_config_path = SysAllocString(L"MACHINE/WEBROOT/APPHOST");

  bool result = false;
  do {
    // Initialize com
    CHECK_HRESULT(CoInitializeEx(NULL, COINIT_MULTITHREADED), "Failed to intialize COM.");

    // Create a writable admin manager
    CHECK_HRESULT(CoCreateInstance(__uuidof(AppHostWritableAdminManager), NULL, 
              CLSCTX_INPROC_SERVER,
              __uuidof(IAppHostWritableAdminManager),
              reinterpret_cast<LPVOID*>(&admin_manager)),
              "Failed to create AppHostWritableAdminManager.");

    // set commit path
    CHECK_HRESULT(admin_manager->put_CommitPath(L"MACHINE/WEBROOT/APPHOST"),
                  "Failed to put commit path.");

    CHECK_BOOL(RemoveFromCollection(admin_manager, L"system.applicationHost/sites", L"name", kSiteName), "Failed to remove Admin console site.");
    CHECK_BOOL(RemoveFromCollection(admin_manager, L"system.webServer/security/isapiCgiRestriction", L"path", bstr_t(cgi_path)),
               "Failed to remove CGI restriction.");
    CHECK_BOOL(RemoveAdminConsoleCustomization(admin_manager),
               "Failed to remove customization of admin console.");

    // Save changes.
    CHECK_HRESULT(admin_manager->CommitChanges(),
                  "Failed to save changes to install module.");

    result = true;
  } while (false);

  // Exiting / Unwinding
  if (admin_manager != NULL) admin_manager->Release();

  SysFreeString(bstr_config_path);

  CoUninitialize();

  return result;
}


bool Iis7Config::SetCGIRestriction(IAppHostWritableAdminManager* manager, BSTR cgi_path) {
  IAppHostElement*        parent = NULL;
  IAppHostElementCollection* collection = NULL;
  IAppHostElement*        element = NULL;

  BSTR bstr_section_name = SysAllocString(L"system.webServer/security/isapiCgiRestriction");
  BSTR bstr_config_path = SysAllocString(L"MACHINE/WEBROOT/APPHOST");
  BSTR bstr_element_name = SysAllocString(L"add");

  HRESULT hresult = S_OK;
  bool result = false;
  do {
    // Get <system.webServer/security/isapiCgiRestriction> section
    hresult = manager->GetAdminSection(bstr_section_name,
                                       bstr_config_path, &parent);
    if (FAILED(hresult)) {
      Log(EVENT_ERROR, "Unable to access isapiCgiRestriction configuration. (0x%0x)", hresult);
      break;
    } else if (&parent == NULL) {
      Log(EVENT_ERROR, "Unable to get isapiCgiRestriction configuration."); 
      break;
    }

    // Get collection of <system.webServer/security/isapiCgiRestriction>
    hresult = parent->get_Collection(&collection);
    if (FAILED(hresult)) {
      Log(EVENT_ERROR, "Unable to access isapiCgiRestriction child collection. (0x%0x)", hresult);
      break;
    } else if (&collection == NULL) {
      Log(EVENT_ERROR, "Unable to get isapiCgiRestriction child collection."); 
      break;
    }

    // create a child element, like:
    // <add path="C:\Program Fils\GSG\AdminConsoleCGI.exe" />

    // First to detect old cgi restriction.
    if (!GetFromCollection(collection, L"path", cgi_path, &element)) {
      Log(EVENT_ERROR, "Failed to try detect old cgi restriction.");
      break;
    }

    // No old cgi restriction, create one.
    if (element == NULL) {
      hresult = collection->CreateNewElement(bstr_element_name, &element);
      if (FAILED(hresult)) {
        Log(EVENT_ERROR, "Failed to create isapiCgiRestriction/add element. (0x%0x)", hresult);
        break;
      }

      if (!SetProperty(element, L"path", cgi_path)) {
        Log(EVENT_ERROR, "Failed to set path property.");
        break;
      }
      
      if (!SetProperty(element, L"allowed", L"true")) {
        Log(EVENT_ERROR, "Failed to set allowed property.");
        break;
      }

      // Add the new element to collection
      hresult = collection->AddElement(element);
      if (FAILED(hresult)) {
        Log(EVENT_ERROR, "Failed to add isapiCgiRestriction/add element. (0x%0x)", hresult);
        break;
      }
    }

    Log(EVENT_NORMAL, "CGIRestriction set properly.");
    result = true;
  } while (false);

  // Exiting / Unwinding
  if (element != NULL) element->Release();
  if (collection != NULL) collection->Release();
  if (parent != NULL) parent->Release();

  SysFreeString(bstr_section_name);
  SysFreeString(bstr_config_path);
  SysFreeString(bstr_element_name);

  return result;
}


bool Iis7Config::CreateAdminConsoleSite(IAppHostWritableAdminManager *manager,  BSTR site_root) {
  IAppHostElement*        parent = NULL;
  IAppHostElementCollection* collection = NULL;
  IAppHostElement*        element = NULL;

  BSTR bstr_section_name = SysAllocString(L"system.applicationHost/sites");
  BSTR bstr_config_path = SysAllocString(L"MACHINE/WEBROOT/APPHOST");

  HRESULT hresult = S_OK;
  bool result = false;
  do {
    // Get <system.applicationHost/sites> section
    hresult = manager->GetAdminSection(bstr_section_name,
                                       bstr_config_path, &parent);
    if (FAILED(hresult)) {
      Log(EVENT_ERROR, "Unable to access sites configuration. (0x%0x)", hresult);
      break;
    } else if (&parent == NULL) {
      Log(EVENT_ERROR, "Unable to get sites configuration."); 
      break;
    }

    // Get collection of <system.applicationHost/sites>
    hresult = parent->get_Collection(&collection);
    if (FAILED(hresult)) {
      Log(EVENT_ERROR, "Unable to access sites child collection. (0x%0x)", hresult);
      break;
    } else if (&collection == NULL) {
      Log(EVENT_ERROR, "Unable to get sites child collection."); 
      break;
    }

    // create a <site> element, like:

    // First to detect old site.
    if (!GetFromCollection(collection, L"name", kSiteName, &element)) {
      Log(EVENT_ERROR, "Failed to try detect old site.");
      break;
    }
    if (element != NULL) {
      Log(EVENT_CRITICAL, "Old Admin Console Site exists. Skip.");
      result = true;
      break;
    }

    DWORD site_count;
    CHECK_HRESULT(collection->get_Count(&site_count),
                  "Failed to get site count.");
    char site_id[64];
    itoa(site_count + 466453, site_id, 10);

    CHECK_HRESULT(collection->CreateNewElement(L"site", &element),
                  "Failed to create site element.");
    CHECK_BOOL(SetProperty(element, L"name", kSiteName),
               "Failed to set site name property.");
    CHECK_BOOL(SetProperty(element, L"id", bstr_t(site_id)),
               "Failed to set site id property.");
    CHECK_HRESULT(collection->AddElement(element),
                  "Failed to add site element.");

    // Create and set <application> element.
    IAppHostElementCollection* app_collection = NULL;
    IAppHostElement* app_element = NULL;
    CHECK_HRESULT(element->get_Collection(&app_collection),
                  "Failed to get app collection.");
    CHECK_HRESULT(app_collection->CreateNewElement(L"application", &app_element), 
                  "Failed to create application element.");
    CHECK_BOOL(SetProperty(app_element, L"path", L"/"),
               "Failed to set app path.");
    // CHECK_BOOL(SetProperty(app_element, L"applicationPool", kSiteName), "Failed to set app applicationPool.");
    CHECK_HRESULT(app_collection->AddElement(app_element),
                  "Failed to add application element.");

    // Create and set <virtualDirectory> element.
    IAppHostElementCollection* dir_collection = NULL;
    IAppHostElement* dir_element = NULL;
    CHECK_HRESULT(app_element->get_Collection(&dir_collection),
                  "Failed to get virtualDirectory collection.");
    CHECK_HRESULT(dir_collection->CreateNewElement(L"virtualDirectory", &dir_element),
                  "Failed to create virtualDirectory.");
    CHECK_BOOL(SetProperty(dir_element, L"path", L"/"),
               "Failed to set dir path.");
    CHECK_BOOL(SetProperty(dir_element, L"physicalPath", site_root),
               "Failed to set dir physicalPath.");
    CHECK_HRESULT(dir_collection->AddElement(dir_element),
                  "Failed to add virtualDirectory.");

    // Get <bindings> element.
    IAppHostElement* bindings_element = NULL;
    CHECK_HRESULT(element->GetElementByName(L"bindings", &bindings_element),
                  "Failed to get bindings element.");
    CHECK_BOOL((bindings_element != NULL),
               "Failed to find bindings element.");

    // Create and set <binding> element.
    IAppHostElementCollection* binding_collection = NULL;
    IAppHostElement* binding_element = NULL;
    CHECK_HRESULT(bindings_element->get_Collection(&binding_collection),
                  "Failed to get binding collection.");
    CHECK_HRESULT(binding_collection->CreateNewElement(L"binding", &binding_element),
                  "Failed to create binding.");
    CHECK_BOOL(SetProperty(binding_element, L"protocol", L"http"),
               "Failed to set binding protocol.");
    CHECK_BOOL(SetProperty(binding_element, L"bindingInformation", bstr_t(IisConfig::kAdminConsoleBinding)),
               "Failed to set binding bindingInformation.");
    CHECK_HRESULT(binding_collection->AddElement(binding_element),
                  "Failed to add binding element.");

    Log(EVENT_NORMAL, "Admin Console site created successfully.");
    result = true;
  } while (false);

  // Exiting / Unwinding
  if (element != NULL) element->Release();
  if (collection != NULL) collection->Release();
  if (parent != NULL) parent->Release();

  SysFreeString(bstr_section_name);
  SysFreeString(bstr_config_path);

  return result;
}
*/
bool Iis7Config::GetFromChildren(IAppHostChildElementCollection* children,
                                 BSTR child_name, short* index) {
  IAppHostElement* element = NULL;

  HRESULT hresult = S_OK;
  *index = -1;
  do {
    // Get number of children.
    DWORD count;
    hresult = children->get_Count(&count);
    if (FAILED(hresult)) {
      Log(EVENT_ERROR, "Unable to get the count of children.");
      break;
    }

    // Iterate every child to check whether it should be removed.
    for (USHORT i = 0; i < count && SUCCEEDED(hresult); ++i) {
      // Get item at index i.
      VARIANT idx;
      idx.vt = VT_I2;
      idx.iVal = i;
      hresult = children->get_Item(idx, &element);
      if (FAILED(hresult)) {
        Log(EVENT_ERROR, "Unable to get item [%d]. (0x%0x)", i, hresult);
        break;
      }

      // Get name property of item i.
      
      BSTR name = NULL;
      if (!element->get_Name(&name)) {
        Log(EVENT_ERROR, "Failed to get property value.");
        hresult = S_FALSE;
        break;
      }

      // Check the property value
      if (wcscmp(child_name, name) == 0) {
        *index = static_cast<short>(i);
        SysFreeString(name);
        break;
      }

      SysFreeString(name);
      element->Release();
      element = NULL;
    }
  } while (false);


  if (element != NULL) element->Release();

  return SUCCEEDED(hresult);  
}

bool Iis7Config::GetFromChildren(IAppHostChildElementCollection* children,
                                 BSTR child_name, IAppHostElement** element) {
  short idx;
  if (!GetFromChildren(children, child_name, &idx)) {
    Log(EVENT_ERROR, "Failed to get element from children.");
  }

  if (idx != -1) {
    VARIANT idx_var;
    idx_var.vt = VT_I2;
    idx_var.iVal = idx;

    HRESULT hresult = children->get_Item(idx_var, element);
    if (FAILED(hresult)) {
      Log(EVENT_ERROR, "Failed to get element from children.");
      return false;
    } else {
      return true;
    }
  } else {
    *element = NULL;
    return true;
  }
}
/*
bool Iis7Config::CustomizeAdminConsole(IAppHostWritableAdminManager* manager, BSTR cgi_path) {
  IAppHostConfigManager* config_manager = NULL;
  IAppHostConfigFile * config_file = NULL;
  IAppHostConfigLocationCollection* location_collection = NULL;
  IAppHostConfigLocation* location = NULL;
  IAppHostElement* handlers = NULL;
  IAppHostElementCollection* handlers_collection = NULL;
  IAppHostElement* remove_element = NULL;
  IAppHostElement* add_element = NULL;

  bool result = false;
  do {
    CHECK_HRESULT(manager->get_ConfigManager(&config_manager),
                  "Failed to get config manager.");
    CHECK_HRESULT(config_manager->GetConfigFile(L"MACHINE/WEBROOT/APPHOST", &config_file),
                  "Failed to get config file.");
    CHECK_HRESULT(config_file->get_Locations(&location_collection),
                  "Failed to get location collection.");

    bool success = false;
    do {
      // Get number of children.
      DWORD count;
      CHECK_HRESULT(location_collection->get_Count(&count), "Unable to get the count of collection.");

      // Iterate every child to check whether it should be removed.
      USHORT i = 0;
      for (; i < count; ++i) {
        // Get item at index i.
        VARIANT idx;
        idx.vt = VT_I2;
        idx.iVal = i;
        CHECK_HRESULT(location_collection->get_Item(idx, &location), "Unable to get item.");

        BSTR location_path;
        CHECK_HRESULT(location->get_Path(&location_path), "Failed to get location path.");
        if (wcscmp(location_path, kSiteName) == 0) {
          success = true;
          break;
        }

        SysFreeString(location_path);
        location->Release();
        location = NULL;
      }

      if (i == count) success = true;
    } while (false);

    if (!success) {
      break;
    }

    if (location != NULL) {
      Log(EVENT_CRITICAL, "Location already exists. Skip.");
      result = true;
      break;
    }

    CHECK_HRESULT(location_collection->AddLocation(kSiteName, &location),
                  "Failed to add location.");
    CHECK_HRESULT(location->AddConfigSection(L"system.webServer/handlers", &handlers),
                  "Failed to add config section."); 
    CHECK_HRESULT(SetProperty(handlers, L"accessPolicy", L"Read, Execute, Script"),
                  "Failed to set accessPolicy.");

    CHECK_HRESULT(handlers->get_Collection(&handlers_collection),
                  "Failed to get handlers collection.");
    CHECK_HRESULT(handlers_collection->CreateNewElement(L"remove", &remove_element),
                  "Failed to create remove element.");
    CHECK_BOOL(SetProperty(remove_element, L"name", L"CGI-exe"),
               "Failed to set name of remove element.");
    CHECK_HRESULT(handlers_collection->AddElement(remove_element),
                  "Failed to add remove element.");
    
    CHECK_HRESULT(handlers_collection->CreateNewElement(L"add", &add_element),
                  "Failed to create add element.");
    CHECK_BOOL(SetProperty(add_element, L"name", L"CGI-exe"),
               "Failed to set name of add element.");
    CHECK_BOOL(SetProperty(add_element, L"path", L"*.cgi"),
               "Failed to set path of add element.");
    CHECK_BOOL(SetProperty(add_element, L"verb", L"*"),
               "Failed to set verb of  element.");
    CHECK_BOOL(SetProperty(add_element, L"modules", L"CgiModule"),
               "Failed to set modules of add element.");
    CHECK_BOOL(SetProperty(add_element, L"scriptProcessor", cgi_path),
               "Failed to set scriptProcessor of add element.");
    CHECK_BOOL(SetProperty(add_element, L"resourceType", L"File"),
               "Failed to set resourceType of add element.");
    CHECK_BOOL(SetProperty(add_element, L"requireAccess", L"Execute"),
               "Failed to set requireAccess of add element.");
    CHECK_BOOL(SetProperty(add_element, L"allowPathInfo", L"true"),
               "Failed to set naallowPathInfome of add element.");
    CHECK_HRESULT(handlers_collection->AddElement(add_element),
                  "Failed to add add element.");

    Log(EVENT_NORMAL, "Admin Console site customized successfully.");
    result = true;
  } while (false);

  return result;
}


bool Iis7Config::RemoveAdminConsoleCustomization(IAppHostWritableAdminManager* manager) {
  IAppHostConfigManager* config_manager = NULL;
  IAppHostConfigFile * config_file = NULL;
  IAppHostConfigLocationCollection* location_collection = NULL;
  IAppHostConfigLocation* location = NULL;

  bool result = false;
  do {
    CHECK_HRESULT(manager->get_ConfigManager(&config_manager), "Failed to get config manager.");
    CHECK_HRESULT(config_manager->GetConfigFile(L"MACHINE/WEBROOT/APPHOST", &config_file), "Failed to get config file.");
    CHECK_HRESULT(config_file->get_Locations(&location_collection), "Failed to get location collection.");

    bool success = false;
    VARIANT idx;
    do {
      // Get number of children.
      DWORD count;
      CHECK_HRESULT(location_collection->get_Count(&count), "Unable to get the count of collection.");

      // Iterate every child to check whether it should be removed.
      USHORT i = 0;
      for (; i < count; ++i) {
        // Get item at index i.
        idx.vt = VT_I2;
        idx.iVal = i;
        CHECK_HRESULT(location_collection->get_Item(idx, &location), "Unable to get item.");

        BSTR location_path;
        CHECK_HRESULT(location->get_Path(&location_path), "Failed to get location path.");
        if (wcscmp(location_path, kSiteName) == 0) {
          success = true;
          break;
        }

        SysFreeString(location_path);
        location->Release();
        location = NULL;
      }

      if (i == count) success = true;
    } while (false);

    if (!success) {
      break;
    }

    if (location == NULL) {
      Log(EVENT_CRITICAL, "No location exists. Skip.");
      result = true;
      break;
    }

    CHECK_HRESULT(location_collection->DeleteLocation(idx), "Failed to remove location.");

    result = true;
  } while (false);

  return result;
}
*/

void CopyBinaries(wchar_t *srcpath, wchar_t *dstpath)
{
    wchar_t *files[] = { L"libapr-1.dll", L"libapriconv-1.dll", L"libaprutil-1.dll", 
                        L"libxml2.dll", L"lua5.1.dll", L"pcre.dll", L"zlib1.dll", L"ModSecurityIIS.dll" };

	for(int i = 0; i < 8; i++)
	{
		wchar_t src[4096];
		wchar_t dst[4096];

		wcsncpy(src,srcpath,4096);
		wcsncat(src,files[i],4096);
		wcsncpy(dst,dstpath,4096);
		wcsncat(dst,files[i],4096);

		if(!CopyFile(src, dst, FALSE))
		{
			Iis7Config::Log(EVENT_ERROR, L"Failed to copy binary: %s %s", src, dst);
		}
	}
}

void fixpath(wchar_t *srcdir)
{
	int l = wcslen(srcdir);

	if(srcdir[l-1] != '\\')
	{
		srcdir[l] = '\\';
		srcdir[l + 1] = 0;
	}
}

int _tmain(int argc, _TCHAR* argv[])
{
	if(argc < 2)
		return -1;

	if(wcscmp(argv[1], L"uninstall") == 0)
	{
		Iis7Config::UninstallFilter();
		return 0;
	}

	wchar_t srcdir[4096];

	if(argv[1][0] == '"')
		wcsncpy(srcdir, &argv[1][1], 4096);
	else
		wcsncpy(srcdir, argv[1], 4096);

	int l = wcslen(srcdir);

	if(srcdir[l-1] == '"')
		srcdir[l-1] = 0;

	fixpath(srcdir);

	wchar_t windir[4096];

	GetWindowsDirectory(windir, 4096);

	fixpath(windir);

	wchar_t crsdir[4096];

	crsdir[0] = windir[0];
	crsdir[1] = windir[1];
	crsdir[2] = windir[2];
	crsdir[3] = 0;

	wcsncat(crsdir, L"inetpub", 4096);

	CreateDirectory(crsdir, NULL);

	wcsncat(crsdir, L"\\wwwroot", 4096);

	CreateDirectory(crsdir, NULL);

	wchar_t zipdir[4096];

	wcsncpy(zipdir, srcdir, 4096);
	wcsncat(zipdir, L"owasp_crs.zip", 4096);

	HZIP hzip = OpenZip(crsdir, zipdir, 0, ZIP_FILENAME);

	if(hzip != 0)
	{
		int z = 0;
		ZIPENTRYW ze;

		while(GetZipItemW(hzip, z, &ze) == ZR_OK)
		{
			UnzipItem(hzip, z++, ze.name, 0, ZIP_FILENAME);
		}

		CloseZip(hzip);
	}

	wchar_t sys32[4096];
	wchar_t syswow[4096];
	wchar_t sysnat[4096];

	wcsncpy(sys32, windir, 4096);
	wcsncpy(syswow, windir, 4096);
	wcsncpy(sysnat, windir, 4096);

	wcsncat(sys32, L"system32\\inetsrv\\", 4096);
	wcsncat(syswow, L"SysWow64\\inetsrv\\", 4096);
	wcsncat(sysnat, L"SysNative\\inetsrv\\", 4096);

	wchar_t src32[4096];
	wchar_t src64[4096];

	wcsncpy(src32, srcdir, 4096);
	wcsncpy(src64, srcdir, 4096);
	wcsncat(src32, L"x86\\", 4096);
	wcsncat(src64, L"amd64\\", 4096);

	wchar_t dstconf[4096];

	if(Iis7Config::GetAppMode() == Iis7Config::MODE_X64)
	{
		CopyBinaries(src32, syswow);
		CopyBinaries(src64, sysnat);
		wcsncpy(dstconf, sysnat, 4096);
	}
	else
	{
		CopyBinaries(src32, sys32);
		wcsncpy(dstconf, sys32, 4096);
	}

	wcsncat(dstconf, L"config\\schema\\ModSecurity.xml", 4096);

	wcsncat(srcdir, L"ModSecurity.xml", 4096);

	if(!CopyFile(srcdir, dstconf, FALSE))
	{
		Iis7Config::Log(EVENT_ERROR, L"Failed to copy config XML: %s %s", srcdir, dstconf);
	}

	wcsncat(sys32, L"ModSecurityIIS.dll", 4096);

	Iis7Config::InstallFilter(sys32);

	return 0;
}
