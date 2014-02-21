// dllmain.h : 模块类的声明。

class CmagicRegisterServerModule : public CAtlDllModuleT< CmagicRegisterServerModule >
{
public :
	DECLARE_LIBID(LIBID_magicRegisterServerLib)
	DECLARE_REGISTRY_APPID_RESOURCEID(IDR_MAGICREGISTERSERVER, "{88AD0587-F61A-46CC-ABF8-B6C8E4DE6153}")
};

extern class CmagicRegisterServerModule _AtlModule;
