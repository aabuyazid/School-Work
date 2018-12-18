`timescale 1ns / 1ps

module stopwatch_states(
    input clk,
    input reset,
    input start_stop,
    input [1:0] mode,
    input [13:0] ext_load,
    output reg [13:0] counter
    );
    
    reg start_or_stop;
    reg [1:0] curr_mode;
    
    reg [4:0] new_clk;
    
    initial begin
    
    start_or_stop = 1;
    new_clk = 0;
    curr_mode = mode;
    
    end
    
    always @ (posedge clk) begin
        if(reset) begin
            case (mode)
                2'b00: counter <= 0;
                2'b10: counter <= 9999;
                default: counter <= ext_load;
            endcase
            curr_mode <= mode;
        end
        
        else if(start_or_stop) begin
            case(curr_mode[1])
                1'b0: begin
                    if(counter == 9999) counter <= counter;
                    else counter <= counter + 1;
                end
                1'b1: begin
                    if(counter == 0) counter <= counter;
                    else counter <= counter - 1;
                end
            endcase
        end
        new_clk = new_clk + 1;
    end
    
    always @ (posedge new_clk[4]) begin
        if(start_stop) start_or_stop <= ~start_or_stop;
    end
    
endmodule


