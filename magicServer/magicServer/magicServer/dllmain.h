// dllmain.h : 模块类的声明。

class CmagicServerModule : public CAtlDllModuleT< CmagicServerModule >
{
public :
	DECLARE_LIBID(LIBID_magicServerLib)
	DECLARE_REGISTRY_APPID_RESOURCEID(IDR_MAGICSERVER, "{0FDC1E65-3560-400A-A905-65C9158BA412}")
};

extern class CmagicServerModule _AtlModule;
