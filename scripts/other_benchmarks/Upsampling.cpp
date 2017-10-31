#include "Upsampling.h"

void upsample(hls::stream<uint8> & img_colour, hls::stream<uint8> & U_upsampled_L)
{

	  const int_t  _ct0 = 3;
	  const int_t  _ct1 = (4 + R);
	  const int_t  _ct2 = (4 + C);
	  hls::stream<data_t1> Dx_downsampled_L_out_stream;
	  uint8  Dx_downsampled_L_img_colour_WBuffer[4];
	  for (int_t  _ci0 = 0; (_ci0 <= (_ct0 - 1)); _ci0 = (_ci0 + 1))
	  {
		  for (int_t  _ci1 = 0; (_ci1 <= (_ct1 - 1)); _ci1 = (_ci1 + 1))
		  {
			  pipe2:for (int_t  _ci2 = 0; (_ci2 <= (_ct2 - 1)); _ci2 = (_ci2 + 1))
			  {
				  for(int_t i=0;i<4;i++){
					  Dx_downsampled_L_img_colour_WBuffer[i] = i < (4-1) ? Dx_downsampled_L_img_colour_WBuffer[i+1] : img_colour.read();
				  }
				  if((_ci2%2)==0){
					  if((_ci2>=4) && (_ci2 <= (C + 2)))
					  {
						  data_t1 val = (Dx_downsampled_L_img_colour_WBuffer[0] + (3 * Dx_downsampled_L_img_colour_WBuffer[1]) + (3 * Dx_downsampled_L_img_colour_WBuffer[2]) + Dx_downsampled_L_img_colour_WBuffer[3]) * data_tc(0.125);
						  Dx_downsampled_L_out_stream.write(val);
					  }
				  }
			  }
		  }
	  }


		const int_t _ct3 = 3;
		const int_t  _ct4 = (R + 4);
		const int  _ct5 = ((C+1)/2);
		hls::stream<data_t2> D_downsampled_L;
		data_t1  D_downsampled_L_Dx_downsampled_L_LBuffer[4][_ct5];
		data_t1  D_downsampled_L_Dx_downsampled_L_WBuffer[4];

		for (int_t  _ci0 = 0; (_ci0 <= (_ct3 - 1)); _ci0 = (_ci0 + 1))
		{
		  for (int_t  _ci4 = 0; (_ci4 <= (_ct4 - 1)); _ci4 = (_ci4 + 1))
		  {
			pipe1:for (int_t  _ci5 = 0; (_ci5 <= (_ct5 - 1)); _ci5 = (_ci5 + 1))
			{

				for(int_t i=0;i<4;i++){
					D_downsampled_L_Dx_downsampled_L_LBuffer[i][_ci5] = (i<(4-1)) ? D_downsampled_L_Dx_downsampled_L_LBuffer[i+1][_ci5] : Dx_downsampled_L_out_stream.read();
				}

				if((_ci4 % 2) == 0){
					if((_ci4>=4) && (_ci4 <= (R+2))){
						for(int_t i=0;i<4;i++){
							D_downsampled_L_Dx_downsampled_L_WBuffer[i] = D_downsampled_L_Dx_downsampled_L_LBuffer[i][_ci5];
						}
						data_t2 val = ((D_downsampled_L_Dx_downsampled_L_WBuffer[0] + (3 * D_downsampled_L_Dx_downsampled_L_WBuffer[1]) + (3 * D_downsampled_L_Dx_downsampled_L_WBuffer[2]) + D_downsampled_L_Dx_downsampled_L_WBuffer[3]) * data_tc(0.125));
						D_downsampled_L.write(val);
					}
				}
			}
		  }
		}


  const int_t  _ct6 = 3;
  const int_t  _ct7 = ((R+1)/2);
  const int_t  _ct8 = (C-2);
  hls::stream<data_t3> Ux_upsampled_L_out_stream;
  data_t2  Ux_upsampled_L_img_colour_WBuffer[2];
  for (int_t  _ci0 = 0; (_ci0 <= (_ct6 - 1)); _ci0 = (_ci0 + 1))
  {
	for (int_t  _ci1 = 0; (_ci1 <= (_ct7 - 1)); _ci1 = (_ci1 + 1))
	{
	  pipe4:for (int_t  _ci2 = 0; (_ci2 <= (_ct8 - 1)); _ci2 = (_ci2 + 1))
	  {
		  bool isEven = false;
		  if((_ci2%2) == 0){
			  isEven = true;
		  }
		    if(isEven||(_ci2 < 2)){
				data_t2 img_colour_in_val = D_downsampled_L.read();
				for(int_t i=0;i<2;i++){
					Ux_upsampled_L_img_colour_WBuffer[i] = i < (2-1) ? Ux_upsampled_L_img_colour_WBuffer[i+1] : img_colour_in_val;
				}
		    }


			if(_ci2 >= 2)
			{
				    if(isEven){
						data_t3 val = ((1 * Ux_upsampled_L_img_colour_WBuffer[1]) + (3 * Ux_upsampled_L_img_colour_WBuffer[0])) * data_tc(0.25);
						Ux_upsampled_L_out_stream.write(val);
				    }
				    else{
						data_t3 val = ((1 * Ux_upsampled_L_img_colour_WBuffer[0]) + (3 * Ux_upsampled_L_img_colour_WBuffer[1])) * data_tc(0.25);
						Ux_upsampled_L_out_stream.write(val);
				    }
			}
		}
	  }
	}


  const int_t  _ct9 = 3;
    const int_t  _ct10 = (R);
    const int  _ct11 = (C-4);
    data_t4 outputBuffer[_ct11];
    data_t3  U_upsampled_L_Ux_upsampled_L_LBuffer[2][_ct11];
    data_t3  U_upsampled_L_Ux_upsampled_L_WBuffer[2];
    for (int_t  _ci3 = 0; (_ci3 <= (_ct9 - 1)); _ci3 = (_ci3 + 1))
    {
      for (int_t  _ci4 = 0; (_ci4 <= (_ct10 - 1)); _ci4 = (_ci4 + 1))
      {
      	  pipe3:for (int_t  _ci5 = 0; (_ci5 <= (_ct11 - 1)); _ci5 = (_ci5 + 1))
  		  {
  			  if((_ci4%2)==0){
  					data_t3 Ux_upsampled_L_in_val = Ux_upsampled_L_out_stream.read();

  					for(int_t i=0;i<2-1;i++){
  						U_upsampled_L_Ux_upsampled_L_LBuffer[i][_ci5] = U_upsampled_L_Ux_upsampled_L_LBuffer[i+1][_ci5];
  					}
  					U_upsampled_L_Ux_upsampled_L_LBuffer[2-1][_ci5] = Ux_upsampled_L_in_val;

  					if((_ci4 >= 2))
  					{

  							for(int_t i=0;i<2;i++){
  								U_upsampled_L_Ux_upsampled_L_WBuffer[i] = U_upsampled_L_Ux_upsampled_L_LBuffer[i][_ci5];
  							}

  							data_t4 val = ((1 * U_upsampled_L_Ux_upsampled_L_WBuffer[1]) + (3 * U_upsampled_L_Ux_upsampled_L_WBuffer[0])) * data_tc(0.25);
  							U_upsampled_L.write(val.to_uint());
  							val = ((1 * U_upsampled_L_Ux_upsampled_L_WBuffer[0]) + (3 * U_upsampled_L_Ux_upsampled_L_WBuffer[1])) * data_tc(0.25);
  							outputBuffer[_ci5] = val;
  					}
				}
				else if(_ci4 >= 2){
					U_upsampled_L.write(outputBuffer[_ci5].to_uint());
				}
  			}
      	}
     }
}