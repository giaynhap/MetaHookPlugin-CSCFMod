/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*|||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
|||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
LineRun V1 beta||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
|||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
Done by: tornado_way|||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
|||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
(C)2010||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
|||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
|||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||*/
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class StringFinder
{

public:
	void clear();
	void add(const char* the_string, int the_number);
	void erase( const char* str );

	
	bool find(const char* str);
	int          num;
	const char*  str;

	// iteration. use with for(;;)
	void it_start  ();
	bool it_running();
	void it_next   ();


private:
	void* data;


public:
	StringFinder();
	~StringFinder();
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

