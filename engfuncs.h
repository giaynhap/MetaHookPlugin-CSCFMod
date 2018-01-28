struct model_s *Engfunc_LoadMapSprite(const char *pszPath);
byte *Engfunc_COM_LoadFile(char *pszPath, int iUseHunk, int *piLength);
char *Engfunc_COM_ParseFile(char *pszData, char *pszToken);
void Engfunc_COM_FreeFile(void *pBuffer);
int EngFunc_HookUserMsg(char *szMsgName, pfnUserMsgHook pfn);
int	Engfunc_ClientCmd( char *szCmdString );
void Engfunc_WeaponAnim( int sequence, int body );
;
void IN_Accumulate(void);
void IN_MouseEvent(int x);
#define SCOREATTRIB_DEAD    ( 1 << 0 )
#define SCOREATTRIB_BOMB    ( 1 << 1 )