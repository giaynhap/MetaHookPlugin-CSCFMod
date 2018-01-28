typedef struct tga_s
{
	byte	id_length;
	byte	colormap_type;
	byte	image_type;
	word	colormap_index;
	word	colormap_length;
	byte	colormap_size;
	word	x_origin;
	word	y_origin;
	word	width;
	word	height;
	byte	pixel_size;
	byte	attributes;
} tga_t;

class Graphic
{
	static const int MAX_IMAGES = 1024;
	typedef struct image_s
	{
		int imageId;
		int width,height;
		int rgba;
		char *name;
	};
public:
	Graphic();
	~Graphic();
public:
	int LoadTgaImage(char *szFileName);
	void DrawTgaImageDefault(int textureId,int x, int y, int alpha = 255,int r = 255,int g = 255 ,int b = 255);
	void DrawTgaImageScale(int textureId,int x,int y,float scale,int alpha = 255);
	image_s GetTexture(int index);
	int IsExists(char *szFileName);
private:
	int CreateNewTexture(bool useCustomId = true);
	void TextureQualityFilter(bool goodGuality = true);
private:
	image_s _imageList[MAX_IMAGES+1];
	int _imageIndex ;
	int _autoIncreasedTextureIndex;
};
extern Graphic *gGraphic;