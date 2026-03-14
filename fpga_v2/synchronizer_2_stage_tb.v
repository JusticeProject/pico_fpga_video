// time unit / time precision
`timescale 1ns / 10ps

module synchronizer_2_stage_tb();

// output signal of the uut
wire sync;

// input signals to the uut
reg clk = 0;
reg async = 0;

// Generate clock signal
always
begin
    // delay for 10 time units (half a clock period), then change the clock
    #10
    clk = ~clk;
end

// instantiate the uut
synchronizer_2_stage uut (.clk(clk), .async(async), .sync(sync));

// give it some input
initial
begin
    #20
    async = 1'b1;
    #80
    async = 1'b0;
    #40
    async = 1'b1;
end

endmodule
