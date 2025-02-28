#include <vector>
#include <fstream>
#include <iostream>
#include <stdlib.h> 
#include <unistd.h>
#ifdef WIN32
#include <time.h>
#endif
#include <lsl_cpp.h>
#include "tools.h"



int main(int argc, char ** argv)
{
  std::vector<std::string> opt_flag(
				    {"-ni",
					"-nl",
					"-nbi",
				     "-nbl",
				    "-dt"});
  std::vector<std::string> opt_label(
				     {"Lsl sample stream's name",
					 "Lsl label stream's name",
					 "Number of channels of the input",
				      "Number of channels of the label",
				     "Delay between sample"});
  std::vector<std::string> opt_value(
				     {"generator_sample",
					 "generator_label",
					 "4",
				      "15",
				     "20000"});
  get_arg(argc, argv, opt_flag, opt_label, opt_value);
  
  std::string stream_sample_name = opt_value[0];
  std::string stream_label_name = opt_value[1];
  int nb_ch = std::stoi(opt_value[2]);
  int nb_ch_l = std::stoi(opt_value[3]);
  int dt = std::stoi(opt_value[4]);

  int t = 0;
  srand (time(NULL));

  try
    {
      lsl::stream_info info_sample(stream_sample_name, "sample", nb_ch, 0, lsl::cf_float32);
      lsl::stream_info info_label(stream_label_name, "label", nb_ch_l, 0, lsl::cf_float32);
      lsl::stream_outlet outlet_sample(info_sample);
      lsl::stream_outlet outlet_label(info_label);

      std::vector<float> sample(nb_ch);
      std::vector<float> label(nb_ch_l);
      std::cout << "[INFOS] Now sending data... " << std::endl;  
      for(;;t++)
	{
	  int r = 70;
	  label[3] = abs((t%r - (t/r)%2*(r-1))%r);
	  //std::cout << label[3] << std::endl;
	    // (i==abs((m_time%10 - (m_time/10)%2*9)%10))?'o':' ';
	  
	  //generate
	  for(int i = 0; i< sample.size(); i++)
	    sample[i] = label[3]/i+3;//(((t+i)/100)%2)?-0.5:0.5;
	  
	  

	  //noise
	  int a =50;
	  float amp = 0.1;
	  for(int i = 0; i<sample.size(); i++)
	    {
	      sample[i] += (float)( rand() % (2*a) - a) / (float)a   *amp;
	      // sample[i] = (sample[i]<0)?0:(sample[i]>1)?1:sample[i];
	    }

	  // //label
	  // for(int i = 0; i< label.size(); i++)
	  //   label[i] = 10*sample[i%sample.size()] * sample[(i+1)%sample.size()];

	  for(int i = 0; i<label.size(); i++)
	    {
	      //label[i] = (label[i]<0)?0:(label[i]>1)?1:label[i];
	    }
	    
	  // send it
	  outlet_sample.push_sample(sample);		
	  outlet_label.push_sample(label);
	  usleep(dt);
	}

    }
  catch (std::exception& e)
    {
      std::cerr << "[ERROR] Got an exception: " << e.what() << std::endl;
    }

  
    
  return 0;
}
