#include <iostream>
#include <cstdint>
#include <cstring>
#include <getopt.h>

namespace
{
  std::string inputFileName;
  std::string outputFileName;
  std::string memoryLimits;
  
  void parseArguments(int argc, char** argv)
  {
      const std::string opts = "i:o:l:";
      
      int opt = 0;
    
      while( (opt = getopt(argc, argv, opts.c_str())) != -1)
      {
      
	switch (opt)
	{
	
	  case 'i':	
	  {
	    if(optarg)	    
	      inputFileName = optarg;
	    else
	    {
	      std::cout << "Input file name is empty" << std::endl;
	      
	      return;
	    }
	  }
	  break;
	
	  case 'o':
	  {
	    if(optarg)	    
	      outputFileName = optarg;
	    else
	    {
	      std::cout << "Output file name is empty" << std::endl;
	      
	      return;
	    }
	  }
	  break;
	
	  case 'l':
	  {
	    if(optarg)
	      memoryLimits = optarg;
	    else 
	    {
	      std::cout << "Memeory limit is empty. PLease specify it" << std::endl;
	      
	      return;
	    }
	  }
	  break;
	
	  case '?':
	  {
	    std::cout << "Wrong argument" << std::endl;
	  
	    return;
	  }
	  break;
	
	  default:
	  {
	    std::cout << "Unknown argument" << std::endl;
	  
	    return;
	  }
	  break;
      }
    }
      
  }
  
}

int main(int argc, char** argv) 
{      
    parseArguments(argc, argv);
  
    std::cout << "Input file name: " 	<< inputFileName << std::endl;
    std::cout << "Output file name: " 	<< outputFileName << std::endl;
    std::cout << "Memeory limit is: " 	<< memoryLimits << std::endl;
    
    return 0;
}
