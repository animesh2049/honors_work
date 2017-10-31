############################################################
## This file is generated automatically by Vivado HLS.
## Please DO NOT edit it.
## Copyright (C) 1986-2016 Xilinx, Inc. All Rights Reserved.
############################################################
set_directive_interface -mode axis "pipeline_mask" input
set_directive_interface -mode axis "pipeline_mask" mask
set_directive_stream -depth 10 -dim 1 "pipeline_mask" blurx_out_stream
set_directive_stream -depth 20 -dim 1 "pipeline_mask" input_sharpen_stream
set_directive_stream -depth 10 -dim 1 "pipeline_mask" blury_out_stream
set_directive_stream -depth 10 -dim 1 "pipeline_mask" sharpen_out_stream
set_directive_stream -depth 10 -dim 1 "pipeline_mask" blury_mask_stream
set_directive_stream -depth 10 -dim 1 "pipeline_mask" input_mask_stream
set_directive_dataflow "pipeline_mask"
set_directive_pipeline -II 1 "pipeline_mask/Pipe1"
set_directive_pipeline -II 1 "pipeline_mask/Pipe2"
set_directive_pipeline -II 1 "pipeline_mask/Pipe3"
set_directive_pipeline -II 1 "pipeline_mask/Pipe4"
set_directive_array_partition -type complete -dim 1 "pipeline_mask" blury_blurx_Coeff
set_directive_array_partition -type block -factor 5 -dim 1 "pipeline_mask" blury_blurx_LBuffer
set_directive_array_partition -type complete -dim 1 "pipeline_mask" blurx_input_Coeff
set_directive_array_partition -type complete -dim 1 "pipeline_mask" blurx_input_WBuffer
set_directive_unroll "pipeline_mask/pipeline_mask_label0"
set_directive_unroll "pipeline_mask/pipeline_mask_label1"
set_directive_unroll "pipeline_mask/pipeline_mask_label3"
set_directive_unroll "pipeline_mask/pipeline_mask_label4"