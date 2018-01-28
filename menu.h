#ifndef MENU_H
#define MENU_H
float g_flTime=5.0;
extern char g_Menu_text[1024];
extern float g_Menu_Drawing_time;
extern int g_Menu_Can_Press[10];
int g_Menu_Drawing;
int g_Menu_WaitingForMore;
char g_Menu_text[1024];
void DrawMenu(void);
void GetHeightWidth(void);
char *BufferedLocaliseTextString(const char *msg);
char *LocaliseTextString(const char *msg, char *dst_buffer, int buffer_size);
inline client_textmessage_t *TextMessageGet(const char *pName) { return gEngfuncs.pfnTextMessageGet(pName); }

#endif