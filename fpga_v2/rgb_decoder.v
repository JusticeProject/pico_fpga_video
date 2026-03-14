module rgb_decoder(
    input wire clk,
    input wire reset,
    input wire video_on,
    input wire [1:0] green_blue,
    output wire [2:0] rgb
    );

// signals
wire red;
wire [2:0] rgb_next;
reg [2:0] rgb_reg;

// grab the data on the falling edge because otherwise lines on the monitor look blurry
always @(posedge reset, negedge clk)
    if (reset)
        rgb_reg <= 3'b000;
    else
        rgb_reg <= rgb_next;

// next-state logic
assign red = green_blue[1] && green_blue[0];
assign rgb_next = {red, green_blue};

// output logic
assign rgb = (video_on) ? rgb_reg : 3'b000;

endmodule
