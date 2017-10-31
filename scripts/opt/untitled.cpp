#include "unsharp_mask.h"
#include <stdio.h>

void pipeline_mask(data_tc1 thresh, data_tc  weight, hls::stream<int> &input, hls::stream<data_t3> &mask)
{

    const int  _ct3 = 3;
    const int  _ct4 = R+4;
    const int  _ct5 = C+4;
    hls::stream<data_t2> blury_out_stream("blury_out_strm");//empty
    hls::stream<int> input_sharpen_stream("input_sharpen_strm");//full
    int  blury_blurx_LBuffer[5][_ct5];
    const int  blury_blurx_Coeff[5] = {1,4,6,4,1};

    for (int  _ci3 = 0; (_ci3 <= (_ct3 - 1)); _ci3 = (_ci3 + 1))
    {
      for (int  _ci4 = 0; (_ci4 <= (_ct4 - 1)); _ci4 = (_ci4 + 1))
      {
        Pipe2:for (int  _ci5 = 0; (_ci5 <= (_ct5 - 1)); _ci5 = (_ci5 + 1))
        {
          
        int blurx_in_val = input.read();
        int out_val = 0;
        
        pipeline_mask_label3:for(int i=1;i<5;i++){
          blury_blurx_LBuffer[i-1][_ci5] = blury_blurx_LBuffer[i][_ci5];
        }

                        blury_blurx_LBuffer[4][_ci5] = blurx_in_val;

          pipeline_mask_label0:for(int i=0;i<5;i++){
              out_val += blury_blurx_LBuffer[i][_ci5] * blury_blurx_Coeff[i];
          }

      if(_ci4>=4)
      {
        if(_ci5>=2 && _ci5<_ct5-2)
                      	{input_sharpen_stream.write(blury_blurx_LBuffer[2][_ci5]);} //if(blury_blurx_LBuffer[2][_ci5]!=-2 && flag=0)count1++;else flag=1;}
        		blury_out_stream.write(out_val*data_c1(0.0625));
      }
        }
      }
    }


  const int  _ct0 = 3;
  const int  _ct1 = R;
  const int  _ct2 = C+4;
  hls::stream<data_t1> blurx_out_stream("blurx_out_strm");//empty
  
  data_t2  blurx_input_WBuffer[5];
  const int  blurx_input_Coeff[5] = {1,4,6,4,1};

  for (int  _ci0 = 0; (_ci0 <= (_ct0 - 1)); _ci0 = (_ci0 + 1))
  {
    for (int  _ci1 = 0; (_ci1 <= (_ct1 - 1)); _ci1 = (_ci1 + 1))
    {
      Pipe1:for (int  _ci2 = 0; (_ci2 <= (_ct2 - 1)); _ci2 = (_ci2 + 1))
      {
        
		    data_t1 input_in_val = blury_out_stream.read();
		    data_t1 out_val = 0;

	    pipeline_mask_label4:for(int i=0;i<4;i++){
			blurx_input_WBuffer[i] = blurx_input_WBuffer[i+1];
	    }

                blurx_input_WBuffer[4] = input_in_val;
                
                pipeline_mask_label1:for(int i=0;i<5;i++){
                          out_val += blurx_input_WBuffer[i] * blurx_input_Coeff[i];
                }
                
            		if(_ci2 >= 4){
          		    	
          				blurx_out_stream.write(out_val*data_c1(0.0625));
          				
			}
      }
    }
  }

  const int  _ct6 = 3;
  const int  _ct7 = R;
  const int  _ct8 = C;
  hls::stream<data_t3> sharpen_out_stream("sharpen_out_strm");
  hls::stream<data_t3> blury_mask_stream("blury_mask_strm");
  hls::stream<int> input_mask_stream("input_mask_strm");

  for (int  _ci6 = 0; (_ci6 <= (_ct6 - 1)); _ci6 = (_ci6 + 1))
  {
    for (int  _ci7 = 0; (_ci7 <= (_ct7 - 1)); _ci7 = (_ci7 + 1))
    {
      Pipe3:for (int  _ci8 = 0; (_ci8 <= (_ct8 - 1)); _ci8 = (_ci8 + 1))
      {
    	    int input_in_val = input_sharpen_stream.read(); 

			data_t2 blury_in_val = blurx_out_stream.read();
			//std::cout << "Count value = " << count1 << std::endl; exit(0);
			data_t3 part=input_in_val + weight*(input_in_val - blury_in_val);
			sharpen_out_stream.write(part);
			blury_mask_stream.write(blury_in_val);
			input_mask_stream.write(input_in_val);
	  }
    }
  }


  const int  _ct9 = 3;
  const int  _ct10 = R;
  const int  _ct11 = C;
  for (int  _ci9 = 0; (_ci9 <= (_ct9 - 1)); _ci9 = (_ci9 + 1))
  {
    for (int  _ci10 = 0; (_ci10 <= (_ct10 - 1)); _ci10 = (_ci10 + 1))
    {
      Pipe4:for (int  _ci11 = 0; (_ci11 <= (_ct11 - 1)); _ci11 = (_ci11 + 1))
      {
    	        	  int input_in_val = input_mask_stream.read(); 
		  data_t3 sharpen_in_val = sharpen_out_stream.read();
		  data_t3 blury_in_val = blury_mask_stream.read();
		  data_t4 abso=fabs(input_in_val - blury_in_val);
//std::cout << "Row = " << _ci10 << "Col = " << _ci11 << std::endl;
		  mask.write(((abso < thresh)?(data_t3)input_in_val:sharpen_in_val));
		  
	  }
    }
  }
}
