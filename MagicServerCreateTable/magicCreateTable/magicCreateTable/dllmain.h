// dllmain.h : 模块类的声明。

class CmagicCreateTableModule : public CAtlDllModuleT< CmagicCreateTableModule >
{
public :
	DECLARE_LIBID(LIBID_magicCreateTableLib)
	DECLARE_REGISTRY_APPID_RESOURCEID(IDR_MAGICCREATETABLE, "{605C5627-5F78-4588-9090-3F395FAFD4DA}")
};

extern class CmagicCreateTableModule _AtlModule;
