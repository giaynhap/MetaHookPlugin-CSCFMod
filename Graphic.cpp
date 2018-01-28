#include <metahook.h>
#include "exportfuncs.h"
#include "Graphic.h"
#include "gl/gl.h"
#include "triangleapi.h"

Graphic *gGraphic;

Graphic::Graphic()
{
	// Initialize Struct
	_imageIndex = 1;
	_autoIncreasedTextureIndex = 8000;
	memset(_imageList,0,sizeof(_imageList));
}
Graphic::~Graphic()
{
	for(int i=1 ;i <_imageIndex;i++ )
	{
		if(_imageList[i].name!=NULL)
		{
			delete[] _imageList[_imageIndex].name;
		}
	}
}
int Graphic::CreateNewTexture(bool useCustomId)
{
	if(useCustomId == false)
	{
		GLuint newId = 0;
		glGenTextures(1,&newId);
		return newId;
	}
	return _autoIncreasedTextureIndex++; // check texture whether is binded
}
Graphic::image_s Graphic::GetTexture(int index)
{
	return _imageList[index];
}
void Graphic::DrawTgaImageScale(int textureId,int x,int y,float scale,int alpha)
{
	if(textureId < 1 || textureId >= _imageIndex) return;
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	//glEnable(GL_ALPHA_TEST);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); 
	

	glBindTexture(GL_TEXTURE_2D, _imageList[textureId].imageId);
	glColor4ub(255,255,255,alpha);

	glBegin(GL_QUADS);
	glTexCoord2f(0,1);
	glVertex3f(x,y+_imageList[textureId].height*scale,0);

	glTexCoord2f(1,1);
	glVertex3f(x+_imageList[textureId].width*scale,y+_imageList[textureId].height*scale,0);
		
	glTexCoord2f(1,0);
	glVertex3f(x+_imageList[textureId].width*scale,y,0);
		
	glTexCoord2f(0,0);
	glVertex3f(x,y,0);
	glEnd();

	gEngfuncs.pTriAPI->RenderMode(kRenderTransTexture);
}
void Graphic::DrawTgaImageDefault(int textureId,int x,int y,int alpha,int r,int g,int b)
{
	if(textureId < 1 || textureId >= _imageIndex) return;
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	//glEnable(GL_ALPHA_TEST);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); 
	

	glBindTexture(GL_TEXTURE_2D, _imageList[textureId].imageId);
	glColor4ub(r,g,b,alpha);

	glBegin(GL_QUADS);
	glTexCoord2f(0,1);
	glVertex3f(x,y+_imageList[textureId].height,0);

	glTexCoord2f(1,1);
	glVertex3f(x+_imageList[textureId].width,y+_imageList[textureId].height,0);
		
	glTexCoord2f(1,0);
	glVertex3f(x+_imageList[textureId].width,y,0);
		
	glTexCoord2f(0,0);
	glVertex3f(x,y,0);
	glEnd();

	gEngfuncs.pTriAPI->RenderMode(kRenderTransTexture);
}
void Graphic::TextureQualityFilter(bool goodGuality)
{
	if(!goodGuality)
	{
		glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR );
		glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
	}
	else 
	{
		glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
		glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
	}
}
int Graphic::IsExists(char *szFileName)
{
	for(int i = 1 ; i < _imageIndex ; i++ )
	{
		if(!strcmp(_imageList[i].name,szFileName)) return i;
	}
	return 0;
}
int Graphic::LoadTgaImage(char *szFileName)
{
	if(IsExists(szFileName))
	{
		//gLog->WriteLog("TGA file:%s already loaded",szFileName);
		return IsExists(szFileName);
	}
	if(!g_pInterface->FileSystem->FileExists(szFileName))
	{
	//	gLog->WriteLog("TGA file:%s not exists",szFileName);
		return -1;
	}
	//gLog->WriteLog("Processing TGA :%s",szFileName);
	FileHandle_t pFile = g_pInterface->FileSystem->Open(szFileName,"r");
	if(pFile == NULL)
	{
	//	gLog->WriteLog("TGA file open error:%s",szFileName);
		return -1;
	}

	int fileLength = g_pInterface->FileSystem->Size(pFile);
	byte *buffer = (byte *)malloc(fileLength);
	g_pInterface->FileSystem->Read(buffer,fileLength,pFile);


	int	i, columns, rows, row_inc, row, col;
	byte	*buf_p, *pixbuf, *targa_rgba;
	byte	palette[256][4], red = 0, green = 0, blue = 0, alpha = 0;
	int	readpixelcount, pixelcount;
	qboolean	compressed;
	tga_t	targa_header;

	buf_p = (byte *)buffer;
	targa_header.id_length = *buf_p++;
	targa_header.colormap_type = *buf_p++;
	targa_header.image_type = *buf_p++;

	targa_header.colormap_index = buf_p[0] + buf_p[1] * 256;		buf_p += 2;
	targa_header.colormap_length = buf_p[0] + buf_p[1] * 256;		buf_p += 2;
	targa_header.colormap_size = *buf_p;				buf_p += 1;
	targa_header.x_origin = *(short *)buf_p;			buf_p += 2;
	targa_header.y_origin = *(short *)buf_p;			buf_p += 2;
	targa_header.width = *(short *)buf_p;		buf_p += 2;
	targa_header.height = *(short *)buf_p;		buf_p += 2;
	targa_header.pixel_size = *buf_p++;
	targa_header.attributes = *buf_p++;
	if( targa_header.id_length != 0 ) buf_p += targa_header.id_length;	// skip TARGA image comment

	if( targa_header.image_type == 1 || targa_header.image_type == 9 )
	{
		// uncompressed colormapped image
		if( targa_header.pixel_size != 8 )
		{
		//	gLog->WriteLog("TGA processing error:Only 8 bit images supported for type 1 and 9");
			return false;
		}
		if( targa_header.colormap_length != 256 )
		{
//gLog->WriteLog("TGA processing error:Only 8 bit colormaps are supported for type 1 and 9");
			return false;
		}
		if( targa_header.colormap_index )
		{
//gLog->WriteLog("TGA processing error:colormap_index is not supported for type 1 and 9");
			return false;
		}
		if( targa_header.colormap_size == 24 )
		{
			for( i = 0; i < targa_header.colormap_length; i++ )
			{
				palette[i][2] = *buf_p++;
				palette[i][1] = *buf_p++;
				palette[i][0] = *buf_p++;
				palette[i][3] = 255;
			}
		}
		else if( targa_header.colormap_size == 32 )
		{
			for( i = 0; i < targa_header.colormap_length; i++ )
			{
				palette[i][2] = *buf_p++;
				palette[i][1] = *buf_p++;
				palette[i][0] = *buf_p++;
				palette[i][3] = *buf_p++;
			}
		}
		else
		{
	//		gLog->WriteLog("TGA processing error:only 24 and 32 bit colormaps are supported for type 1 and 9");
			return false;
		}
	}
	else if( targa_header.image_type == 2 || targa_header.image_type == 10 )
	{
		// uncompressed or RLE compressed RGB
		if( targa_header.pixel_size != 32 && targa_header.pixel_size != 24 )
		{
	//		gLog->WriteLog( "TGA processing error:Only 32 or 24 bit images supported for type 2 and 10");
			return false;
		}
	}
	else if( targa_header.image_type == 3 || targa_header.image_type == 11 )
	{
		// uncompressed greyscale
		if( targa_header.pixel_size != 8 )
		{
		//	gLog->WriteLog( "TGA processing error:Only 8 bit images supported for type 3 and 11");
			return false;
		}
	}

	columns = targa_header.width;
	rows = targa_header.height;

	int size = columns * rows * 4;
	targa_rgba = (byte *)malloc (size);

	pixbuf = targa_rgba + ( rows - 1 ) * columns * 4;
	row_inc = -columns * 4 * 2;	

	compressed = ( targa_header.image_type == 9 || targa_header.image_type == 10 || targa_header.image_type == 11 );
	for( row = col = 0; row < rows; )
	{
		pixelcount = 0x10000;
		readpixelcount = 0x10000;

		if( compressed )
		{
			pixelcount = *buf_p++;
			if( pixelcount & 0x80 )  // run-length packet
				readpixelcount = 1;
			pixelcount = 1 + ( pixelcount & 0x7f );
		}

		while( pixelcount-- && ( row < rows ) )
		{
			if( readpixelcount-- > 0 )
			{
				switch( targa_header.image_type )
				{
				case 1:
				case 9:
					// colormapped image
					blue = *buf_p++;
					red = palette[blue][0];
					green = palette[blue][1];
					alpha = palette[blue][3];
					blue = palette[blue][2];
					//if( alpha != 255 ) image.flags |= IMAGE_HAS_ALPHA;
					break;
				case 2:
				case 10:
					// 24 or 32 bit image
					blue = *buf_p++;
					green = *buf_p++;
					red = *buf_p++;
					alpha = 255;
					if( targa_header.pixel_size == 32 )
					{
						alpha = *buf_p++;
						//if( alpha != 255 )
							//image.flags |= IMAGE_HAS_ALPHA;
					}
					break;
				case 3:
				case 11:
					// greyscale image
					blue = green = red = *buf_p++;
					alpha = 255;
					break;
				}
			}


			*pixbuf++ = red;
			*pixbuf++ = green;
			*pixbuf++ = blue;
			*pixbuf++ = alpha;
			if( ++col == columns )
			{
				row++;
				col = 0;
				pixbuf += row_inc;
			}
		}
	}

	// Make a texture
	_imageList[_imageIndex].height = targa_header.height;
	_imageList[_imageIndex].width = targa_header.width;
	_imageList[_imageIndex].rgba = 4;
	_imageList[_imageIndex].imageId = CreateNewTexture();
	
	int length = strlen(szFileName);
	_imageList[_imageIndex].name = new char[length+1];
	sprintf(_imageList[_imageIndex].name,"%s",szFileName);
	
	//Set texture
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, _imageList[_imageIndex].imageId);
	TextureQualityFilter();
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, _imageList[_imageIndex].width, _imageList[_imageIndex].height, 0, GL_RGBA, GL_UNSIGNED_BYTE, targa_rgba);
	free(targa_rgba);
	free(buffer);
	g_pInterface->FileSystem->Close(pFile);
	
	
	//gLog->WriteLog("Tga loaded succeed.Infomation:[width:%d ,height:%d ,assign texture id:%d]",_imageList[_imageIndex].width, _imageList[_imageIndex].height,_imageList[_imageIndex].imageId);
	_imageIndex ++; // next
	return _imageIndex-1;
}