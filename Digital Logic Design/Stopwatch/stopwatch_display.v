`timescale 1ns / 1ps

module stopwatch_display(
    input clk,
    input [6:0] in0, 
    input [6:0] in1, 
    input [6:0] in2, 
    input [6:0] in3,
    output reg [3:0] an,
    output reg [6:0] sseg,
    output reg dp
    );
    
    reg [1:0] state;
    reg [1:0] next_state;
    
    always @ (*) begin
            case (state)
                2'b00: begin
                    an = 4'b0111;
                    sseg = in0;
                    dp = 1'b1;
                    next_state = 2'b01;
                    end
                2'b01: begin
                    an = 4'b1011;
                    sseg = in1;
                    dp = 1'b0;
                    next_state = 2'b10;
                    end
                2'b10: begin
                    an = 4'b1101; 
                    sseg = in2;
                    dp = 1'b1;
                    next_state = 2'b11;
                    end
                2'b11: begin
                    an = 4'b1110; 
                    sseg = in3;
                    dp = 1'b1;
                    next_state = 2'b00;
                    end
            endcase
        end
        
        always @(posedge clk) begin
            state <= next_state;
        end
    
endmodule