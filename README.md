# LE_Demo_iOS
This is a melodify demo by SDK of Little Endian.

There is somthing we need to config:

1. Change Search Paths of Build Settings :
	  
	Header Search Paths  
	+ $(SRCROOT)/include   
	
	Library Search Paths  
	
	+ $(SRCROOT)/libs/development  
	+ $(PROJECT_DIR)/libs/development  

2. Change Language of Build Settings :

	Compile Sources As 
	+ Objective-C++
