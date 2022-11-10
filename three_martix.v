module three_martix(
	input 					clk,
    input 					rst_n,
    
    input	[7:0]			img_process
 );


reg     [7:0]   data_in1    ;   //fifo1写数据
reg     [7:0]   data_in2    ;   //fifo2写数据





fifo_1	u_fifo_1(
    .clk(clk),
    .rst(rst)
    
    .data    (data_in1   ),  // input [7 : 0] din
    .we  	 (wr_en1     ),  	// input wr_en
    .re  	 (rd_en      ),  	// input rd_en

    .do      (data_out1  )   // output [7 : 0] dout
);


fifo_2	u_fifo_2(
    
    
);

endmodule
