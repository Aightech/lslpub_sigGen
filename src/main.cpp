#include <vector>
#include <fstream>
#include <iostream>
#include <stdlib.h> 
#include <unistd.h>

#include <lsl_cpp.h>
#include "tools.h"



int main(int argc, char ** argv)
{
  std::vector<std::string> opt_flag(
				    {"-ni",
					"-nl",
					"-nb"});
  std::vector<std::string> opt_label(
				     {"Lsl sample stream's name",
					 "Lsl label stream's name",
					 "Number of channels"});
  std::vector<std::string> opt_value(
				     {"generator_sample",
					 "generator_label",
					 "5"});
  get_arg(argc, argv, opt_flag, opt_label, opt_value);
  
  std::string stream_sample_name = opt_value[0];
  std::string stream_label_name = opt_value[1];
  int nb_ch = std::stoi(opt_value[2]);


  int t = 0;
  srand (time(NULL));

  try
    {
      lsl::stream_info info_sample(stream_sample_name, "sample", nb_ch, 0, lsl::cf_float32);
      lsl::stream_info info_label(stream_label_name, "label", 1, 0, lsl::cf_float32);
      lsl::stream_outlet outlet_sample(info_sample);
      lsl::stream_outlet outlet_label(info_label);

      std::vector<float> sample(nb_ch);
      std::vector<float> label(1);
      std::cout << "[INFOS] Now sending data... " << std::endl;  
      for(;;t++)
	{
	  //generate
	  for(int i = 0; i< sample.size(); i++)
	    sample[i] = ((t/100)%2)?1:0;
	  
	  //label
	  label[0] = (sample[0]==1)?1:0;

	  //noise
	  int a =50;
	  float amp = 0.1;
	  for(int i = 0; i<sample.size(); i++)
	    {
	      sample[i] += (float)( rand() % (2*a) - a) / (float)a   *amp;
	      sample[i] = (sample[i]<0)?0:(sample[i]>1)?1:sample[i];
	    }
	    
	  // send it
	  outlet_sample.push_sample(sample);		
	  outlet_label.push_sample(label);
	  usleep(10000);
	}

    }
  catch (std::exception& e)
    {
      std::cerr << "[ERROR] Got an exception: " << e.what() << std::endl;
    }

  
    
  return 0;
}
