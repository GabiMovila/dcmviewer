#include "DCMExtractor.h"
#include <QtWidgets/QApplication>
#include <vld.h>


int main(int argc, char* argv[])
{
	//ofstream outfile("new.xml");	 
	//file->writeJson();
	//	file->writeXML();;
	//const DcmTagKey& tag;
	
	QApplication a(argc, argv);
	DCMExtractor w;
	w.show();

	return a.exec();
}  
